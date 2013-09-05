///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramSocket.C
// Description: This file contains the DatagramSocket class methods
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
// Static members
//
DatagramSocketImplFactory* DatagramSocket::_factory = NULL;




//
// Constructor
//
DatagramSocket::DatagramSocket(const char* interface, bool ipv6)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;
    _ipv6    = ipv6;

    // Create a SOCK_DGRAM socket
    _impl->create(_ipv6);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(PlainSocketImpl::ANY_PORT, _ipv6);
        bind((SocketAddress*) &inet, interface);
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(in_port_t port, const char* interface, bool ipv6)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;
    _ipv6    = ipv6;

    // Create a SOCK_DGRAM socket
    _impl->create(ipv6);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(port, _ipv6);
        bind((SocketAddress*) &inet,interface);
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(in_port_t port, const InetAddress& localAddr, 
                               const char* interface, bool ipv6)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;
    _ipv6    = ipv6;

    // Create a SOCK_DGRAM socket
    _impl->create(_ipv6);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(&localAddr, port, _ipv6);
        bind((SocketAddress*) &inet,interface);
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(const SocketAddress* bindAddr, 
                               const char* interface, bool ipv6)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;
    _ipv6    = ipv6;

    // Create a SOCK_DGRAM socket
    _impl->create(_ipv6);
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    if (bindAddr)
    {
        try
        {
            bind(bindAddr,interface);
        }
        catch(IllegalArgumentException& e)
        {
            throw SocketException(e.what());
        }
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(const DatagramSocketImpl* impl, bool ipv6)
    throw(SocketException, SystemException)
{
    // Use implementation object reference
    _impl    = (DatagramSocketImpl*) impl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;
    _ipv6    = ipv6;

    // Create a SOCK_DGRAM socket
    _impl->create(_ipv6);
    _status |= TID_SOCKET_STATUS_CREATED;
}




//
// Destructor
//
DatagramSocket::~DatagramSocket()
    throw(SystemException)
{
    // Close this socket
    if (! isClosed())
    {
        try
        {
            close();
        }
        catch(IOException& e)
        {
            delete _impl;
            throw SystemException(e.what());
        }
    }

    // Release resources
    delete _impl;
}




//
// static setDatagramSocketImplFactory()
//
void DatagramSocket::setDatagramSocketImplFactory(
    const DatagramSocketImplFactory& fac)
    throw(SocketException)
{
    if (_factory)
    {
        throw SocketException("DatagramSocketImpl factory already defined");
    }
    _factory = (DatagramSocketImplFactory*) &fac;
}




//
// bind()
//
void DatagramSocket::bind(const SocketAddress* addr,const char* interface)
    throw(SocketException, IllegalArgumentException)
{
	  // InetSocketAddress a la que nos asociamos (cualquier IP, cualquier puerto)
    InetSocketAddress bindaddr(PlainSocketImpl::ANY_PORT, _ipv6);
    InetSocketAddress* bindaddrptr = &bindaddr;

    // Comprueba si addr es referencia a un objeto InetSocketAddress; en
    // caso afirmativo, se asocia a dicha direccion
    if (addr)
    {
        bindaddrptr = (InetSocketAddress*)
                      dynamic_cast<const InetSocketAddress*>(addr);
        if (bindaddrptr == NULL)
        {
            throw IllegalArgumentException("Invalid SocketAddress");
        }
    }

    try
    {
        Synchronized synchronized(_sync);
        {
            // Asocia el socket a la direccion InetSocketAddress
            _impl->bind(bindaddrptr->getPort(), bindaddrptr->getAddress(),interface);
            _status |= TID_SOCKET_STATUS_BOUND;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// close()
//
void DatagramSocket::close()
    throw()
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
    catch(...)
    {
    }
}




//
// connect()
//
void DatagramSocket::connect(const InetAddress& address, in_port_t port,const char* interface)
    throw(SocketException, IllegalArgumentException)
{
    InetSocketAddress sock(&address, port);
    connect(sock,interface);
}




//
// connect()
//
void DatagramSocket::connect(const SocketAddress& addr,const char* interface)
    throw(SocketException, IllegalArgumentException)
{
    // Comprueba si addr es referencia a un objeto InetSocketAddress
    const InetSocketAddress* addrptr =
        dynamic_cast<const InetSocketAddress*>(&addr);

    if (addrptr == NULL)
    {
        throw IllegalArgumentException("Invalid SocketAddress");
    }

    try
    {
        Synchronized synchronized(_sync);
        {
        	//_remoteaddr = addrptr->getAddress();
            if (_remoteaddr)
              delete _remoteaddr;
            _remoteaddr = addrptr->getAddress().clone();
            _remoteport = addrptr->getPort();
            //_impl->connect(_remoteaddr, _remoteport);
            _impl->connect(*_remoteaddr, _remoteport,interface);
            _status |= TID_SOCKET_STATUS_CONNECTED;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// disconnect()
//
void DatagramSocket::disconnect()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            _impl->disconnect();
            _status &= ~TID_SOCKET_STATUS_CONNECTED;
        }
    }
    catch(...)
    {
    }
}




//
// getBroadcast()
//
bool DatagramSocket::getBroadcast()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_BROADCAST;
            return PlainSocketImpl::getBoolOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getChannel()
//
const DatagramChannel* DatagramSocket::getChannel()
    throw()
{
    return (const DatagramChannel*) _channel;
}




//
// getInetAddress()
//
const InetAddress* DatagramSocket::getInetAddress()
    throw()
{
    InetAddress* inet = NULL;

    if (isConnected())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                inet = &(*_remoteaddr);
            }
        }
        catch(...)
        {
        }
    }

    return (const InetAddress*) inet;
}




