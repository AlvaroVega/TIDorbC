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
