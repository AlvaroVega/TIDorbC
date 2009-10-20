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
