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
#include "PlainSocketImpl.h"




using TIDThr::Synchronized;




namespace TIDSocket
{




//
// Constructor
//
MulticastSocket::MulticastSocket(const char* interface, bool ipv6)
    throw(SocketException, SystemException)
    : DatagramSocket(new PlainSocketImpl, ipv6)
{
    InetSocketAddress bindAddr(PlainSocketImpl::ANY_PORT, _ipv6);
    init(&bindAddr, interface);
}




//
// Constructor
//
MulticastSocket::MulticastSocket(in_port_t port, const char* interface, 
                                 bool ipv6)
    throw(SocketException, SystemException)
    : DatagramSocket(new PlainSocketImpl, ipv6)
{
    InetSocketAddress bindAddr(port, ipv6);
    init(&bindAddr, interface);
}




//
// Constructor
//
MulticastSocket::MulticastSocket(const SocketAddress* bindAddr,
                                 const char* interface, bool ipv6)
    throw(SocketException, SystemException)
    : DatagramSocket(new PlainSocketImpl, ipv6)
{
    init(bindAddr,interface);
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
void MulticastSocket::init(const SocketAddress* bindAddr,const char* interface)
    throw(SocketException)
{
    setReuseAddress(true);

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
            //int optID = SocketOptions::_IP_MULTICAST_IF;
            int optID;
            if (_ipv6)
              optID = SocketOptions::_IPV6_MULTICAST_IF;
            else
              optID = SocketOptions::_IP_MULTICAST_IF;
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
            //mcpg
            //int optID = SocketOptions::_IP_MULTICAST_IF;
            int optID;
            if (_ipv6)
              optID = SocketOptions::_IPV6_MULTICAST_IF;
            else
              optID = SocketOptions::_IP_MULTICAST_IF;
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
        	  //mcpg
            //int optID = SocketOptions::_IP_MULTICAST_LOOP;
            int optID;
            if (_ipv6)
              optID = SocketOptions::_IPV6_MULTICAST_LOOP;
            else
              optID = SocketOptions::_IP_MULTICAST_LOOP;
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
    //setInterface(addresses[0]);
    setInterface(*(addresses[0]));
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
