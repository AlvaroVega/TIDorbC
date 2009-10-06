///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SocketImpl.h
// Description: This file contains the SocketImpl class methods
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


#ifndef __TID_SOCKET_SOCKET_IMPL_H__
#define __TID_SOCKET_SOCKET_IMPL_H__


#include <sys/types.h>


namespace TIDSocket
{


class SocketImpl : public virtual SocketOptions
{
    friend class Socket;
    friend class ServerSocket;
    friend class PlainSocketImpl;

    protected:
        // Virtual destructor
        virtual ~SocketImpl()
            throw() {}

        // Assign operator
        SocketImpl& operator= (const SocketImpl& s)
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

        // Connects this socket to the specified port number on the
        // specified host
        virtual void connect(const InetAddress& address, in_port_t port)
            throw(IOException) = 0;

        // Connects this socket to the specified port number on the
        // specified host
        virtual void connect(const SocketAddress& address, time_t timeout)
            throw(IOException) = 0;

        // Connects this socket to the specified port on the named host
        virtual void connect(const char* host, in_port_t port)
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
        // The IP address of the remote end of this socket
        InetAddress _address;

        // The file descriptor object for this socket
        FileDescriptor _fd;

        // The local port number to which this socket is connected
        in_port_t _localport;

        // The port number on the remote host to which this socket is connected
        in_port_t _port;

        // Timeout on blocking Socket operations
        time_t _timeout;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SOCKET_IMPL_H__
