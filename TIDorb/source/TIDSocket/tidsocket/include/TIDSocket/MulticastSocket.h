///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_MulticastSocket.h
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


#ifndef __TID_SOCKET_MULTICAST_SOCKET_H__
#define __TID_SOCKET_MULTICAST_SOCKET_H__


#include <sys/types.h>


using TIDThr::SystemException;


namespace TIDSocket
{


class MulticastSocket : public virtual DatagramSocket
{
    friend class DatagramChannel;

    public:
        // Create a multicast socket
        MulticastSocket()
            throw(SocketException, SystemException);

        // Create a multicast socket and bind it to a specific port
        MulticastSocket(in_port_t port)
            throw(SocketException, SystemException);

        // Create a multicast socket bound to the specified socket address
        MulticastSocket(const SocketAddress* bindAddr)
            throw(SocketException, SystemException);

        // Destroys the multicast socket
        virtual ~MulticastSocket()
            throw(SystemException);

    public:
        // Retrieve the address of the network interface used for
        // multicast packets
        // (caller must delete InetAddress object)
        InetAddress* getInterface()
            throw(SocketException);

        // Get the setting for local loopback of multicast datagrams
        bool getLoopbackMode()
            throw(SocketException);

        // Get the multicast network interface set.
        // (caller must delete NetworkInterface object)
        NetworkInterface* getNetworkInterface()
            throw(SocketException);

        // Get the default time-to-live for multicast packets sent out
        // on the socket
        int getTimeToLive()
            throw(IOException);

        // Joins a multicast group
        void joinGroup(const InetAddress& mcastaddr)
            throw(IOException);

        // Joins the specified multicast group at the specified interface
        void joinGroup(const SocketAddress& mcastaddr,
                       const NetworkInterface* netIf)
            throw(IOException);

        // Leave a multicast group
        void leaveGroup(const InetAddress& mcastaddr)
            throw(IOException);

        // Leave a multicast group on a specified local interface
        void leaveGroup(const SocketAddress& mcastaddr,
                        const NetworkInterface* netIf)
            throw(IOException);

        // Set the multicast network interface used by methods whose behavior
        // would be affected by the value of the network interface
        void setInterface(const InetAddress& inf)
            throw(SocketException);

        // Disable/Enable local loopback of multicast datagrams. The option is
        // used by the platform's networking code as a hint for setting whether
        // multicast data will be looped back to the local socket
        void setLoopbackMode(bool disable)
            throw(SocketException);

        // Specify the network interface for outgoing multicast datagrams sent
        // on this socket
        void setNetworkInterface(const NetworkInterface& netIf)
            throw(SocketException);

        // Set the default time-to-live for multicast packets sent out on this
        // multicast socket in order to control the scope of the multicasts
        void setTimeToLive(int ttl)
            throw(IOException);

    private:
        // Object initialization
        void init(const SocketAddress* bindAddr)
            throw(SocketException);

};


} //namespace TIDSocket


#endif //__TID_SOCKET_MULTICAST_SOCKET_H__
