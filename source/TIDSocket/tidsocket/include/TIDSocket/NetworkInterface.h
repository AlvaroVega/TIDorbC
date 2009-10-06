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
