///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SelectionKey.C
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




namespace TIDSocket
{




//
// Constructor
//
SelectionKey::SelectionKey(
    int                 fd,
    Selector*           selector,
    SelectableChannel*  channel,
    int                 interest)
    throw()
{
    _fd       = fd;
    _selector = selector;
    _interest = interest;
    _channel  = channel;
    _ready    = 0;
}




//
// channel()
//
const SelectableChannel* SelectionKey::channel()
    throw()
{
    return _channel;
}




//
// interestOps() (getter)
//
int SelectionKey::interestOps()
    throw()
{
    return _interest;
}




//
// interestOps() (setter)
//
SelectionKey& SelectionKey::interestOps(int ops)
    throw(IllegalArgumentException)
{
    // Comprueba ops
    if (ops & ~_channel->validOps())
    {
        throw IllegalArgumentException("Invalid interest ops");
    }

    _interest = ops;
    return *this;
}




//
// isAcceptable()
//
bool SelectionKey::isAcceptable()
    throw()
{
    return (_ready & OP_ACCEPT);
}




//
// isConnectable()
//
bool SelectionKey::isConnectable()
    throw()
{
    return (_ready & OP_CONNECT);
}




//
// isReadable()
//
bool SelectionKey::isReadable()
    throw()
{
    return (_ready & OP_READ);
}




//
// isWritable()
//
bool SelectionKey::isWritable()
    throw()
{
    return (_ready & OP_WRITE);
}




//
// readyOps()
//
int SelectionKey::readyOps()
    throw()
{
    return _ready;
}




//
// selector()
//
const Selector* SelectionKey::selector()
    throw()
{
    return _selector;
}




} //namespace TIDSocket
