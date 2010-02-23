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
