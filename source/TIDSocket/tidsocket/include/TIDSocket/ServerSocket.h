///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_ServerSocket.h
// Description: This file contains the ServerSocket class methods
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


#ifndef __TID_SOCKET_SERVER_SOCKET_H__
#define __TID_SOCKET_SERVER_SOCKET_H__


using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{


class ServerSocket : public virtual RefCounter
{
    friend class ServerSocketChannel;

    public:
        // Creates an unbound server socket
        ServerSocket(bool ipv6 = false)
            throw(IOException, SystemException);

        // Creates a server socket on a specified port
        ServerSocket(in_port_t port, const char* interface = NULL, bool ipv6 = false)
            throw(IOException, SystemException);

        // Creates a server socket and binds it to the specified local port
        // number, with the specified backlog
        ServerSocket(in_port_t port, int backlog, const char* interface = NULL, 
                     bool ipv6 = false)
            throw(IOException, SystemException);

        // Creates a server socket with the specified port, listen backlog,
        // and local IP address to bind to
        ServerSocket(in_port_t port, int backlog, const InetAddress* addr, 
                     const char* interface = NULL, bool ipv6 = false)
            throw(IOException, SystemException);

        // Destroys the server socket
        virtual ~ServerSocket()
            throw(SystemException);

    public:
        // Sets the client socket implementation factory for the application
        static void setSocketImplFactory(const SocketImplFactory& fac)
            throw(SocketException);

    public:
        // Listens for a connection to be made to this socket and accepts it
        // (caller must delete Socket object)
        virtual Socket* accept()
            throw(IOException, IllegalBlockingModeException);

        // Binds the server socket to a specific address (IP address and port)
        void bind(const SocketAddress* endpoint, const char* interface = NULL)
            throw(IOException, IllegalArgumentException);

        // Binds the server socket to a specific address (IP address and port)
        void bind(const SocketAddress* endpoint, int backlog, const char* interface = NULL)
            throw(IOException, IllegalArgumentException);

        // Closes this socket
        virtual void close()
            throw(IOException);

        // Returns the unique ServerSocketChannel object associated with this
        // socket, if any
        const ServerSocketChannel* getChannel()
            throw();

        // Returns the local address of this server socket
        // (caller must delete InetAddress object)
        InetAddress* getInetAddress()
            throw();

        // Returns the local port on which this socket is listening
        int getLocalPort()
            throw();

        // Returns the address of the endpoint this socket is bound to, or null
        // if it is not bound yet
        // (caller must delete SocketAddress object)
        SocketAddress* getLocalSocketAddress()
            throw();

        // Gets the value of the SO_RCVBUF option for this server socket, that
        // is the proposed buffer size that will be used for Sockets accepted
        // from this server socket
        size_t getReceiveBufferSize()
            throw(SocketException);

        // Tests if SO_REUSEADDR is enabled
        bool getReuseAddress()
            throw(SocketException);

        // Returns setting for SO_TIMEOUT (milliseconds)
        time_t getSoTimeout()
            throw(SocketException);

        // Returns the binding state of the ServerSocket
        bool isBound()
            throw();

        // Returns the closed state of the ServerSocket
        bool isClosed()
            throw();

        // Sets a default proposed value for the SO_RCVBUF option for sockets
        // accepted from this server socket
        void setReceiveBufferSize(size_t size)
            throw(SocketException, IllegalArgumentException);

        // Enable/disable the SO_REUSEADDR socket option
        void setReuseAddress(bool on)
            throw(SocketException);

        // Enable/disable SO_TIMEOUT with the specified timeout, in milliseconds
        void setSoTimeout(time_t timeout)
            throw(SocketException);

    protected:
        // Subclasses of ServerSocket use this method to override accept() to
        // return their own subclass of socket
        void implAccept(Socket* s)
            throw(IOException, IllegalBlockingModeException);

    protected:
        RecursiveMutex             _sync;
        SocketImpl*                _impl;
        ServerSocketChannel*       _channel;
        long                       _status;
        bool                       _ipv6;

    protected:
        static SocketImplFactory*  _factory;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SERVER_SOCKET_H__
