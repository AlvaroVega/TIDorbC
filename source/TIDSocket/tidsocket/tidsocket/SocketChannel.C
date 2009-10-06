///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SocketChannel.C
// Description: This file contains the SocketChannel class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
//   02/29/2008 by Alvaro Polo <apv@tid.es>
//      * Some ioctl constants defined conditionally for Darwin
//        (Mac OS X) systems
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"
#include "SocketChannelStatus.h"
#include "PlainSocketImpl.h"

#ifdef __sun
#include <sys/conf.h>
#include <inttypes.h>
#endif //__sun

#include <sys/types.h>
#include <sys/socket.h>
#ifdef __darwin
   #include <sys/ioctl.h>
#else
   #include <stropts.h>
#endif

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>




using TIDThr::Synchronized;




namespace TIDSocket
{




//
// Constructor
//
SocketChannel::SocketChannel()
    throw(SystemException)
{
    _socket  = NULL;
    _istream = NULL;
    _ostream = NULL;
}




//
// Destructor
//
SocketChannel::~SocketChannel()
    throw(SystemException)
{
    delete _istream;
    delete _ostream;
    delete _socket;
}




//
// connect()
//
bool SocketChannel::connect(const SocketAddress& remote)
    throw(AlreadyConnectedException, ConnectionPendingException,
          ClosedChannelException, AsynchronousCloseException,
          ClosedByInterruptException, UnresolvedAddressException,
          UnsupportedAddressTypeException, IOException)
{
    int error = 0;

    try
    {
        // Sincroniza esta operacion con finishConnect()
        Synchronized synchronized(_connect);
        {
            // Comprueba si ya esta conectado
            if (isConnected())
            {
                throw AlreadyConnectedException("Channel already connected");
            }

            // Comprueba si hay una operacion de conexion en curso
            if (isConnectionPending())
            {
                throw ConnectionPendingException(
                          "Non-blocking connection already in progress");
            }

            // Comprueba si esta cerrado
            if (! isOpen())
            {
                throw ClosedChannelException("Channel is closed");
            }

            // Comprueba si 'remote' es un objeto InetSocketAddress
            const InetSocketAddress* remoteptr =
                dynamic_cast<const InetSocketAddress*>(&remote);

            if (remoteptr == NULL)
            {
                throw UnsupportedAddressTypeException("Invalid SocketAddress");
            }

            // Comprueba si la direccion SockAddress esta resuelta
            if (remoteptr->isUnresolved())
            {
                throw UnresolvedAddressException(
                          "Remote address not fully resolved");
            }

            // Comprueba si esta en modo bloqueante
            if (isBlocking())
            {
                // Intenta la conexion
                _socket->connect(remote);

                // Modifica el estado
                Synchronized synchronized(_sync);
                {
                    _status |= TID_SOCKET_CHANNEL_STATUS_CONNECTED;
                }

                return true;
            }

            // Prepara los datos de la conexion
            PlainSocketImpl::toSockAddr(remoteptr->getAddress(),
                                        remoteptr->getPort(),
                                        _sockaddr,
                                        _socksize);

            // Conexion no bloqueante
            int fd = (int) _socket->_impl->getFileDescriptor();
            error = ::connect(fd, &_sockaddr, _socksize);
            if (error && (errno != EINPROGRESS))
            {
                throw IOException("connect() error", errno);
            }

            // Modifica el estado
            Synchronized synchronized(_sync);
            {
                if (error == 0) _status |= TID_SOCKET_CHANNEL_STATUS_CONNECTED;
                else _status |= TID_SOCKET_CHANNEL_STATUS_CONNECTION_PENDING;
            }

        } // Fin sincronizacion con finishConnect()
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return (error == 0);
}




//
// finishConnect
//
bool SocketChannel::finishConnect()
    throw(NoConnectionPendingException, ClosedChannelException,
          AsynchronousCloseException, ClosedByInterruptException, IOException)
{
    int error = 0;

    try
    {
        // Sincroniza esta operacion con connect()
        Synchronized synchronized(_connect);
        {
            // Comprueba si hay pendiente una conexion
            if (! isConnected() || ! isConnectionPending())
            {
                throw NoConnectionPendingException("No connection pending");
            }

            // Comprueba si esta cerrado
            if (! isOpen())
            {
                throw ClosedChannelException("Channel is closed");
            }

            // Comprueba si esta en modo bloqueante
            if (isBlocking())
            {
                InetSocketAddress* remote = NULL;
                try
                {
                    // Recupera los datos de la conexion
                    remote = PlainSocketImpl::toInetSocketAddress(&_sockaddr,
                                                                  _socksize);

                    // Reintenta la conexion con los datos de connect()
                    _socket->connect(*remote);
                    delete remote;

                    // Modifica el estado
                    Synchronized synchronized(_sync);
                    {
                        _status &=~TID_SOCKET_CHANNEL_STATUS_CONNECTION_PENDING;
                        _status |= TID_SOCKET_CHANNEL_STATUS_CONNECTED;
                    }
                }
                catch(...)
                {
                    delete remote;
                    throw;
                }
                return true;
            }

            // Conexion no bloqueante
            int fd = (int) _socket->_impl->getFileDescriptor();
            error = ::connect(fd, &_sockaddr, _socksize);

            if (error &&
                errno != EINPROGRESS && errno != EALREADY && errno != EISCONN)
            {
                throw IOException("finishConnect() error", errno);
            }

            // Modifica el estado
            if (error==0 || errno==EISCONN)
            {
                Synchronized synchronized(_sync);
                {
                    _status &=~TID_SOCKET_CHANNEL_STATUS_CONNECTION_PENDING;
                    _status |= TID_SOCKET_CHANNEL_STATUS_CONNECTED;
                }
            }

        } // Fin sincronizacion con finishConnect()
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return (error==0 || errno==EISCONN);
}




//
// isConnected()
//
bool SocketChannel::isConnected()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_CHANNEL_STATUS_CONNECTED);
        }
    }
    catch(TIDThr::Exception& e)
    {
    }

    return false;
}




