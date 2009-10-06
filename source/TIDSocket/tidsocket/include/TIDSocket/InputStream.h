///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InputStream.h
// Description: This file contains the InputStream class methods
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


#ifndef __TID_SOCKET_INPUT_STREAM_H__
#define __TID_SOCKET_INPUT_STREAM_H__


#include <sys/types.h>


#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <fstream>
#else
#include <fstream.h>
#endif

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
using std::ifstream;
using std::streampos;
#endif



namespace TIDSocket
{


class InputStream // caceres@tid.es 14/7/2004 : public virtual ifstream
{
    friend class Socket;

    public:
        // Constructor (timeout in milliseconds)
        InputStream(int fd, time_t timeout)
            throw() :
            // caceres@tid.es 14/7/2004->  ifstream(fd),
            _fd(fd), _timeout(timeout), _channel(NULL) {}

        // Destructor
        virtual ~InputStream()
            throw() {}

    public:
        // Returns the number of bytes that can be read (or skipped over) from
        // this input stream without blocking by the next caller of a method
        // for this input stream
        size_t available()
            throw(IOException);

        // Closes this input stream and releases any system resources associated
        // with the stream
        void close()
            throw(IOException);

        // Marks the current position in this input stream
        void mark(streampos readlimit)
            throw();

        // Tests if this input stream supports the mark and reset methods
        bool markSupported()
            throw();

        // Reads the next byte of data from the input stream
        int read()
            throw(IOException, IllegalBlockingModeException);

        // Reads some number of bytes from the input stream and stores them
        // into the buffer array b
        ssize_t read(unsigned char* b, size_t size)
            throw(IOException, IllegalBlockingModeException);

        // Reads up to len bytes of data from the input stream
        // into an array of bytes
        ssize_t read(unsigned char* b, size_t size, size_t off, size_t len)
            throw(IOException, IllegalBlockingModeException,
                  IndexOutOfBoundsException);

        // Repositions this stream to the position at the time the mark method
        // was last called on this input stream
        void reset()
            throw(IOException);

        // Skips over and discards n bytes of data from this input stream
        size_t skip(size_t n)
            throw(IOException);

    protected:
        // File descriptor associated
        int _fd;

        // Timeout on blocking Socket operations (milliseconds)
        time_t _timeout;

        // Channel to delegate I/O operations
        SelectableChannel* _channel;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_INPUT_STREAM_H__
