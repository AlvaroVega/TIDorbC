///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_ServerSocket.C
// Description: This file contains the ServerSocket class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
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
#include "SocketStatus.h"
#include "PlainSocketImpl.h"
#include <sys/types.h>
#include <string.h>




using TIDThr::Synchronized;




namespace TIDSocket
{




//
// Miembros estaticos de la clase ServerSocket
//
SocketImplFactory* ServerSocket::_factory = NULL;




//
// Constructor
//
ServerSocket::ServerSocket()
    throw(IOException, SystemException)
{
    // Nuevo objeto socket
    _impl    = new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Crea un SOCK_STREAM
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;
}




//
// Constructor
//
ServerSocket::ServerSocket(in_port_t port)
    throw(IOException, SystemException)
{
    // Nuevo objeto socket utilizando la factoria, si esta definida
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Crea un SOCK_STREAM
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(port);
        bind((SocketAddress*) &inet);
    }
    catch(IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
}




//
// Constructor
//
ServerSocket::ServerSocket(in_port_t port, int backlog)
    throw(IOException, SystemException)
{
    // Nuevo objeto socket utilizando la factoria, si esta definida
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Crea un SOCK_STREAM
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(port);
        bind((SocketAddress*) &inet, backlog);
    }
    catch(IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
}




//
// Constructor
//
ServerSocket::ServerSocket(in_port_t port, int backlog, const InetAddress* addr)
    throw(IOException, SystemException)
{
    // Nuevo objeto socket utilizando la factoria, si esta definida
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Crea un SOCK_STREAM
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(port);
        if (addr)
        {
            inet = InetSocketAddress(addr, port);
        }
        bind((SocketAddress*) &inet, backlog);
    }
    catch(IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
}




//
// Destructor
//
ServerSocket::~ServerSocket()
    throw(SystemException)
{
    // Cierra el socket
    if (! isClosed())
    {
        try
        {
            close();
        }
        catch(...)
        {
        }
    }

    // Libera recursos
    delete _impl;
}




//
// static setSocketImplFactory()
//
void ServerSocket::setSocketImplFactory(const SocketImplFactory& fac)
    throw(SocketException)
{
    if (_factory)
    {
        throw SocketException("SocketImpl factory already defined");
    }
    _factory = (SocketImplFactory*) &fac;
}




//
// accept()
//
Socket* ServerSocket::accept()
    throw(IOException, IllegalBlockingModeException)
{
    Socket* s = new Socket(new PlainSocketImpl);
    try
    {
        implAccept(s);
    }
    catch(...)
    {
        delete s;
        throw;
    }

    return s;
}




//
// bind()
//
void ServerSocket::bind(const SocketAddress* endpoint)
    throw(IOException, IllegalArgumentException)
{
    bind(endpoint, PlainSocketImpl::DEFAULT_BACKLOG);
}




//
// bind()
//
void ServerSocket::bind(const SocketAddress* endpoint, int backlog)
    throw(IOException, IllegalArgumentException)
{
    // InetSocketAddress a la que nos asociamos (cualquier IP, cualquier puerto)
    InetSocketAddress addr(PlainSocketImpl::ANY_PORT);
    InetSocketAddress* addrptr = &addr;

    // Comprueba si endpoint es referencia a un objeto InetSocketAddress; en
    // caso afirmativo, se asocia a dicha direccion
    if (endpoint)
    {
        addrptr = (InetSocketAddress*)
                  dynamic_cast<const InetSocketAddress*>(endpoint);
        if (addrptr == NULL)
        {
            throw IllegalArgumentException("Invalid SocketAddress");
        }
    }

    try
    {
        Synchronized synchronized(_sync);
        {
            // Asocia el socket a la direccion InetSocketAddress
            _impl->bind(addrptr->getAddress(), addrptr->getPort());
            _status |= TID_SOCKET_STATUS_BOUND;

            // Pone el socket en escucha
            _impl->listen(backlog);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// close()
//
void ServerSocket::close()
    throw(IOException)
{
    try
    {
        // Cierra el socket
        _impl->close();

        Synchronized synchronized(_sync);
        {
            // Anula todos los flags de estado excepto CLOSED
            _status = TID_SOCKET_STATUS_CLOSED;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// getChannel()
//
const ServerSocketChannel* ServerSocket::getChannel()
    throw()
{
    return (const ServerSocketChannel*) _channel;
}




//
// getInetAddress()
//
InetAddress* ServerSocket::getInetAddress()
    throw()
{
    InetAddress*       inet = NULL;
    InetSocketAddress* sock = NULL;

    if (isBound())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                size_t size;
                int optID = SocketOptions::_SO_BINDADDR;
                sock = (InetSocketAddress*) _impl->getOption(optID, size);

                const InetAddress& addr = sock->getAddress();
                unsigned char*     data = (unsigned char*) addr.getAddress(size);

                inet = InetAddress::getByAddress(data, size);
            }
        }
        catch(...)
        {
        }
    }

    delete sock;
    return inet;
}




//
// getLocalPort()
//
int ServerSocket::getLocalPort()
    throw()
{
    int port = -1;

    if (isBound())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                port = (int) _impl->getLocalPort();
            }
        }
        catch(...)
        {
        }
    }

    return port;
}




//
// getLocalSocketAddress()
//
SocketAddress* ServerSocket::getLocalSocketAddress()
    throw()
{
    SocketAddress* sock = NULL;

    if (isBound())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                size_t size;
                int optID = SocketOptions::_SO_BINDADDR;
                sock = (SocketAddress*) _impl->getOption(optID, size);
            }
        }
        catch(...)
        {
        }
    }

    return sock;
}




//
// getReceiveBufferSize()
//
size_t ServerSocket::getReceiveBufferSize()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_RCVBUF;
            return PlainSocketImpl::getSizeOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getReuseAddress()
//
bool ServerSocket::getReuseAddress()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_REUSEADDR;
            return PlainSocketImpl::getBoolOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getSoTimeout()
//
time_t ServerSocket::getSoTimeout()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_TIMEOUT;
            return PlainSocketImpl::getTimeOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// isBound()
//
bool ServerSocket::isBound()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_STATUS_BOUND);
        }
    }
    catch(...)
    {
    }

    return false;
}




//
// isClosed()
//
bool ServerSocket::isClosed()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_STATUS_CLOSED);
        }
    }
    catch(...)
    {
    }

    return false;
}




//
// setReceiveBufferSize()
//
void ServerSocket::setReceiveBufferSize(size_t size)
    throw(SocketException, IllegalArgumentException)
{
    if (size <= 0)
    {
        throw IllegalArgumentException("Invalid buffer size");
    }

    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_RCVBUF;
            PlainSocketImpl::setSizeOption(_impl, optID, size);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setReuseAddress()
//
void ServerSocket::setReuseAddress(bool on)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_REUSEADDR;
            PlainSocketImpl::setBoolOption(_impl, optID, on);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setSoTimeout()
//
void ServerSocket::setSoTimeout(time_t timeout)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_TIMEOUT;
            PlainSocketImpl::setTimeOption(_impl, optID, timeout);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// implAccept()
//
void ServerSocket::implAccept(Socket* s)
    throw(IOException, IllegalBlockingModeException)
{
    // Comprueba el modo de bloqueo del canal asociado, si existe
    if (_channel && ! _channel->isBlocking())
    {
        throw IllegalBlockingModeException(
                  "ServerSocketChannel in non-blocking mode");
    }

    // Acepta la conexion
    _impl->accept(s->_impl);
    s->setConnected();
}




} //namespace TIDSocket