//
// isConnectionPending()
//
bool SocketChannel::isConnectionPending()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_CHANNEL_STATUS_CONNECTION_PENDING);
        }
    }
    catch(TIDThr::Exception& e)
    {
    }

    return false;
}




//
// read()
//
ssize_t SocketChannel::read(unsigned char* dst, size_t dst_len)
    throw (NotYetConnectedException, IOException)
{
    // Comprueba si esta conectado
    if (! isConnected())
    {
        throw NotYetConnectedException("Channel not connected");
    }

    // Comprueba si el extremo de lectura esta cerrado
    if (_socket->isInputShutdown())
    {
        return -1;
    }

    // Obtiene el stream de entrada del socket
    try
    {
        Synchronized synchronized(_sync);
        {
            if (_istream == NULL)
            {
                _istream = _socket->getInputStream();
            }
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return _istream->read(dst, dst_len);
}




//
// socket()
//
const Socket* SocketChannel::socket()
    throw()
{
    return (const Socket*) _socket;
}




//
// validOps()
//
int SocketChannel::validOps()
    throw()
{
    return ( SelectionKey::OP_CONNECT |
             SelectionKey::OP_READ    |
             SelectionKey::OP_WRITE );
}




//
// write()
//
ssize_t SocketChannel::write(const unsigned char* src, size_t src_len)
    throw (NotYetConnectedException, ClosedChannelException, IOException)
{
    // Comprueba si esta conectado
    if (! isConnected())
    {
        throw NotYetConnectedException("Channel not connected");
    }

    // Comprueba si el extremo de escritura esta cerrado
    if (_socket->isOutputShutdown())
    {
        throw ClosedChannelException("Channel is closed");
    }

    // Obtiene el stream de entrada del socket
    try
    {
        Synchronized synchronized(_sync);
        {
            if (_ostream == NULL)
            {
                _ostream = _socket->getOutputStream();
            }
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return _ostream->write(src, src_len);
}




//
// implConfigureBlocking()
//
void SocketChannel::implConfigureBlocking(bool block)
    throw(IOException)
{
    // Flags actuales
    int flags;
    int fd = (int) _socket->_impl->getFileDescriptor();
    if ((flags = fcntl(fd, F_GETFL)) == -1)
    {
        throw IOException("configureBlocking() error", errno);
    }

    if (block) flags &= ~O_NONBLOCK;
    else flags |= O_NONBLOCK;

    // Establece los nuevos flags
    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        throw IOException("configureBlocking() error", errno);
    }
}




//
// implCloseChannel()
//
void SocketChannel::implCloseChannel()
    throw(IOException)
{
    _socket->close();
}




//
// implRegisterChannel()
//
SelectionKey& SocketChannel::implRegisterChannel(Selector& sel, int ops)
    throw()
{
    int fd = (int) _socket->_impl->getFileDescriptor();
    return sel.registerKey(fd, ops, (SelectableChannel*) this);
}




//
// static open()
//
SocketChannel* SocketChannel::open()
    throw(IOException)
{
    SocketChannel* channel = NULL;

    try
    {
        channel = new SocketChannel;

        // Crea un socket no conectado
        channel->_socket = new Socket;

        // Fija el estado del canal
        channel->_status |= TID_SOCKET_CHANNEL_STATUS_OPEN;

        // Fija en el socket la referencia a este nuevo canal creado
        channel->_socket->_channel = channel;
    }
    catch (TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return channel;
}




//
// static open()
//
SocketChannel* SocketChannel::open(const SocketAddress& sock)
    throw(AsynchronousCloseException, ClosedByInterruptException,
          UnresolvedAddressException, UnsupportedAddressTypeException,
          IOException)
{
    // Crea un nuevo canal
    SocketChannel* channel = open();
    channel->connect(sock);
    return channel;
}




} //namespace TIDSocket
