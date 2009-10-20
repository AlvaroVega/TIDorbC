///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InetAddress.h
// Description: This file contains the InetAddress class methods
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


#ifndef __TID_SOCKET_INET_ADDRESS_H__
#define __TID_SOCKET_INET_ADDRESS_H__


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


namespace TIDSocket
{


class InetAddress;
typedef vector<InetAddress> InetAddressList;


class InetAddress
{
    friend class Socket;
    friend class ServerSocket;
    friend class DatagramSocket;
    friend class MulticastSocket;
    friend class InetSocketAddress;
    friend class PlainSocketImpl;
    friend class SocketImpl;

    public:
        // Destructor
        virtual ~InetAddress()
            throw();

        // Assign operator
        InetAddress& operator= (const InetAddress& addr)
            throw();

        // Equals operator
        bool operator== (const InetAddress& addr)
            throw();

        // Return the raw IP address of this InetAddress object
        const unsigned char* getAddress(size_t& out_len) const
            throw();

        // Get the host name for this IP address
        const char* getHostName()
            throw();

        // Return the IP address string in textual presentation
        // (caller must delete returned string)
        char* getHostAddress()
            throw();

        // Get the fully qualified domain name for this IP address
        // (caller must delete returned string)
        char* getCanonicalHostName()
            throw();

        // Utility routine to check if the InetAddress in a wildcard address
        bool isAnyLocalAddress() const
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

    public:
        // Return the local host
        // (caller must delete InetAddress object)
        static InetAddress* getLocalHost()
            throw(UnknownHostException);

        // Determine the IP address of a host, given the host's name
        // (caller must delete InetAddress object)
        static InetAddress* getByName(const char* host)
            throw(UnknownHostException);

        // Given the name of a host, return an array of its IP addresses, based
        // on the configured name service on the system
        // (caller must delete returned vector)
        static InetAddressList* getAllByName(const char* host)
            throw(UnknownHostException);

        // Return an InetAddress object given the raw IP address
        // (caller must delete InetAddress object)
        static InetAddress* getByAddress(const unsigned char* addr, size_t len)
            throw(UnknownHostException);

        // Create an InetAddress based on the provided host name and IP address
        // No name service is checked for the validity of the address
        // (caller must delete InetAddress object)
        static InetAddress* getByAddress(const char* host,
                                         const unsigned char* addr, size_t len)
            throw(UnknownHostException);

    private:
        // Constructor
        InetAddress()
            throw();

        // Duplicate string
        char* duplicate(const string& s)
            throw();

    protected:
        // IP address
        struct in_addr _ip;

        // Host's name, or "" if unspecified
        string _hostname;
};


} //namespace TIDSocket


ostream& operator<< (ostream& os, const TIDSocket::InetAddress& inet);


#endif //__TID_SOCKET_INET_ADDRESS_H__
