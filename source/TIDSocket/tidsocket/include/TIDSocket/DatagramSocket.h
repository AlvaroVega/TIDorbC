///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramSocket.h
// Description: This file contains the DatagramSocket class methods
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


#ifndef __TID_SOCKET_DATAGRAM_SOCKET_H__
#define __TID_SOCKET_DATAGRAM_SOCKET_H__


#include <sys/types.h>


using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{


class DatagramSocket : public virtual TIDThr::RefCounter
{
    friend class DatagramChannel;

    public:
        // Constructs a datagram socket and binds it to any available port
        // on the local host machine
        DatagramSocket(const char* interface = NULL, bool ipv6 = false)
            throw(SocketException, SystemException);

        // Constructs a datagram socket and binds it to the specified port
        // on the local host machine
        DatagramSocket(in_port_t port,const char* interface = NULL, 
                       bool ipv6 = false)
            throw(SocketException, SystemException);

        // Creates a datagram socket, bound to the specified local
        // address
        DatagramSocket(in_port_t port, const InetAddress& localAddr,
                       const char* interface = false, bool ipv6 = false)
            throw(SocketException, SystemException);

        // Creates a datagram socket, bound to the specified local
        // socket address
        DatagramSocket(const SocketAddress* bindAddr,
                       const char* interface = false, bool ipv6 = false)
            throw(SocketException, SystemException);

        // Destroys the datagram socket
        virtual ~DatagramSocket()
            throw(SystemException);

    protected:
        // Creates an unbound datagram socket with specified DatagramSocketImpl
        // (caller must NOT delete DatagramSocketImpl object)
        DatagramSocket(const DatagramSocketImpl* impl, bool ipv6 = false)
            throw(SocketException, SystemException);

    public:
        // Sets the datagram socket implementation factory for the application
        static void
        setDatagramSocketImplFactory(const DatagramSocketImplFactory& fac)
            throw(SocketException);

    public:
        // Binds this DatagramSocket to a specific address and port
        void bind(const SocketAddress* addr, const char* interface = false)
            throw(SocketException, IllegalArgumentException);

        // Closes this datagram socket
        void close()
            throw();

        // Connects the socket to a remote address for this socket
        void connect(const InetAddress& address, in_port_t port,
                     const char* interface = NULL)
            throw(SocketException, IllegalArgumentException);

        // Connects this socket to a remote socket address
        void connect(const SocketAddress& addr,const char* interface)
            throw(SocketException, IllegalArgumentException);

        // Disconnects the socket
        void disconnect()
            throw();

        // Tests if SO_BROADCAST is enabled
        bool getBroadcast()
            throw(SocketException);

        // Returns the unique DatagramChannel object associated with this
        // datagram socket, if any (caller must delete returned object)
        const DatagramChannel* getChannel()
            throw();

        // Returns the address to which this socket is connected
        const InetAddress* getInetAddress()
            throw();

        // Gets the local address to which the socket is bound
        // (caller must delete InetAddress object)
        InetAddress* getLocalAddress()
            throw();

        // Returns the port number on the local host to which
        // this socket is bound
        in_port_t getLocalPort()
            throw();

        // Returns the address of the endpoint this socket is bound to, or null
        // if it is not bound yet (caller must delete SocketAddress object)
        SocketAddress* getLocalSocketAddress()
            throw();

        // Returns the port for this socket
        int getPort()
            throw();

        // Get value of the SO_RCVBUF option for this DatagramSocket, that is
        // the buffer size used by the platform for input on this DatagramSocket
        size_t getReceiveBufferSize()
            throw(SocketException);

        // Returns the address of the endpoint this socket is connected to, or
        // null if it is unconnected (caller must delete SocketAddress object)
        SocketAddress* getRemoteSocketAddress()
            throw();

        // Tests if SO_REUSEADDR is enabled
        bool getReuseAddress()
            throw(SocketException);

        // Get value of the SO_SNDBUF option for this DatagramSocket, that is
        // the buffer size used by the platform for output on this DatagramSocket
        size_t getSendBufferSize()
            throw(SocketException);

        // Returns setting for SO_TIMEOUT
        time_t getSoTimeout()
            throw(SocketException);

        // Gets traffic class or type-of-service in the IP datagram header for
        // packets sent from this DatagramSocket
        int getTrafficClass()
            throw(SocketException);

        // Returns the binding state of the socket
        bool isBound()
            throw();

        // Returns the closed state of the socket
        bool isClosed()
            throw();

        // Returns the connection state of the socket
        bool isConnected()
            throw();

        // Receives a datagram packet from this socket
        void receive(DatagramPacket& p)
            throw(IOException, SocketTimeoutException,
                  PortUnreachableException, IllegalBlockingModeException);

        // Sends a datagram packet from this socket
        void send(DatagramPacket& p,const char* interface = NULL)
            throw(IOException,
                  PortUnreachableException, IllegalBlockingModeException);

        // Enable/disable SO_BROADCAST
        void setBroadcast(bool on)
            throw(SocketException);

        // Sets the SO_RCVBUF option to the specified value for this socket
        void setReceiveBufferSize(size_t size)
            throw(SocketException, IllegalArgumentException);

        // Enable/disable the SO_REUSEADDR socket option
        void setReuseAddress(bool on)
            throw(SocketException);

        // Sets the SO_SNDBUF option to the specified value for this socket
        void setSendBufferSize(size_t size)
            throw(SocketException, IllegalArgumentException);

        // Enable/disable SO_TIMEOUT with the specified timeout, in milliseconds
        void setSoTimeout(time_t timeout)
            throw(SocketException);

        // Sets traffic class or type-of-service octet in the IP header for
        // packets sent from this socket
        void setTrafficClass(int tc)
            throw(SocketException, IllegalArgumentException);

    protected:
        RecursiveMutex                    _sync;
        DatagramSocketImpl*               _impl;
        DatagramChannel*                  _channel;
        //InetAddress                      _remoteaddr;
        InetAddress*                      _remoteaddr;
        in_port_t                         _remoteport;
        long                              _status;
        bool                              _ipv6;

    protected:
        static DatagramSocketImplFactory* _factory;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_DATAGRAM_SOCKET_H__
