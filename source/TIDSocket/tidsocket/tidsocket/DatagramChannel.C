///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramChannel.C
// Description: This file contains the DatagramChannel class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
//   02/29/2008 by Alvaro Polo <apv@tid.es>
//      * Some ioctl constants defined conditionally for Darwin
//        (Mac OS X) systems
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"
#include "SocketChannelStatus.h"

#ifdef __sun
#include <sys/conf.h>
#include <inttypes.h>
#endif //__sun

#include <sys/types.h>
#include <sys/socket.h>
#ifndef __darwin
   #include <stropts.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>




/*
   TODO: DatagramChannel not fully implemented.
   See Java 2 Platform SE documentation.
*/




using TIDThr::Synchronized;




namespace TIDSocket {




//
// Constructor
//
DatagramChannel::DatagramChannel()
    throw(SystemException)
{
    _socket = NULL;
}




//
// Destructor
//
DatagramChannel::~DatagramChannel()
    throw(SystemException)
{
    delete _socket;
}




//
// connect()
//
DatagramChannel& DatagramChannel::connect(const SocketAddress& addr)
    throw(ClosedChannelException, AsynchronousCloseException,
          ClosedByInterruptException, IOException)
{
    // Comprueba si esta cerrado
    if (! isOpen())
    {
        throw ClosedChannelException("Channel is closed");
    }

    // Intenta conexion
    try
    {
        Synchronized synchronized(_sync);
        {
            _socket->connect(addr);
            _status |= TID_SOCKET_CHANNEL_STATUS_CONNECTED;
        }
    }
    catch(IllegalArgumentException& e)
    {
       throw IOException(e.what());
    }
    catch(TIDThr::Exception& e)
    {
       throw IOException(e.what());
    }

    return *this;
}




//
// disconnect()
//
DatagramChannel& DatagramChannel::disconnect()
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            _socket->disconnect();
            _status &= ~TID_SOCKET_CHANNEL_STATUS_CONNECTED;
        }
    }
    catch(TIDThr::Exception& e)
    {
       throw IOException(e.what());
    }

    return *this;
}




//
// isConnected()
//
bool DatagramChannel::isConnected()
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
// read()
//
ssize_t DatagramChannel::read(unsigned char* dst, size_t dst_len)
    throw(NotYetConnectedException, IOException)
{
    // Comprueba si esta conectado
    if (! isConnected())
    {
        throw NotYetConnectedException("Channel is not connected");
    }

    ssize_t nread  = 0;
    size_t  length = dst_len;
    try
    {
        DatagramPacket pack(dst, dst_len, length);
        _socket->receive(pack);
        nread = pack.getLength();
    }
    catch (IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
    catch (IllegalBlockingModeException& e)
    {
        throw IOException(e.what());
    }

    return nread;
}




//
// receive()
//
SocketAddress* DatagramChannel::receive(unsigned char* dst, size_t dst_len)
    throw(ClosedChannelException, ClosedByInterruptException,
          AsynchronousCloseException, IOException)
{
    // Comprueba si esta cerrado
    if (! isOpen())
    {
        throw ClosedChannelException("Channel is closed");
    }

    SocketAddress* from = NULL;
    if (isConnected())
    {
        size_t length = dst_len;
        try
        {
            DatagramPacket pack(dst, dst_len, length);
            _socket->receive(pack);
            if (pack.getLength() == -1)
            {
                throw IOException("receive() error");
            }
            else if (isBlocking() || (pack.getLength() > 0))
            {
                const InetAddress* addr = pack.getAddress();
                in_port_t          port = pack.getPort();

                from = new InetSocketAddress(addr, port);
            }
        }
        catch (IllegalArgumentException& e)
        {
            throw IOException(e.what());
        }
        catch (IllegalBlockingModeException& e)
        {
            throw IOException(e.what());
        }
    }

    return from;
}




//
// send()
//
ssize_t DatagramChannel::send(const unsigned char* src, size_t src_len,
                          const SocketAddress& target)
    throw(ClosedChannelException, ClosedByInterruptException,
          AsynchronousCloseException, IOException)
{
    // Comprueba si esta cerrado
    if (! isOpen())
    {
        throw ClosedChannelException("Channel is closed");
    }

    ssize_t nwrite = 0;
    size_t  length = src_len;
    try
    {
        DatagramPacket pack((unsigned char*) src, src_len, length, target);
        _socket->send(pack);
        nwrite = pack.getLength();
    }
    catch (IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
    catch (IllegalBlockingModeException& e)
    {
        throw IOException(e.what());
    }

    return nwrite;
}




//
// validOps()
//
int DatagramChannel::validOps()
    throw()
{
    return ( SelectionKey::OP_READ | SelectionKey::OP_WRITE );
}




//
// write()
//
ssize_t DatagramChannel::write(const unsigned char* src, size_t src_len)
    throw(NotYetConnectedException, IOException)
{
    // Comprueba si esta conectado
    if (! isConnected())
    {
        throw NotYetConnectedException("Channel is not connected");
    }

    ssize_t nwrite = 0;
    size_t  length = src_len;
    try
    {
        InetSocketAddress addr(_socket->getInetAddress(), _socket->getPort());
        DatagramPacket pack((unsigned char*) src, src_len, length, addr);
        _socket->send(pack);
        nwrite = pack.getLength();
    }
    catch (IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
    catch (IllegalBlockingModeException& e)
    {
        throw IOException(e.what());
    }

    return nwrite;
}




//
// implConfigureBlocking()
//
void DatagramChannel::implConfigureBlocking(bool block)
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
void DatagramChannel::implCloseChannel()
    throw(IOException)
{
    _socket->close();
}




//
// implRegisterChannel()
//
SelectionKey& DatagramChannel::implRegisterChannel(Selector& sel, int ops)
    throw()
{
    int fd = (int) _socket->_impl->getFileDescriptor();
    return sel.registerKey(fd, ops, (SelectableChannel*) this);
}




//
// static open()
//
DatagramChannel* DatagramChannel::open()
    throw(IOException)
{
    DatagramChannel* channel = NULL;

    try
    {
        channel = new DatagramChannel;

        // Crea un socket no conectado
        channel->_socket = new DatagramSocket;

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




} //namespace TIDSocket
