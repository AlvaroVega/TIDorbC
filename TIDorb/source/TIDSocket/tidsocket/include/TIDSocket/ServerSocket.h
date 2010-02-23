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
        ServerSocket()
            throw(IOException, SystemException);

        // Creates a server socket on a specified port
        ServerSocket(in_port_t port)
            throw(IOException, SystemException);

        // Creates a server socket and binds it to the specified local port
        // number, with the specified backlog
        ServerSocket(in_port_t port, int backlog)
            throw(IOException, SystemException);

        // Creates a server socket with the specified port, listen backlog,
        // and local IP address to bind to
        ServerSocket(in_port_t port, int backlog, const InetAddress* addr)
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
        Socket* accept()
            throw(IOException, IllegalBlockingModeException);

        // Binds the server socket to a specific address (IP address and port)
        void bind(const SocketAddress* endpoint)
            throw(IOException, IllegalArgumentException);

        // Binds the server socket to a specific address (IP address and port)
        void bind(const SocketAddress* endpoint, int backlog)
            throw(IOException, IllegalArgumentException);

        // Closes this socket
        void close()
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

    protected:
        static SocketImplFactory*  _factory;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SERVER_SOCKET_H__
