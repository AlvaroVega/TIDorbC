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
        virtual DatagramChannel& connect(const SocketAddress& addr)
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
                     const SocketAddress& target)
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
        ssize_t write(const unsigned char* src, size_t src_len)
            throw(NotYetConnectedException, IOException);

    public:
        // Opens a datagram channel
        // (caller must delete DatagramChannel object)
        static DatagramChannel* open()
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
