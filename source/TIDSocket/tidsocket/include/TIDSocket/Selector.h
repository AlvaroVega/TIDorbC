///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Selector.h
// Description: This file contains the Selector class methods
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


#ifndef __TID_SOCKET_SELECTOR_KEY_H__
#define __TID_SOCKET_SELECTOR_KEY_H__


#include <sys/types.h>
#include <sys/time.h>
#include <vector>


#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::vector;
#endif


namespace TIDSocket
{


class SelectionKey;
class SelectableChannel;


class Selector
{
    friend class SocketChannel;
    friend class ServerSocketChannel;
    friend class DatagramChannel;

    public:
        // Returns this selector's key set
        const fd_set* keys()
            throw();

        // Selects a set of keys whose corresponding channels are ready for I/O
        // operations
        int select()
            throw(IOException);

        // Selects a set of keys whose corresponding channels are ready for I/O
        // operations (timeout in milliseconds)
        int select(time_t timeout)
            throw(IOException, IllegalArgumentException);

        // Selects a set of keys whose corresponding channels are ready for I/O
        // operations
        int selectNow()
            throw(IOException);

        // Returns this selector's selected-key set
        const fd_set* selectedKeys()
            throw();

    public:
        // Opens a selector
        // (caller must delete Selector object)
        static Selector* open()
            throw(IOException);

    protected:
        // Constructor
        Selector()
            throw();

    private:
        // implSelect
        int implSelect(struct timeval* timeout)
            throw(IOException);

        // registerKey
        SelectionKey& registerKey(int fd, int ops, SelectableChannel* channel)
            throw();

    protected:
        int             _max_fd;
        fd_set          _key_set;
        fd_set          _sel_key_set;
        SelectionKeySet _reg_keys;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SELECTION_KEY_H__
