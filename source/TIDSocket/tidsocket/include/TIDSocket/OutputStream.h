///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_OutputStream.h
// Description: This file contains the OutputStream class methods
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


#ifndef __TID_SOCKET_OUTPUT_STREAM_H__
#define __TID_SOCKET_OUTPUT_STREAM_H__


#include <sys/types.h>


#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <fstream>
#else
#include <fstream.h>
#endif

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
using std::ofstream;
#endif


namespace TIDSocket
{


class OutputStream // caceres@tid.es 14/7/2004 : public virtual ofstream
{
    friend class Socket;

    public:
        // Constructor (timeout in milliseconds)
        OutputStream(int fd, time_t timeout)
           throw() :
           // caceres@tid.es 14/7/2004 ofstream(fd),
           _fd(fd), _timeout(timeout), _channel(NULL) {}

        // Destructor
        virtual ~OutputStream()
           throw() {}

    public:
        // Closes this output stream and releases any system resources
        // associated with this stream
        void close()
            throw(IOException);

        // Flushes this output stream and forces any buffered output bytes to
        // be written out
        void flush()
            throw(IOException);

        // Writes the specified byte to this output stream
        ssize_t write(unsigned char b)
            throw(IOException, IllegalBlockingModeException);

        // Writes size bytes from the specified byte array to this output stream
        ssize_t write(const unsigned char* b, size_t size)
            throw(IOException, IllegalBlockingModeException);

        // Writes len bytes from the specified byte array starting at offset
        // off to this output stream
        ssize_t write(const unsigned char* b,size_t size,size_t off,size_t len)
            throw(IOException, IllegalBlockingModeException,
                  IndexOutOfBoundsException);

    protected:
        // File descriptor associated
        int _fd;

        // Timeout on blocking Socket operations (milliseconds)
        time_t _timeout;

        // Channel to delegate I/O operations
        SelectableChannel* _channel;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_OUTPUT_STREAM_H__
