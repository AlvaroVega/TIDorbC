///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SocketOptions.h
// Description: This file contains the SocketOptions class methods
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


#ifndef __TID_SOCKET_SOCKET_OPTIONS_H__
#define __TID_SOCKET_SOCKET_OPTIONS_H__


#include <sys/types.h>


namespace TIDSocket
{


struct SocketOptions
{
    // Set which outgoing interface on which to send multicast packets
    // (data type: InetAddress)
    static const int _IP_MULTICAST_IF   = 0;
    static const int _IP_MULTICAST_IF2  = 1;


    // This option enables or disables local loopback of multicast datagrams
    // (data type: bool)
    static const int _IP_MULTICAST_LOOP = 2;


    // This option sets the type-of-service or traffic class field in the IP
    // header for a TCP or UDP socket
    // (data type: int)
    static const int _IP_TOS            = 3;


    // Fetch the local address binding of a socket (this option cannot be "set"
    // only "gotten", since sockets are bound at creation time, and so the
    // locally bound address cannot be changed)
    // (data type: InetSocketAddress)
    static const int _SO_BINDADDR       = 4;


    // Set SO_BROADCAST for a socket
    // (data type: bool)
    static const int _SO_BROADCAST      = 5;


    // When the keepalive option is set for a TCP socket and no data has been
    // exchanged across the socket in either direction for 2 hours (NOTE: the
    // actual value is implementation dependent), TCP automatically sends a
    // keepalive probe to the peer
    // (data type: bool)
    static const int _SO_KEEPALIVE      = 6;


    // Specify a linger-on-close timeout
    // (data type: time_t)
    static const int _SO_LINGER         = 7;


    // When the OOBINLINE option is set, any TCP urgent data received on the
    // socket will be received through the socket input stream
    // (data type: bool)
    static const int _SO_OOBINLINE      = 8;


    // Set a hint the size of the underlying buffers used by the platform for
    // incoming network I/O
    // (data type: size_t)
    static const int _SO_RCVBUF         = 9;


    // Set SO_REUSEADDR for a socket
    // (data type: bool)
    static const int _SO_REUSEADDR      = 10;


    // Set a hint the size of the underlying buffers used by the platform for
    // outgoing network I/O
    // (data type: size_t)
    static const int _SO_SNDBUF         = 11;


    // Set a timeout on blocking Socket operations
    // (data type: time_t)
    static const int _SO_TIMEOUT        = 12;


    // Disable Nagle's algorithm for this connection
    // (data type: bool)
    static const int _TCP_NODELAY       = 13;
    
    
    static const int _IPV6_MULTICAST_IF = 14;
    static const int _IPV6_MULTICAST_IF2 = 15;
    static const int _IPV6_MULTICAST_LOOP = 16;


    // Fetch the value of an option
    virtual void* getOption(int optID, size_t& result_size) const
        throw(SocketException) = 0;

    // Enable/disable the option specified by optID
    virtual void setOption(int optID, const void* value, size_t size)
        throw(SocketException) = 0;

    virtual ~SocketOptions() {}
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SOCKET_OPTIONS_H__
