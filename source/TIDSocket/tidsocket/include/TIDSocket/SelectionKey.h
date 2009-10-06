///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SelectionKey.h
// Description: This file contains the SelectionKey class methods
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


#ifndef __TID_SOCKET_SELECTION_KEY_H__
#define __TID_SOCKET_SELECTION_KEY_H__


#include <vector>


#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::vector;
#endif


namespace TIDSocket
{


class SelectableChannel;
class Selector;

class SelectionKey;
typedef vector<SelectionKey> SelectionKeySet;


class SelectionKey
{
    friend class Selector;
    friend class SocketChannel;

    public:
        // Returns the channel for which this key was created
        const SelectableChannel* channel()
            throw();

        // Retrieves this key's interest set
        int interestOps()
            throw();

        // Sets this key's interest set to the given value
        SelectionKey& interestOps(int ops)
            throw(IllegalArgumentException);

        // Tests whether this key's channel is ready to accept a new socket
        // connection
        bool isAcceptable()
            throw();

        // Tests whether this key's channel has either finished, or failed to
        // finish, its socket-connection operation
        bool isConnectable()
            throw();

        // Tests whether this key's channel is ready for reading
        bool isReadable()
            throw();

        // Tests whether this key's channel is ready for writing
        bool isWritable()
            throw();

        // Retrieves this key's ready-operation set
        int readyOps()
            throw();

        // Returns the selector for which this key was created
        const Selector* selector()
            throw();

    public:
        static const int OP_ACCEPT  = 0x0001;
        static const int OP_CONNECT = 0x0002;
        static const int OP_READ    = 0x0004;
        static const int OP_WRITE   = 0x0008;

    protected:
        SelectionKey(
            int                 fd,
            Selector*           selector,
            SelectableChannel*  channel,
            int                 interest)
            throw();

    protected:
        int                     _fd;
        int                     _interest;
        int                     _ready;
        Selector*               _selector;
        SelectableChannel*      _channel;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SELECTION_KEY_H__
