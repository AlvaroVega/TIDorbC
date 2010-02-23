///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SelectableChannel.C
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
#include "SocketChannelStatus.h"




using TIDThr::Synchronized;




namespace TIDSocket
{




//
// Constructor
//
SelectableChannel::SelectableChannel()
    throw(SystemException)
{
    _status = TID_SOCKET_CHANNEL_STATUS_BLOCKING;
}




//
// Destructor
//
SelectableChannel::~SelectableChannel()
    throw(SystemException)
{
}




//
// blockingLock()
//
RecursiveMutex& SelectableChannel::blockingLock()
    throw()
{
    return _blocking;
}




//
// configureBlocking()
//
SelectableChannel& SelectableChannel::configureBlocking(bool block)
    throw(ClosedChannelException, IllegalBlockingModeException, IOException)
{
    try
    {
        Synchronized synchronized(blockingLock());
        {
            // Check if channel is open
            if (! isOpen())
            {
                throw ClosedChannelException("Channel is not open");
            }

            // Check for illegal blocking mode
            if (block && isRegistered())
            {
                throw IllegalBlockingModeException("Channel is registered");
            }

            // Check for a blocking mode change
            if (block ^ isBlocking())
            {
                Synchronized synchronized(_sync);
                {
                    implConfigureBlocking(block);

                    if (block) _status |= TID_SOCKET_CHANNEL_STATUS_BLOCKING;
                    else _status &= ~TID_SOCKET_CHANNEL_STATUS_BLOCKING;
                }
            }
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }

    return *this;
}




//
// close()
//
void SelectableChannel::close()
    throw(IOException)
{
    if (isOpen())
    {
        try
        {
            Synchronized synchronized(_sync);
            {
                _status &= ~TID_SOCKET_CHANNEL_STATUS_OPEN;
                implCloseChannel();
            }
        }
        catch(TIDThr::Exception& e)
        {
            throw IOException(e.what());
        }
    }
}




//
// isBlocking()
//
bool SelectableChannel::isBlocking()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_CHANNEL_STATUS_BLOCKING);
        }
    }
    catch(TIDThr::Exception& e)
    {
    }

    return false;
}




//
// isOpen()
//
bool SelectableChannel::isOpen()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_CHANNEL_STATUS_OPEN);
        }
    }
    catch(TIDThr::Exception& e)
    {
    }

    return false;
}




//
// isRegistered()
//
bool SelectableChannel::isRegistered()
    throw()
{
    try
    {
        Synchronized synchronized(_sync);
        {
            return (_status & TID_SOCKET_CHANNEL_STATUS_REGISTERED);
        }
    }
    catch(TIDThr::Exception& e)
    {
    }

    return false;
}




//
// registerChannel()
//
SelectionKey& SelectableChannel::registerChannel(Selector& sel, int ops)
    throw(ClosedChannelException, IllegalBlockingModeException,
          IllegalArgumentException, IOException)
{
    try
    {
        Synchronized synchronized(blockingLock());
        {
            // Check if channel is open
            if (! isOpen())
            {
                throw ClosedChannelException("Channel is not open");
            }

            // Check for illegal blocking mode
            if (isBlocking())
            {
                throw IllegalBlockingModeException("Channel is registered");
            }

            // Check operation set
            if (ops & ~validOps())
            {
                throw IllegalArgumentException("Invalid channel operation");
            }

            Synchronized synchronized(_sync);
            {
                _status |= TID_SOCKET_CHANNEL_STATUS_REGISTERED;
                return implRegisterChannel(sel, ops);
            }
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}




} //namespace TIDSocket
