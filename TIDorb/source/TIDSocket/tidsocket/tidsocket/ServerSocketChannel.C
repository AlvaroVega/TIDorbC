///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_ServerSocketChannel.C
// Description: This file contains the ServerSocketChannel class methods
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

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef __darwin
   #include <sys/ioctl.h>
   #include <CoreServices/CoreServices.h>
#else
   #include <stropts.h>
#endif

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>




namespace TIDSocket
{




//
// Constructor
//
ServerSocketChannel::ServerSocketChannel()
    throw(SystemException)
{
    _socket = NULL;
}




//
// Destructor
//
ServerSocketChannel::~ServerSocketChannel()
    throw(SystemException)
{
    delete _socket;
}




//
// accept()
//
SocketChannel* ServerSocketChannel::accept()
    throw(ClosedChannelException, ClosedByInterruptException,
          AsynchronousCloseException, NotYetBoundException, IOException)
{
    // Comprueba si ya esta asociado a una direccion
    if (! _socket->isBound())
    {
        throw NotYetBoundException("Channel not yet bound");
    }

    // Comprueba si esta cerrado
    if (! isOpen())
    {
        throw ClosedChannelException("Channel is closed");
    }

    SocketChannel* channel = NULL;

    // Comprobacion no-bloqueante de peticiones pendientes
    if (! isBlocking())
    {
        struct timeval timeout = { 0L, 0L };

        int fd = (int) _socket->_impl->getFileDescriptor();
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(fd, &rset);
        if (select(fd+1, &rset, NULL, NULL, &timeout)<=0 || !FD_ISSET(fd,&rset))
        {
            return NULL;
        }
    }

    // Intenta aceptar una conexion
    Socket* new_socket = _socket->accept();
    channel = new SocketChannel;
    if (channel == NULL)
    {
        delete new_socket;
        throw IOException("Cannot create new SocketChannel");
    }
    channel->_socket = new_socket;

    // Fija el estado del canal
    channel->_status |= TID_SOCKET_CHANNEL_STATUS_OPEN;

    // Fija en el socket la referencia a este nuevo canal creado
    channel->_socket->_channel = channel;

    return channel;
}




//
// socket()
//
const ServerSocket* ServerSocketChannel::socket()
    throw()
{
    return (const ServerSocket*) _socket;
}




//
// validOps()
//
int ServerSocketChannel::validOps()
    throw()
{
    return ( SelectionKey::OP_ACCEPT );
}




//
// implConfigureBlocking()
//
void ServerSocketChannel::implConfigureBlocking(bool block)
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
void ServerSocketChannel::implCloseChannel()
    throw(IOException)
{
    _socket->close();
}




//
// implRegisterChannel()
//
SelectionKey& ServerSocketChannel::implRegisterChannel(Selector& sel, int ops)
    throw()
{
    int fd = (int) _socket->_impl->getFileDescriptor();
    return sel.registerKey(fd, ops, (SelectableChannel*) this);
}




//
// static open()
//
ServerSocketChannel* ServerSocketChannel::open()
    throw(IOException)
{
    ServerSocketChannel* channel = NULL;

    try
    {
        channel = new ServerSocketChannel;

        // Crea un socket no conectado
        channel->_socket = new ServerSocket;

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
