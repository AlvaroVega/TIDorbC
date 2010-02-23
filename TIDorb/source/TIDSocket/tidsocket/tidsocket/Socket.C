///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Socket.C
// Description: This file contains the Socket class methods
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
#include <sys/socket.h>
#include <string.h>




using TIDThr::Synchronized;




namespace TIDSocket
{




//
// Socket class static members
//
SocketImplFactory* Socket::_factory = NULL;




//
// Constructor
//
Socket::Socket()
    throw(IOException, SystemException)
{
    // New socket implementation object
    _impl    = new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_STREAM socket
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;
}




//
// Constructor
//
Socket::Socket(const InetAddress& address, in_port_t port)
    throw(IOException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_STREAM socket
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Connect socket
    _impl->connect(address, port);
    setConnected();
}




//
// Constructor
//
Socket::Socket(const InetAddress& address,   in_port_t port,
               const InetAddress& localAddr, in_port_t localPort)
    throw(IOException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_STREAM socket
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Bind socket to pair (localAddr,localPort)
    _impl->bind(localAddr, localPort);
    _status |= TID_SOCKET_STATUS_BOUND;

    // Connect socket to remote host
    _impl->connect(address, port);
    setConnected();
}




//
// Constructor
//
Socket::Socket(const char* host, in_port_t port)
    throw(UnknownHostException, IOException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_STREAM socket
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Connect socket
    _impl->connect(host, port);
    setConnected();
}




//
// Constructor
//
Socket::Socket(const char* host,             in_port_t port,
               const InetAddress& localAddr, in_port_t localPort)
    throw(IOException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createSocketImpl() : new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_STREAM socket
    _impl->create(true);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Bind socket to pair (localAddr,localPort)
    _impl->bind(localAddr, localPort);
    _status |= TID_SOCKET_STATUS_BOUND;

    // Connect socket
    _impl->connect(host, port);
    setConnected();
}




//
// Constructor
//
Socket::Socket(const SocketImpl* impl)
    throw(SocketException, SystemException)
{
    // Use implementation object reference
    _impl    = (SocketImpl*) impl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;
}




//
// Destructor
//
Socket::~Socket()
    throw(SystemException)
{
    // Close this socket
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

    // Release resources
    delete _impl;
}




//
// static setSocketImplFactory()
//
void Socket::setSocketImplFactory(const SocketImplFactory& fac)
    throw(SocketException)
{
    if (_factory)
    {
        throw SocketException("SocketImpl factory already defined");
    }
    _factory = (SocketImplFactory*) &fac;
}




//
// bind()
//
void Socket::bind(const SocketAddress* bindpoint)
    throw(IOException, IllegalArgumentException)
{
    // InetSocketAddress a la que nos asociamos (cualquier IP, cualquier puerto)
    InetSocketAddress addr(PlainSocketImpl::ANY_PORT);
    InetSocketAddress* addrptr = &addr;

    // Comprueba si bindpoint es referencia a un objeto InetSocketAddress; en
    // caso afirmativo, se asocia a dicha direccion
    if (bindpoint)
    {
        addrptr = (InetSocketAddress*)
                  dynamic_cast<const InetSocketAddress*>(bindpoint);
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
void Socket::close()
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            // Cierra el socket y anula todos los flags de estado excepto CLOSED
            _impl->close();
            _status = TID_SOCKET_STATUS_CLOSED;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// connect()
//
void Socket::connect(const SocketAddress& endpoint)
    throw(IllegalBlockingModeException, IllegalArgumentException, IOException)
{
    // Comprueba si endpoint es referencia a un objeto InetSocketAddress
    const InetSocketAddress* addrptr =
        dynamic_cast<const InetSocketAddress*>(&endpoint);

    if (addrptr == NULL)
    {
        throw IllegalArgumentException("Invalid SocketAddress");
    }

    try
    {
        Synchronized synchronized(_sync);
        {
            // Comprueba el modo de bloqueo del canal asociado, si existe
            if (_channel && _channel->isBlocking()==false)
            {
                throw IllegalBlockingModeException(
                          "SocketChannel in non-blocking mode");
            }

            // Invoca a la operacion connect de SocketImpl
            _impl->connect(addrptr->getAddress(), addrptr->getPort());
            setConnected();
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// connect()
//
void Socket::connect(const SocketAddress& endpoint, time_t timeout)
    throw(IllegalBlockingModeException, IllegalArgumentException, IOException)
{
    // Comprueba si endpoint es referencia a un objeto InetSocketAddress
    const InetSocketAddress* addrptr =
        dynamic_cast<const InetSocketAddress*>(&endpoint);

    if (addrptr == NULL)
    {
        throw IllegalArgumentException("Invalid SocketAddress");
    }

    try
    {
        Synchronized synchronized(_sync);
        {
            // Comprueba el modo de bloqueo del canal asociado, si existe
            if (_channel && _channel->isBlocking()==false)
            {
                throw IllegalBlockingModeException(
                          "SocketChannel in non-blocking mode");
            }

            // Invoca a la operacion connect de SocketImpl
            _impl->connect(endpoint, timeout);
            setConnected();
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
const SocketChannel* Socket::getChannel()
    throw()
{
    return (const SocketChannel*) _channel;
}




//
// getInetAddress()
//
const InetAddress* Socket::getInetAddress()
    throw()
{
    InetAddress* inet = NULL;

    if (isConnected())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                inet = (InetAddress*) &(_impl->getInetAddress());
            }
        }
        catch(...)
        {
        }
    }

    return (const InetAddress*) inet;
}




//
// getInputStream()
//
InputStream* Socket::getInputStream()
    throw(IOException)
{
    if (isConnected()==false)
    {
        throw IOException("Socket not connected");
    }

    InputStream* stream = NULL;

    try
    {
        Synchronized synchronized(_sync);
        {
             stream = _impl->getInputStream();

             // If this socket has an associated channel then the resulting
             // input stream delegates all of its operations to the channel.
             // If the channel is in non-blocking mode then the input stream's
             // read operations will throw an IllegalBlockingModeException
             stream->_channel = _channel;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return stream;
}




//
// getKeepAlive()
//
bool Socket::getKeepAlive()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_KEEPALIVE;
            return PlainSocketImpl::getBoolOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getLocalAddress()
//
InetAddress* Socket::getLocalAddress()
    throw()
{
    InetAddress*       inet = NULL;
    InetSocketAddress* sock = NULL;

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

    delete sock;
    return inet;
}




//
// getLocalPort()
//
int Socket::getLocalPort()
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
SocketAddress* Socket::getLocalSocketAddress()
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
// getOOBInline()
//
bool Socket::getOOBInline()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_OOBINLINE;
            return PlainSocketImpl::getBoolOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getOutputStream()
//
OutputStream* Socket::getOutputStream()
    throw(IOException)
{
    if (! isConnected())
    {
        throw IOException("Socket not connected");
    }

    OutputStream* stream = NULL;

    try
    {
        Synchronized synchronized(_sync);
        {
            stream = _impl->getOutputStream();

            // If this socket has an associated channel then the resulting
            // output stream delegates all of its operations to the channel.
            // If the channel is in non-blocking mode then the output stream's
            // write operations will throw an IllegalBlockingModeException
            stream->_channel = _channel;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return stream;
}




//
// getPort()
//
int Socket::getPort()
    throw()
{
    int port = 0;

    if (isConnected())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                port = (int) _impl->getPort();
            }
        }
        catch(...)
        {
        }
    }

    return port;
}




//
// getReceiveBufferSize()
//
size_t Socket::getReceiveBufferSize()
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
// getRemoteSocketAddress
//
SocketAddress* Socket::getRemoteSocketAddress()
    throw()
{
    SocketAddress* sock = NULL;

    if (isConnected())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                const InetAddress& addr = _impl->getInetAddress();
                sock = new InetSocketAddress(&addr, _impl->getPort());
            }
        }
        catch(...)
        {
        }
    }

    return sock;
}




//
// getReuseAddress()
//
bool Socket::getReuseAddress()
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
// getSendBufferSize()
//
size_t Socket::getSendBufferSize()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_SNDBUF;
            return PlainSocketImpl::getSizeOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getSoLinger()
//
time_t Socket::getSoLinger()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_LINGER;
            return PlainSocketImpl::getTimeOption(_impl, optID);
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
time_t Socket::getSoTimeout()
    throw (SocketException)
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
// getTcpNoDelay()
//
bool Socket::getTcpNoDelay()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_TCP_NODELAY;
            return PlainSocketImpl::getBoolOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getTrafficClass()
//
int Socket::getTrafficClass()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_IP_TOS;
            return PlainSocketImpl::getIntOption(_impl, optID);
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
bool Socket::isBound()
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
bool Socket::isClosed()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return ((! (_status & TID_SOCKET_STATUS_CREATED)) ||
                    (_status & TID_SOCKET_STATUS_CLOSED));
        }
    }
    catch(...)
    {
    }

    return false;
}




//
// isConnected()
//
bool Socket::isConnected()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_STATUS_CONNECTED);
        }
    }
    catch(...)
    {
    }

    return false;
}




