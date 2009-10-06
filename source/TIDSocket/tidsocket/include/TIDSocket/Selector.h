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
