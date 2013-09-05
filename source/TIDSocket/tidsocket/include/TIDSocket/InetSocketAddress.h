///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InetSocketAddress.h
// Description: This file contains the InetSocketAddress class methods
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


#ifndef __TID_SOCKET_INET_SOCKET_ADDRESS_H__
#define __TID_SOCKET_INET_SOCKET_ADDRESS_H__


#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
using std::ostream;
#endif


namespace TIDSocket
{


class InetSocketAddress : public SocketAddress
{
    public:
        // Create a socket address from an IP address and a port number
        InetSocketAddress(const InetAddress* inaddr, in_port_t port, 
                          bool ipv6 = false)
            throw(IllegalArgumentException);

        // Create a socket address where the IP address is the wildcard address
        // and the port number a specified value
        InetSocketAddress(in_port_t port, 
                          bool ipv6 = false)
            throw(IllegalArgumentException);

        // Create a socket address from a hostname and a port number
        InetSocketAddress(const char* hostname, in_port_t port, 
                          bool ipv6 = false)
            throw(IllegalArgumentException);

        // Destroy the InetSocketAddress
        virtual ~InetSocketAddress()
            throw();

        // Assign operator
        InetSocketAddress& operator= (const InetSocketAddress& inaddr)
            throw();

        // Equals operator
        bool operator== (const InetSocketAddress& inaddr)
            throw();

        // Get the InetAddress
        const InetAddress& getAddress() const
            throw();

        // Get the hostname
        const char* getHostName()
            throw();

        // Get the port number
        in_port_t getPort() const
            throw();

        // Check whether the address has been resolved or not
        bool isUnresolved() const
            throw();

    protected:
        //InetAddress _addr;
        InetAddress* _addr;
        in_port_t    _port;
        bool         _resolved;
        bool         _ipv6;

    private:
        // Object initialization
        void init_addr(const InetAddress* inaddr)
            throw();

        void init_port(in_port_t port)
            throw(IllegalArgumentException);
};


} //namespace TIDSocket


ostream& operator<< (ostream& os, const TIDSocket::InetSocketAddress& sock);


#endif //__TID_SOCKET_INET_SOCKET_ADDRESS_H__
