///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SelectableChannel.h
// Description: This file contains the SelectableChannel class methods
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


#ifndef __TID_SOCKET_SELECTABLE_CHANNEL_H__
#define __TID_SOCKET_SELECTABLE_CHANNEL_H__


using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{


class SelectableChannel
{
    public:
        // Destructor
        virtual ~SelectableChannel()
            throw(SystemException);

        // Retrieves the object upon which the configureBlocking and
        // registerChannel methods synchronize
        virtual RecursiveMutex& blockingLock()
            throw();

        // Adjusts this channel's blocking mode
        virtual SelectableChannel& configureBlocking(bool block)
            throw(ClosedChannelException, IllegalBlockingModeException,
                  IOException);

        // Closes this channel
        virtual void close()
            throw(IOException);

        // Tells whether or not every I/O operation on this channel will block
        // until it completes
        virtual bool isBlocking()
            throw();

        // Tells whether or not this channel is open
        virtual bool isOpen()
            throw();

        // Tells whether or not this channel is currently registered with
        // any selectors
        virtual bool isRegistered()
            throw();

        // Registers this channel with the given selector, returning a
        // selection key
        virtual SelectionKey& registerChannel(Selector& sel, int ops)
            throw(ClosedChannelException, IllegalBlockingModeException,
                  IllegalArgumentException, IOException);

        // Returns an operation set identifying this channel's supported
        // operations
        virtual int validOps()
            throw() = 0;

    protected:
        // Constructor
        SelectableChannel()
            throw(SystemException);

        // Adjusts this channel's blocking mode
        virtual void implConfigureBlocking(bool block)
            throw(IOException) = 0;

        // Closes this channel
        virtual void implCloseChannel()
            throw(IOException) = 0;

        // Registers this channel with the given selector, returning a
        virtual SelectionKey& implRegisterChannel(Selector& sel, int ops)
            throw() = 0;

    protected:
        long           _status;
        RecursiveMutex _blocking;
        RecursiveMutex _sync;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SELECTABLE_CHANNEL_H__
