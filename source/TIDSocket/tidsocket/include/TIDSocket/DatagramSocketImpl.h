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