//
// isInputShutdown()
//
bool Socket::isInputShutdown()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_STATUS_INPUT_SHUTDOWN);
        }
    }
    catch(...)
    {
    }

    return false;
}




//
// isOutputShutdown()
//
bool Socket::isOutputShutdown()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_STATUS_OUTPUT_SHUTDOWN);
        }
    }
    catch(...)
    {
    }

    return false;
}




//
// sendUrgentData()
//
void Socket::sendUrgentData(int data)
    throw(IOException)
{
    _impl->sendUrgentData(data);
}




//
// setKeepAlive()
//
void Socket::setKeepAlive(bool on)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_KEEPALIVE;
            PlainSocketImpl::setBoolOption(_impl, optID, on);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setOOBInline()
//
void Socket::setOOBInline(bool on)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_OOBINLINE;
            PlainSocketImpl::setBoolOption(_impl, optID, on);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setReceiveBufferSize()
//
void Socket::setReceiveBufferSize(size_t size)
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
void Socket::setReuseAddress(bool on)
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
// setSendBufferSize()
//
void Socket::setSendBufferSize(size_t size)
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
            int optID = SocketOptions::_SO_SNDBUF;
            PlainSocketImpl::setSizeOption(_impl, optID, size);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setSoLinger()
//
void Socket::setSoLinger(bool on, time_t linger_time)
    throw(SocketException, IllegalArgumentException)
{
    if (on && (linger_time <= 0))
    {
        throw IllegalArgumentException("Invalid linger time");
    }

    time_t seconds = (on) ? linger_time : -1;

    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_LINGER;
            PlainSocketImpl::setTimeOption(_impl, optID, seconds);
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
void Socket::setSoTimeout(time_t timeout)
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
// setTcpNoDelay()
//
void Socket::setTcpNoDelay(bool on)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_TCP_NODELAY;
            PlainSocketImpl::setBoolOption(_impl, optID, on);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setTrafficClass()
//
void Socket::setTrafficClass(int tc)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_IP_TOS;
            PlainSocketImpl::setIntOption(_impl, optID, tc);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// shutdownInput()
//
void Socket::shutdownInput()
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            _impl->shutdownInput();
            _status |= TID_SOCKET_STATUS_INPUT_SHUTDOWN;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// shutdownOutput()
//
void Socket::shutdownOutput()
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            _impl->shutdownOutput();
            _status |= TID_SOCKET_STATUS_OUTPUT_SHUTDOWN;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setConnected()
//
void Socket::setConnected()
    throw()
{
    // Se marca como "created" por si es un socket creado desde un accept()
    _status |= TID_SOCKET_STATUS_CREATED;

    // Se marca como "bound" porque connect() ejecuta un bind() si es necesario
    _status |= TID_SOCKET_STATUS_BOUND;
    _status |= TID_SOCKET_STATUS_CONNECTED;
}




} //namespace TIDSocket
