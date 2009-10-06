///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_MulticastSocket.C
// Description: This file contains the MulticastSocket class methods
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
#include "PlainSocketImpl.h"




using TIDThr::Synchronized;




namespace TIDSocket
{




//
// Constructor
//
MulticastSocket::MulticastSocket()
    throw(SocketException, SystemException)
    : DatagramSocket(new PlainSocketImpl)
{
    InetSocketAddress bindAddr(PlainSocketImpl::ANY_PORT);
    init(&bindAddr);
}




//
// Constructor
//
MulticastSocket::MulticastSocket(in_port_t port)
    throw(SocketException, SystemException)
    : DatagramSocket(new PlainSocketImpl)
{
    InetSocketAddress bindAddr(port);
    init(&bindAddr);
}




//
// Constructor
//
MulticastSocket::MulticastSocket(const SocketAddress* bindAddr)
    throw(SocketException, SystemException)
    : DatagramSocket(new PlainSocketImpl)
{
    init(bindAddr);
}




//
// Destructor
//
MulticastSocket::~MulticastSocket()
    throw(SystemException)
{
}




//
// Object initialization
//
void MulticastSocket::init(const SocketAddress* bindAddr)
    throw(SocketException)
{
    setReuseAddress(true);

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
// getInterface()
//
InetAddress* MulticastSocket::getInterface()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            size_t size;
            int optID = SocketOptions::_IP_MULTICAST_IF;
            return (InetAddress*) _impl->getOption(optID, size);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getLoopbackMode()
//
bool MulticastSocket::getLoopbackMode()
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_IP_MULTICAST_LOOP;
            return PlainSocketImpl::getBoolOption(_impl, optID);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// getNetworkInterface()
//
NetworkInterface* MulticastSocket::getNetworkInterface()
    throw(SocketException)
{
    InetAddress*      inet  = NULL;
    NetworkInterface* netIf = NULL;

    try
    {
        Synchronized synchronized(_sync);
        {
            inet  = getInterface();
            netIf = NetworkInterface::getByInetAddress(*inet);
            delete inet;
            return netIf;
        }
    }
    catch(TIDThr::Exception& e)
    {
        delete inet;
        delete netIf;
        throw SocketException(e.what());
    }
}




//
// getTimeToLive()
//
int MulticastSocket::getTimeToLive()
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return _impl->getTimeToLive();
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// joinGroup()
//
void MulticastSocket::joinGroup(const InetAddress& mcastaddr)
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            InetSocketAddress sock(&mcastaddr, PlainSocketImpl::ANY_PORT);
            joinGroup(sock, NULL);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// joinGroup()
//
void MulticastSocket::joinGroup(const SocketAddress& mcastaddr,
                                const NetworkInterface* netIf)
    throw(IOException)
{
    NetworkInterface* incoming_netIf = NULL;

    try
    {
        Synchronized synchronized(_sync);
        {
            if (netIf)
                _impl->joinGroup(mcastaddr, *netIf);
            else {
                incoming_netIf = getNetworkInterface();
                _impl->joinGroup(mcastaddr, *incoming_netIf);
                delete incoming_netIf;
            }
        }
    }
    catch(TIDThr::Exception& e)
    {
        delete incoming_netIf;
        throw IOException(e.what());
    }
}




//
// leaveGroup()
//
void MulticastSocket::leaveGroup(const InetAddress& mcastaddr)
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            InetSocketAddress sock(&mcastaddr, PlainSocketImpl::ANY_PORT);
            leaveGroup(sock, NULL);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




//
// leaveGroup()
//
void MulticastSocket::leaveGroup(const SocketAddress& mcastaddr,
                                 const NetworkInterface* netIf)
    throw(IOException)
{
    NetworkInterface* incoming_netIf = NULL;

    try
    {
        Synchronized synchronized(_sync);
        {
            if (netIf)
                _impl->leaveGroup(mcastaddr, *netIf);
            else {
                incoming_netIf = getNetworkInterface();
                _impl->leaveGroup(mcastaddr, *incoming_netIf);
                delete incoming_netIf;
            }
        }
    }
    catch(TIDThr::Exception& e)
    {
        delete incoming_netIf;
        throw IOException(e.what());
    }
}




//
// setInterface()
//
void MulticastSocket::setInterface(const InetAddress& inf)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            size_t size = sizeof(InetAddress);
            int optID = SocketOptions::_IP_MULTICAST_IF;
            _impl->setOption(optID, (const void*) &inf, size);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setLoopbackMode()
//
void MulticastSocket::setLoopbackMode(bool disable)
    throw(SocketException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            int optID = SocketOptions::_IP_MULTICAST_LOOP;
            PlainSocketImpl::setBoolOption(_impl, optID, disable);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw SocketException(e.what());
    }
}




//
// setNetworkInterface()
//
void MulticastSocket::setNetworkInterface(const NetworkInterface& netIf)
    throw(SocketException)
{
    const InetAddressList& addresses = netIf.getInetAddresses();
    setInterface(addresses[0]);
}




//
// setTimeToLive()
//
void MulticastSocket::setTimeToLive(int ttl)
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            _impl->setTimeToLive(ttl);
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




} //namespace TIDSocket
