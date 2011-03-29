///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSocketImpl.h
// Description: This file contains the SSLSocketImpl class methods
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


#ifndef __TID_SOCKET_SOCKET_IMPL_H__
#define __TID_SOCKET_SOCKET_IMPL_H__


#include <sys/types.h>


namespace TIDSocket
{


class SSLSocketImpl : public virtual SSLSocket
{
    friend class SSLSocket;
    friend class SSLServerSocket;

 public:

    SSLSocketImpl(SSLContextImpl context,
                  const char* host,
                  int port)
      throw (IOException, UnknownHostException);

    SSLSocketImpl(SSLContextImpl context,
                  InetAddress host,
                  int port)
      throw (IOException, UnknownHostException);

    SSLSocketImpl(SSLContextImpl context, const char* host, int port,
                  InetAddress localAddr, int localPort)
      throw (IOException, UnknownHostException);

    SSLSocketImpl(SSLContextImpl context, InetAddress host, int port,
                  InetAddress localAddr, int localPort)
      throw (IOException, UnknownHostException);

    SSLSocketImpl(SSLContextImpl context, bool servMode, int port,
                  InetAddress localAddr, int localPort)
      throw (IOException, UnknownHostException);

    SSLSocketImpl(SSLContextImpl context);

    SSLSocketImpl(SSLContextImpl context, Socket sock, const char* host,
                  int port, bool autoclose)
      throw (IOException);


    protected:
        // Virtual destructor
        virtual ~SSLSocketImpl()
            throw() {}

        void init(SocketImpl context, bool isServer);


        // Connects this socket to the specified port number on the
        // specified host
        void connect(const InetAddress& address, in_port_t port)
            throw(IOException);

        // Connects this socket to the specified port number on the
        // specified host
        void connect(const SocketAddress& address, time_t timeout)
            throw(IOException);

        // Connects this socket to the specified port on the named host
        void connect(const char* host, in_port_t port)
            throw(IOException);

        void doneConnect() throw (IOException);

        // Assign operator
        SocketImpl& operator= (const SSLSocketImpl& s)
            throw();

        // Equals operator
        bool operator== (const SocketImpl& s)
            throw();

        // Accepts a connection
        virtual void accept(SocketImpl* s)
            throw(IOException) = 0;

        // Returns the number of bytes that can be read from this socket
        // without blocking
        virtual size_t available()
            throw(IOException) = 0;

        // Binds this socket to the specified port number on the specified host
        virtual void bind(const InetAddress& host, in_port_t port)
            throw(IOException) = 0;

        // Closes this socket
        virtual void close()
            throw(IOException) = 0;



    public:
        // Creates either a stream or a datagram socket
        virtual void create(bool stream)
            throw(IOException) = 0;

        // Returns an input stream for this socket
        // (caller must delete istream object)
        virtual InputStream* getInputStream()
            throw(IOException) = 0;

        // Returns an output stream for this socket
        // (caller must delete ostream object)
        virtual OutputStream* getOutputStream()
            throw(IOException) = 0;

        // Sets the maximum queue length for incoming connection indications
        // (a request to connect) to the count argument
        virtual void listen(int backlog)
            throw(IOException) = 0;

        // Send one byte of urgent data on the socket
        virtual void sendUrgentData(int data)
            throw(IOException) = 0;

        // Returns the value of this socket's fd field
        virtual FileDescriptor getFileDescriptor() const
            throw();

        // Returns the value of this socket's address field
        const InetAddress& getInetAddress() const
            throw();

        // Returns the value of this socket's localport field
        virtual in_port_t getLocalPort() const
            throw();

        // Returns the value of this socket's port field
        in_port_t getPort() const
            throw();

        // Places the input stream for this socket at "end of stream"
        void shutdownInput()
            throw(IOException);

        // Disables the output stream for this socket
        void shutdownOutput()
            throw(IOException);

        // Returns whether or not this SocketImpl supports sending urgent data
        virtual bool supportsUrgentData()
            throw();

    protected:
        Socket   self;
        SSLContextImpl      sslContext;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SOCKET_IMPL_H__
