///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramChannel.h
// Description: This file contains the DatagramChannel class methods
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


#ifndef __TID_SOCKET_DATAGRAM_CHANNEL_H__
#define __TID_SOCKET_DATAGRAM_CHANNEL_H__


#include <sys/types.h>


using TIDThr::SystemException;


namespace TIDSocket
{


class DatagramSocket;


class DatagramChannel : public virtual SelectableChannel
{
    public:
        // Destructor
        virtual ~DatagramChannel()
            throw(SystemException);

        // Connects this channel's socket
        virtual DatagramChannel& connect(const SocketAddress& addr,const char* interface)
            throw(ClosedChannelException,
                  AsynchronousCloseException,
                  ClosedByInterruptException,
                  IOException);

        // Disconnects this channel's socket
        virtual DatagramChannel& disconnect()
            throw(IOException);

        // Tells whether or not this channel's socket is connected
        bool isConnected()
            throw();

        // Reads a datagram from this channel
        ssize_t read(unsigned char* dst, size_t dst_len)
            throw(NotYetConnectedException, IOException);

        // Receives a datagram via this channel
        // (caller must delete returned object)
        SocketAddress* receive(unsigned char* dst, size_t dst_len)
            throw(ClosedChannelException, ClosedByInterruptException,
                  AsynchronousCloseException, IOException);

        // Sends a datagram via this channel
        ssize_t send(const unsigned char* src, size_t src_len,
                     const SocketAddress& target,const char* interface)
            throw(ClosedChannelException, ClosedByInterruptException,
                  AsynchronousCloseException, IOException);

        // Retrieves a datagram socket associated with this channel
        const DatagramSocket* socket()
            throw();

        // Returns an operation set identifying this channel's supported
        // operations
        int validOps()
            throw();

        // Writes a datagram to this channel
        ssize_t write(const unsigned char* src, size_t src_len,const char* interface)
            throw(NotYetConnectedException, IOException);

    public:
        // Opens a datagram channel
        // (caller must delete DatagramChannel object)
        static DatagramChannel* open(const char* interface)
            throw(IOException);

    protected:
        // Constructor
        DatagramChannel()
            throw(SystemException);

        // Adjusts this channel's blocking mode
        void implConfigureBlocking(bool block)
            throw(IOException);

        // Closes this channel
        void implCloseChannel()
            throw(IOException);

        // Registers this channel with the given selector, returning a
        SelectionKey& implRegisterChannel(Selector& sel, int ops)
            throw();

    protected:
        DatagramSocket* _socket;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_DATAGRAM_CHANNEL_H__
