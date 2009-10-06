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
        InetSocketAddress(const InetAddress* inaddr, in_port_t port)
            throw(IllegalArgumentException);

        // Create a socket address where the IP address is the wildcard address
        // and the port number a specified value
        InetSocketAddress(in_port_t port)
            throw(IllegalArgumentException);

        // Create a socket address from a hostname and a port number
        InetSocketAddress(const char* hostname, in_port_t port)
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
        InetAddress _addr;
        in_port_t   _port;
        bool        _resolved;

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
