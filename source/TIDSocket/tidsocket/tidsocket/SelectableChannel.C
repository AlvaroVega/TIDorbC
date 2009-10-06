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
