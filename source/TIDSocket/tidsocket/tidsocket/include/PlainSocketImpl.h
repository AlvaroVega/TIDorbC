///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_include/PlainSocketImpl.h
// Description: 
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


#ifndef __TID_SOCKET_PLAIN_SOCKET_IMPL_H__
#define __TID_SOCKET_PLAIN_SOCKET_IMPL_H__


#include <sys/types.h>
#include <sys/socket.h>


namespace TIDSocket
{


class PlainSocketImpl : public virtual SocketImpl,
                        public virtual DatagramSocketImpl
{
    using SocketImpl::_fd;
    using SocketImpl::_localport;

    friend class SocketChannel;
    friend class DatagramChannel;

    public:
        // Constructor
        PlainSocketImpl()
            throw();

        // Fetch the value of an option
        // (caller must delete object pointed to by void* result; object's type
        // and size depends on the optID argument, see documentation)
        void* getOption(int optID, size_t& result_size) const
            throw(SocketException);

        // Enable/disable the option specified by optID
        // (type and size of object pointed to by 'value' depends on optID)
        void setOption(int optID, const void* value, size_t size)
            throw(SocketException);

        // Accept a connection
        void accept(SocketImpl* s)
            throw(IOException);

        // Return the number of bytes that can be read from this socket
        // without blocking
        size_t available()
            throw(IOException);

        // Bind this socket to the specified port number on the
        // specified host
        void bind(const InetAddress& host, in_port_t port)
            throw(SocketException);

        // Bind a datagram socket to a local port and address
        void bind(in_port_t lport, const InetAddress& laddr)
            throw(SocketException);

        // Close this socket
        void close()
            throw(IOException);

        // Connect this socket to the specified port number on the
        // specified host
        void connect(const InetAddress& address, in_port_t port)
            throw(SocketException);

        // Connect this socket to the specified port number on the
        // specified host (timeout in milliseconds)
        void connect(const SocketAddress& address, time_t timeout)
            throw(SocketException);

        // Connect this socket to the specified port number on the
        // named host
        void connect(const char* host, in_port_t port)
            throw(SocketException);

        // Create either a stream or a datagram socket
        void create(bool stream)
            throw(SocketException);

        // Create a datagram socket
        void create()
            throw(SocketException);

        // Disconnect a datagram socket from its remote destination
        void disconnect()
            throw();

        // Get the datagram socket file descriptor
        FileDescriptor getFileDescriptor() const
            throw();

        // Return an input stream for this socket
        // (caller must delete InputStream object)
        InputStream* getInputStream()
            throw(IOException);

        // Get the local port
        in_port_t getLocalPort() const
            throw();

        // Return an output stream for this socket
        // (caller must delete OutputStream object)
        OutputStream* getOutputStream()
            throw(IOException);

        // Retrieve the TTL (time-to-live) option
        int getTimeToLive()
            throw(IOException);

        // Join the multicast group
        void join(const InetAddress& inetaddr)
            throw(IOException);

        // Join the multicast group
        void joinGroup(const SocketAddress& mcastaddr,
                       const NetworkInterface& netIf)
            throw(IOException);

        // Leave the multicast group
        void leave(const InetAddress& inetaddr)
            throw(IOException);

        // Leave the multicast group
        void leaveGroup(const SocketAddress& mcastaddr,
                        const NetworkInterface& netIf)
            throw(IOException);

        // Set the maximum queue length for incoming connection indications
        // (a request to connect) to the backlog argument
        void listen(int backlog)
            throw(IOException);

        // Peek at the packet to see who it is from
        int peek(const InetAddress& i)
            throw(IOException, PortUnreachableException);

        // Peek at the packet to see who it is from
        int peekData(const DatagramPacket& i)
            throw(IOException, PortUnreachableException);

        // Receive the datagram packet
        void receive(DatagramPacket& i)
            throw(IOException, PortUnreachableException);

        // Send the datagram packet
        void send(DatagramPacket& i)
            throw(IOException, PortUnreachableException);

        // Send one byte of urgent data on the socket
        void sendUrgentData(int data)
            throw(IOException);

        // Set the TTL (time-to-live) option
        void setTimeToLive(int ttl)
            throw(IOException);

        // Return whether or not this socket supports sending urgent data
        bool supportsUrgentData()
            throw();

    public:
        // Any port number
        static const in_port_t ANY_PORT  = 0;

        // Default socket backlog
        static const int DEFAULT_BACKLOG = 50;

        // Fetch the value of an integer option
        static int getIntOption(const SocketOptions* socket, int optID)
            throw(SocketException);

        // Fetch the value of a boolean option
        static bool getBoolOption(const SocketOptions* socket, int optID)
            throw(SocketException);

        // Fetch the value of a size_t option
        static size_t getSizeOption(const SocketOptions* socket, int optID)
            throw(SocketException);

        // Fetch the value of a time_t option
        static time_t getTimeOption(const SocketOptions* socket, int optID)
            throw(SocketException);

        // Enable/disable the integer option specified by optID
        static void setIntOption(SocketOptions* socket, int optID, int value)
            throw(SocketException);

        // Enable/disable the boolean option specified by optID
        static void setBoolOption(SocketOptions* socket, int optID, bool value)
            throw(SocketException);

        // Enable/disable the size_t option specified by optID
        static void setSizeOption(SocketOptions* socket, int optID, size_t value)
            throw(SocketException);

        // Enable/disable the time_t option specified by optID
        static void setTimeOption(SocketOptions* socket, int optID, time_t value)
            throw(SocketException);

        // Get struct ip_mreq from pair (SocketAddress,NetworkInterface)
        static void toMulticastReq(const SocketAddress&    addr,
                                   const NetworkInterface& netIf,
                                   struct ip_mreq&         req)
            throw(IOException);

        // Get struct sockaddr from pair (InetAddress,port)
        static void toSockAddr(const InetAddress& inet,
                               in_port_t          port,
                               struct sockaddr&   sock,
                               socklen_t&         size)
            throw();

        // Get struct sockaddr initialized to null socket address
        static void toNullSockAddr(struct sockaddr& sock,
                                   socklen_t&       size)
            throw();

    private:
        // Fetch int value from buffer containing an int
        static int* toInt(const void* buffer, socklen_t bufflen)
            throw();

        // Fetch bool value from buffer containing an unsigned char or int
        static bool* toBool(const void* buffer, socklen_t bufflen)
            throw();

        // Fetch size_t value from buffer containing an int
        static size_t* toSize(const void* buffer, socklen_t bufflen)
            throw();

        // Fetch time_t value in seconds from buffer containing a struct linger
        static time_t* toSeconds(const void* buffer, socklen_t bufflen)
            throw();

        // Fetch InetAddress from buffer containing a struct in_addr
        static InetAddress* toInetAddress(const void* buffer,
                                          socklen_t   bufflen)
            throw(SocketException);

        // Fetch InetSocketAddress from buffer containing a struct sockaddr_in
        static InetSocketAddress* toInetSocketAddress(const void* buffer,
                                                      socklen_t   bufflen)
            throw(SocketException);

        // Store int value from buffer into int destination
        static void fromInt(const void* buffer, size_t bufflen,
                            void*       dest,   size_t destlen)
            throw(SocketException);

        // Store bool value from buffer into unsigned char or int destination
        static void fromBool(const void* buffer, size_t bufflen,
                             void*       dest,   size_t destlen)
            throw(SocketException);

        // Store size_t value from buffer into int destination
        static void fromSize(const void* buffer, size_t bufflen,
                             void*       dest,   size_t destlen)
            throw(SocketException);

        // Store value in seconds from buffer into struct linger destination
        static void fromSeconds(const void* buffer, size_t bufflen,
                                void*       dest,   size_t destlen)
            throw(SocketException);

        // Store InetAddress from buffer into struct in_addr destination
        static void fromInetAddress(const void* buffer, size_t bufflen,
                                    void*       dest,   size_t destlen)
            throw(SocketException);
};


} //namespace TIDSocket


#endif //__TID_SOCKET_PLAIN_SOCKET_IMPL_H__