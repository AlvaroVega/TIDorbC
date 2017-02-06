///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Inet6Address.h
// Description: This file contains the Inet6Address class methods
// Rel:         01.00
// Created:     Feb, 2008
// Author:      Alvaro Vega García      ( avega@tid.es   )
//
// Revised:
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


#ifndef __TID_SOCKET_INET6_ADDRESS_H__
#define __TID_SOCKET_INET6_ADDRESS_H__


#include <string>
#include <vector>

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::string;
using std::vector;
#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
using std::ostream;
#endif
#endif


#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>


#if defined(__sun)

#include <netdb.h>

#elif defined(__hpux)

extern "C" {
#include <netdb.h>
}

#ifndef _IN_PORT_T
#define _IN_PORT_T
typedef unsigned short in_port_t;
#endif //_IN_PORT_T

#endif

typedef struct in6_addr in6_addr_t;


namespace TIDSocket
{


class Inet6Address: public virtual InetAddress
{
    friend class Socket;
    friend class ServerSocket;
    friend class DatagramSocket;
    friend class MulticastSocket;
    friend class InetSocketAddress;
    friend class PlainSocketImpl;
    friend class SocketImpl;
    friend class InetAddress;

    public:
        // Destructor
        virtual ~Inet6Address()
            throw();

        // Assign operator
        Inet6Address& operator= (const Inet6Address& addr)
            throw();

        // Equals operator
        bool operator== (const Inet4Address& addr)
            throw();

        bool operator== (const Inet6Address& addr)
            throw();

        // Return the raw IP address of this InetAddress object
        virtual const unsigned char* getAddress(size_t& out_len) const
            throw();

        // Return the IP address string in textual presentation
        // (caller must delete returned string)
        virtual char* getHostAddress()
            throw();

        // Utility routine to check if the InetAddress in a wildcard address
        bool isAnyLocalAddress() const
            throw();

        // Utility routine to check if the InetAddress is an IPv4 compatible IPv6 address
        bool isIPv4CompatibleAddress() const
            throw();

        // Utility routine to check if the InetAddress is an link local address
        bool isLinkLocalAddress() const
            throw();

        // Utility routine to check if the InetAddress is a loopback address
        bool isLoopbackAddress() const
            throw();

        // Utility routine to check if the multicast address has global scope
        bool isMCGlobal() const
            throw();

        // Utility routine to check if the multicast address has link scope
        bool isMCLinkLocal() const
            throw();

        // Utility routine to check if the multicast address has node scope
        bool isMCNodeLocal() const
            throw();

        // Utility routine to check if the multicast address has organization
        // scope
        bool isMCOrgLocal() const
            throw();

        // Utility routine to check if the multicast address has site scope
        bool isMCSiteLocal() const
            throw();

        // Utility routine to check if the InetAddress is an IP multicast
        // address
        bool isMulticastAddress() const
            throw();

        // Utility routine to check if the InetAddress is a site local address
        bool isSiteLocalAddress() const
            throw();

        // Utility routine to clone the InetAddress 
        virtual InetAddress* clone() const  
            throw();

    public:
        // Constructor
        Inet6Address()
            throw();

    protected:
        // IP address
        struct in6_addr _ip;

};


} //namespace TIDSocket


ostream& operator<< (ostream& os, const TIDSocket::Inet6Address& inet);


#endif //__TID_SOCKET_INET6_ADDRESS_H__
