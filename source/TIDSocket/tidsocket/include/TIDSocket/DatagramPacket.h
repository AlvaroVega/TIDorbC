///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramPacktet.h
// Description: This file contains the DatagramPacket class methods
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


#ifndef __TID_SOCKET_DATAGRAM_PACKET_H__
#define __TID_SOCKET_DATAGRAM_PACKET_H__


#include <sys/types.h>


namespace TIDSocket
{


class DatagramPacket
{
    public:
        // Constructs a datagram packet for receiving packets of length 'length'
        DatagramPacket(unsigned char* buf, size_t buflen, size_t length)
            throw(IllegalArgumentException);

        // Constructs a datagram packet for sending packets of length 'length'
        // to the specified port number on the specified host
        DatagramPacket(unsigned char* buf, size_t buflen, size_t length,
                       const InetAddress& address, in_port_t port)
            throw(IllegalArgumentException);

        // Constructs a datagram packet for receiving packets of length 'length'
        // specifying an offset into the buffer
        DatagramPacket(unsigned char* buf, size_t buflen,
                       size_t offset, size_t length)
            throw(IllegalArgumentException);

        // Constructs a datagram packet for sending packets of length 'length'
        // with offset 'offset' to the specified port number on the
        // specified host
        DatagramPacket(unsigned char* buf, size_t buflen,
                       size_t offset, size_t length,
                       const InetAddress& address, in_port_t port)
            throw(IllegalArgumentException);

        // Constructs a datagram packet for sending packets of length 'length'
        // with offset 'offset' to the specified port number on the
        // specified host
        DatagramPacket(unsigned char* buf, size_t buflen,
                       size_t offset, size_t length,
                       const SocketAddress& address)
            throw(IllegalArgumentException);

        // Constructs a datagram packet for sending packets of length 'length'
        // to the specified port number on the specified host
        DatagramPacket(unsigned char* buf, size_t buflen, size_t length,
                       const SocketAddress& address)
            throw(IllegalArgumentException);

        // Destructor
        virtual ~DatagramPacket()
            throw();

    public:
        // Returns the IP address of the machine to which this datagram is being
        // sent or from which the datagram was received
        const InetAddress* getAddress() const
            throw();

        // Returns the data buffer
        const unsigned char* getData() const
            throw();

        // Returns the length of the data to be sent or the length of the data
        // received
        ssize_t getLength() const
            throw();

        // Returns the offset of the data to be sent or the offset of the data
        // received
        size_t getOffset() const
            throw();

        // Returns the port number on the remote host to which this datagram
        // is being sent or from which the datagram was received
        int getPort() const
            throw();

        // Gets the SocketAddress (usually IP address + port number) of the
        // remote host that this packet is being sent to or is coming from
        const SocketAddress* getSocketAddress() const
            throw();

        // Sets the IP address of the machine to which this datagram is
        // being sent
        void setAddress(const InetAddress& iaddr)
            throw();

        // Sets the data buffer for this packet
        void setData(unsigned char* buf, size_t buflen)
            throw(IllegalArgumentException);

        // Sets the data buffer for this packet
        void setData(unsigned char* buf, size_t buflen,
                     size_t offset, size_t length)
            throw(IllegalArgumentException);

        // Sets the length for this packet
        void setLength(size_t length)
            throw(IllegalArgumentException);

        // Sets the port number on the remote host to which this datagram is
        // being sent
        void setPort(in_port_t iport)
            throw();

        // Sets the SocketAddress (usually IP address + port number) of the
        // remote host to which this datagram is being sent
        void setSocketAddress(const SocketAddress& address)
            throw(IllegalArgumentException);

    protected:
        unsigned char*           _buffer;
        size_t                   _buflen;
        size_t                   _offset;
        ssize_t                  _length;
        InetSocketAddress*       _address;

    private:
        void init(unsigned char* buf,
                  size_t         buflen,
                  size_t         offset,
                  size_t         length,
                  SocketAddress* address)
            throw(IllegalArgumentException);
};


} //namespace TIDSocket


#endif //__TID_SOCKET_DATAGRAM_PACKET_H__
