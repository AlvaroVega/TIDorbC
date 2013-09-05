///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_NetworkInterface.h
// Description: This file contains the NetworkInterface class methods
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


#ifndef __TID_SOCKET_NETWORK_INTERFACE_H__
#define __TID_SOCKET_NETWORK_INTERFACE_H__


#include <vector>

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <iostream>
#else
#include <iostream.h>
#endif


#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::vector;
#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
using std::ostream;
#endif
#endif


namespace TIDSocket
{


class NetworkInterface;
typedef vector<NetworkInterface> NetworkInterfaceList;


class NetworkInterface
{
    public:
        // Destructor
        ~NetworkInterface()
            throw();

        // Assign operator
        NetworkInterface& operator= (const NetworkInterface& netIf)
            throw();

        // Equals operator
        bool operator== (const NetworkInterface& netIf)
            throw();

        // Get the display name of this network interface
        // (caller must delete returned string)
        char* getDisplayName()
            throw();

        // Convenience method to return a list with all or a subset of the
        // InetAddresses bound to this network interface
        const InetAddressList& getInetAddresses() const
            throw();

        // Get the name of this network interface
        // (caller must delete returned string)
        char* getName()
            throw();

    public:
        // Convenience method to search for a network interface that has the
        // specified Internet Protocol (IP) address bound to it
        // (caller must delete returned object)
        static NetworkInterface* getByInetAddress(const InetAddress& addr)
            throw(SocketException);

        // Search for the network interface with the specified name
        // (caller must delete returned object)
        static NetworkInterface* getByName(const char* name)
            throw(SocketException);

        // Return all the interfaces on this machine
        // (caller must delete returned vector)
        static NetworkInterfaceList* getNetworkInterfaces()
            throw(SocketException);

    protected:
        // IP addresses
        InetAddressList _ip_list;
};


} //namespace TIDSocket


ostream& operator<< (ostream& os, const TIDSocket::NetworkInterface& netIf);


#endif //__TID_SOCKET_NETWORK_INTERFACE_H__