//
// getLocalAddress()
//
InetAddress* DatagramSocket::getLocalAddress()
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
in_port_t DatagramSocket::getLocalPort()
    throw()
{
    in_port_t port = PlainSocketImpl::ANY_PORT;

    try
    {
        Synchronized synchronized(_sync);
        {
            port = _impl->getLocalPort();
        }
    }
    catch(...)
    {
    }

    return port;
}




//
// getLocalSocketAddress()
//
SocketAddress* DatagramSocket::getLocalSocketAddress()
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
// getPort()
//
int DatagramSocket::getPort()
    throw()
{
    int port = -1;

    if (isConnected())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                port = _remoteport;
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
size_t DatagramSocket::getReceiveBufferSize()
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
// getRemoteSocketAddress()
//
SocketAddress* DatagramSocket::getRemoteSocketAddress()
    throw()
{
    SocketAddress* sock = NULL;

    if (isConnected())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                sock = new InetSocketAddress(&(*_remoteaddr), _remoteport);
            }
        }
        catch(TIDThr::Exception& e)
        {
        }
    }

    return sock;
}




//
// getReuseAddress()
//
bool DatagramSocket::getReuseAddress()
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
size_t DatagramSocket::getSendBufferSize()
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
// getSoTimeout()
//
time_t DatagramSocket::getSoTimeout()
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
// getTrafficClass()
//
int DatagramSocket::getTrafficClass()
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
bool DatagramSocket::isBound()
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
bool DatagramSocket::isClosed()
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
// isConnected()
//
bool DatagramSocket::isConnected()
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
// receive()
//
void DatagramSocket::receive(DatagramPacket& p)
    throw(IOException, SocketTimeoutException,
          PortUnreachableException, IllegalBlockingModeException)
{
    // Comprueba el modo de bloqueo del canal asociado, si existe
    if (_channel && ! _channel->isBlocking())
    {
         throw IllegalBlockingModeException(
                   "DatagramChannel in non-blocking mode");
    }

    // Invoca a la operacion de DatagramSocketImpl
    _impl->receive(p);
}




//
// send()
//
void DatagramSocket::send(DatagramPacket& p,const char* interface)
    throw(IOException, PortUnreachableException, IllegalBlockingModeException)
{
	  // Comprueba el modo de bloqueo del canal asociado, si existe
    if (_channel && ! _channel->isBlocking())
    {
         throw IllegalBlockingModeException(
                   "DatagramChannel in non-blocking mode");
    }

    // Invoca a la operacion de DatagramSocketImpl
    _impl->send(p,interface);
}




//
// setBroadcast()
//
void DatagramSocket::setBroadcast(bool on)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_SO_BROADCAST;
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
void DatagramSocket::setReceiveBufferSize(size_t size)
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
void DatagramSocket::setReuseAddress(bool on)
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
void DatagramSocket::setSendBufferSize(size_t size)
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
// setSoTimeout()
//
void DatagramSocket::setSoTimeout(time_t timeout)
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
// setTrafficClass()
//
void DatagramSocket::setTrafficClass(int tc)
    throw(SocketException, IllegalArgumentException)
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




} //namespace TIDSocket
