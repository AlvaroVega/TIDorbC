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
