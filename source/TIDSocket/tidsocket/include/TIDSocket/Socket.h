///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_TIDSocket.h
// Description: This file contains the TIDSocket class methods
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


#ifndef __TID_SOCKET_SOCKET_H__
#define __TID_SOCKET_SOCKET_H__


#include <sys/types.h>


using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{


class Socket : public virtual RefCounter
{
    friend class ServerSocket;
    friend class SocketChannel;
    friend class ServerSocketChannel;

    public:
        // Create an unconnected socket, with the system-default SocketImpl
        Socket(bool ipv6=false)
            throw(IOException, SystemException);

        // Create a stream socket and connects it to the specified port number
        // at the specified IP address
        Socket(const InetAddress& address,   in_port_t port, 
               const char* interface = NULL, bool ipv6 = false)
            throw(IOException, SystemException);

        // Create a socket and connects it to the specified remote address on
        // the specified remote port
        Socket(const InetAddress& address,   in_port_t port,
               const InetAddress& localAddr, in_port_t localPort,
               const char* interface = NULL, bool ipv6 = false)
            throw(IOException, SystemException);

        // Create a stream socket and connects it to the specified port number
        // on the named host
        Socket(const char* host, in_port_t port, const char* interface = NULL,
               bool ipv6 = false)
            throw(UnknownHostException, IOException, SystemException);

        // Create a socket and connects it to the specified remote host on the
        // specified remote port
        Socket(const char* host,             in_port_t port,
               const InetAddress& localAddr, in_port_t localPort, 
               const char* interface = NULL, bool ipv6 = false)
            throw(IOException, SystemException);

        // Destroy the socket
        virtual ~Socket()
            throw(SystemException);

    protected:
        // Create an unconnected socket with a user-specified SocketImpl
        // (caller must NOT delete SocketImpl object)
        Socket(const SocketImpl* impl)
            throw(SocketException, SystemException);

    public:
        // Set the client socket implementation factory for the application
        static void setSocketImplFactory(const SocketImplFactory& fac)
            throw(SocketException);

    public:
        // Bind the socket to a local address
        void bind(const SocketAddress* bindpoint, const char* interface = NULL)
            throw(IOException, IllegalArgumentException);

        // Close this socket
        virtual void close()
            throw(IOException);

        // Connect this socket to the server
        void connect(const SocketAddress& endpoint, const char* interface = NULL)
            throw(IllegalBlockingModeException, IllegalArgumentException,
                  IOException);

        // Connect this socket to the server with a specified timeout value
        void connect(const SocketAddress& endpoint, time_t timeout,
                     const char* interface = NULL)
            throw(IllegalBlockingModeException, IllegalArgumentException,
                  IOException);

        // Return the unique SocketChannel object associated with this socket,
        // if any
        const SocketChannel* getChannel()
            throw();

        // Return the address to which the socket is connected
        const InetAddress* getInetAddress()
            throw();

        // Return an input stream for this socket
        // (caller must delete istream object)
        virtual InputStream* getInputStream()
            throw(IOException);

        // Test if SO_KEEPALIVE is enabled
        bool getKeepAlive()
            throw(SocketException);

        // Get the local address to which the socket is bound
        // (caller must delete InetAddress object)
        InetAddress* getLocalAddress()
            throw();

        // Return the local port to which this socket is bound
        int getLocalPort()
            throw();

        // Return the address of the endpoint this socket is bound to, or null
        // if it is not bound yet
        // (caller must delete SocketAddress object)
        SocketAddress* getLocalSocketAddress()
            throw();

        // Test if OOBINLINE is enabled
        bool getOOBInline()
            throw(SocketException);

        // Return an output stream for this socket
        // (caller must delete ostream object)
        virtual OutputStream* getOutputStream()
            throw(IOException);

        // Return the remote port to which this socket is connected
        int getPort()
            throw();

        // Get the value of the SO_RCVBUF option for this Socket, that is the
        // buffer size used by the platform for input on this Socket
        size_t getReceiveBufferSize()
            throw(SocketException);

        // Return the address of the endpoint this socket is connected to, or
        // null if it is unconnected
        // (caller must delete SocketAddress object)
        SocketAddress* getRemoteSocketAddress()
            throw();

        // Test if SO_REUSEADDR is enabled
        bool getReuseAddress()
            throw(SocketException);

        // Get value of the SO_SNDBUF option for this Socket, that is the buffer
        // size used by the platform for output on this Socket
        size_t getSendBufferSize()
            throw(SocketException);

        // Return setting for SO_LINGER (seconds)
        time_t getSoLinger()
            throw(SocketException);

        // Return setting for SO_TIMEOUT (milliseconds)
        time_t getSoTimeout()
            throw(SocketException);

        // Test if TCP_NODELAY is enabled
        bool getTcpNoDelay()
            throw(SocketException);

        // Get traffic class or type-of-service in the IP header for packets
        // sent from this socket
        int getTrafficClass()
            throw(SocketException);

        // Return the binding state of the socket
        bool isBound()
            throw();

        // Return the closed state of the socket
        bool isClosed()
            throw();

        // Return the connection state of the socket
        bool isConnected()
            throw();

        // Return whether the read-half of the socket connection is closed
        bool isInputShutdown()
            throw();

        // Return whether the write-half of the socket connection is closed
        bool isOutputShutdown()
            throw();

        // Send one byte of urgent data on the socket
        void sendUrgentData(int data)
            throw(IOException);

        // Enable/disable SO_KEEPALIVE
        void setKeepAlive(bool on)
            throw(SocketException);

        // Enable/disable OOBINLINE (receipt of TCP urgent data). By default,
        // this option is disabled and TCP urgent data received on a socket is
        // silently discarded
        void setOOBInline(bool on)
            throw(SocketException);

        // Set the SO_RCVBUF option to the specified value for this socket
        void setReceiveBufferSize(size_t size)
            throw(SocketException, IllegalArgumentException);

        // Enable/disable the SO_REUSEADDR socket option
        void setReuseAddress(bool on)
            throw(SocketException);

        // Set the SO_SNDBUF option to the specified value for this socket
        void setSendBufferSize(size_t size)
            throw(SocketException, IllegalArgumentException);

        // Enable/disable SO_LINGER with the specified linger time in seconds
        void setSoLinger(bool on, time_t linger_time)
            throw(SocketException, IllegalArgumentException);

        // Enable/disable SO_TIMEOUT with the specified timeout, in milliseconds
        void setSoTimeout(time_t timeout)
            throw(SocketException);

        // Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm)
        void setTcpNoDelay(bool on)
            throw(SocketException);

        // Set traffic class or type-of-service octet in the IP header for
        // packets sent from this socket
        void setTrafficClass(int tc)
            throw(SocketException);

        // Place the input stream for this socket at "end of stream"
        void shutdownInput()
            throw(IOException);

        // Disable the output stream for this socket
        void shutdownOutput()
            throw(IOException);

    //protected:
    public:
        RecursiveMutex            _sync;
        SocketImpl*               _impl;
        SocketChannel*            _channel;
        long                      _status;
        bool                      _ipv6;

    protected:
        static SocketImplFactory* _factory;

    protected:
        void setConnected()
            throw();
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SOCKET_H__
