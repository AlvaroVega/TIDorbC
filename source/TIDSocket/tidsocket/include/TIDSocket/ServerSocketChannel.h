///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_ServerSocketChannel.h
// Description: This file contains the ServerSocketChannel class methods
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


#ifndef __TID_SOCKET_SERVER_SOCKET_CHANNEL_H__
#define __TID_SOCKET_SERVER_SOCKET_CHANNEL_H__


using TIDThr::SystemException;


namespace TIDSocket
{


class ServerSocket;
class SocketChannel;

class ServerSocketChannel : public virtual SelectableChannel
{
    public:
        // Destructor
        virtual ~ServerSocketChannel()
            throw(SystemException);

        // Accepts a connection made to this channel's socket
        SocketChannel* accept()
            throw(ClosedChannelException, ClosedByInterruptException,
                  AsynchronousCloseException,NotYetBoundException, IOException);

        // Retrieves a socket associated with this channel
        const ServerSocket* socket()
            throw();

        // Returns an operation set identifying this channel's supported
        // operations
        int validOps()
            throw();

    public:
        // Opens a socket channel
        // (caller must delete SocketChannel object)
        static ServerSocketChannel* open()
            throw(IOException);

    protected:
        // Constructor
        ServerSocketChannel()
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
        ServerSocket* _socket;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SERVER_SOCKET_CHANNEL_H__
