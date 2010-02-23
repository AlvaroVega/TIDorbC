///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramSocketImpl.h
// Description: This file contains the DatagramSocketImpl class methods
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


#ifndef __TID_SOCKET_DATAGRAM_SOCKET_IMPL_H__
#define __TID_SOCKET_DATAGRAM_SOCKET_IMPL_H__


namespace TIDSocket
{


class DatagramSocketImpl : public virtual SocketOptions
{
    friend class DatagramSocket;
    friend class DatagramChannel;
    friend class PlainSocketImpl;
    friend class MulticastSocket;

    public:
        // Destructor
        virtual ~DatagramSocketImpl()
            throw();

        // Assign operator
        DatagramSocketImpl& operator= (const DatagramSocketImpl& s)
            throw();

        // Equals operator
        bool operator== (const DatagramSocketImpl& s)
            throw();

    protected:
        // Bind a datagram socket to a local port and address
        virtual void bind(in_port_t lport, const InetAddress& laddr)
            throw(SocketException) = 0;

        // Close this socket
        virtual void close()
            throw(IOException) = 0;

        // Connect a datagram socket to a remote destination
        virtual void connect(const InetAddress& address, in_port_t port)
            throw(SocketException);

        // Create a datagram socket
        virtual void create()
            throw(SocketException) = 0;

        // Disconnect a datagram socket from its remote destination
        virtual void disconnect()
            throw();

        // Get the datagram socket file descriptor
        virtual FileDescriptor getFileDescriptor() const
            throw();

        // Get the local port
        virtual in_port_t getLocalPort() const
            throw();

        // Retrieve the TTL (time-to-live) option
        virtual int getTimeToLive()
            throw(IOException) = 0;

        // Join the multicast group
        virtual void join(const InetAddress& inetaddr)
            throw(IOException) = 0;

        // Join the multicast group
        virtual void joinGroup(const SocketAddress& mcastaddr,
                               const NetworkInterface& netIf)
            throw(IOException) = 0;

        // Leave the multicast group
        virtual void leave(const InetAddress& inetaddr)
            throw(IOException) = 0;

        // Leave the multicast group
        virtual void leaveGroup(const SocketAddress& mcastaddr,
                                const NetworkInterface& netIf)
            throw(IOException) = 0;

        // Peek at the packet to see who it is from
        virtual int peek(const InetAddress& i)
            throw(IOException, PortUnreachableException) = 0;

        // Peek at the packet to see who it is from
        virtual int peekData(const DatagramPacket& i)
            throw(IOException, PortUnreachableException) = 0;

        // Receive the datagram packet
        virtual void receive(DatagramPacket& i)
            throw(IOException, PortUnreachableException) = 0;

        // Send the datagram packet
        virtual void send(DatagramPacket& i)
            throw(IOException, PortUnreachableException) = 0;

        // Set the TTL (time-to-live) option
        virtual void setTimeToLive(int ttl)
            throw(IOException) = 0;

    protected:
        // The file descriptor object for this socket
        FileDescriptor _fd;

        // The local port number to which this socket is connected
        in_port_t _localport;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_DATAGRAM_SOCKET_IMPL_H__
