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
DatagramSocket::DatagramSocket()
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_DGRAM socket
    _impl->create();
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(PlainSocketImpl::ANY_PORT);
        bind((SocketAddress*) &inet);
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(in_port_t port)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_DGRAM socket
    _impl->create();
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(port);
        bind((SocketAddress*) &inet);
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(in_port_t port, const InetAddress& localAddr)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_DGRAM socket
    _impl->create();
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    try
    {
        InetSocketAddress inet(&localAddr, port);
        bind((SocketAddress*) &inet);
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// Constructor
//
DatagramSocket::DatagramSocket(const SocketAddress* bindAddr)
    throw(SocketException, SystemException)
{
    // New implementation object from factory, if defined
    _impl    = (_factory) ? _factory->createDatagramSocketImpl()
                          :  new PlainSocketImpl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_DGRAM socket
    _impl->create();
    _status |= TID_SOCKET_STATUS_CREATED;

    // Asocia el socket y lo pone en escucha
    if (bindAddr)
    {
        try
        {
            bind(bindAddr);
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
DatagramSocket::DatagramSocket(const DatagramSocketImpl* impl)
    throw(SocketException, SystemException)
{
    // Use implementation object reference
    _impl    = (DatagramSocketImpl*) impl;
    _status  = TID_SOCKET_STATUS_UNSPECIFIED;
    _channel = NULL;

    // Create a SOCK_DGRAM socket
    _impl->create();
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
void DatagramSocket::bind(const SocketAddress* addr)
    throw(SocketException, IllegalArgumentException)
{
    // InetSocketAddress a la que nos asociamos (cualquier IP, cualquier puerto)
    InetSocketAddress bindaddr(PlainSocketImpl::ANY_PORT);
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
            _impl->bind(bindaddrptr->getPort(), bindaddrptr->getAddress());
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
void DatagramSocket::connect(const InetAddress& address, in_port_t port)
    throw(SocketException, IllegalArgumentException)
{
    InetSocketAddress sock(&address, port);
    connect(sock);
}




//
// connect()
//
void DatagramSocket::connect(const SocketAddress& addr)
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
            _remoteaddr = addrptr->getAddress();
            _remoteport = addrptr->getPort();
            _impl->connect(_remoteaddr, _remoteport);
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
                inet = &_remoteaddr;
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
                sock = new InetSocketAddress(&_remoteaddr, _remoteport);
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
void DatagramSocket::send(DatagramPacket& p)
    throw(IOException, PortUnreachableException, IllegalBlockingModeException)
{
    // Comprueba el modo de bloqueo del canal asociado, si existe
    if (_channel && ! _channel->isBlocking())
    {
         throw IllegalBlockingModeException(
                   "DatagramChannel in non-blocking mode");
    }

    // Invoca a la operacion de DatagramSocketImpl
    _impl->send(p);
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
