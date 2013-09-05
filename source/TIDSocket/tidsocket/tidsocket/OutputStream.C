///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_OutputStream.C
// Description: This file contains the OutputStream class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//   02/29/2008 by Alvaro Polo <apv@tid.es>
//      * Some ioctl constants defined conditionally for Darwin
//        (Mac OS X) systems
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
#include <sys/types.h>
#if (defined __darwin || defined __CYGWIN__ || defined __ANDROID__ || defined __mips)
   #include <sys/ioctl.h>

   // I_FLUSH was declared in CoreServices framework in OSX < 10.4
   // This framework cannot be used in 10.5 and later, so this
   // value is included directly
   #define I_FLUSH ((65 << 8) | 5)
   #define FLUSHW  0x02
#else
   #include <stropts.h>
#endif
#include <unistd.h>
#include <errno.h>

#ifdef __sun
#include <sys/conf.h>
#endif //__sun




namespace TIDSocket
{




//
// close()
//
void OutputStream::close()
    throw(IOException)
{
}




//
// flush()
//
void OutputStream::flush()
    throw(IOException)
{
    if (ioctl((int) _fd, I_FLUSH, FLUSHW))
    {
        throw IOException("flush() error", errno);
    }
}




//
// write()
//
ssize_t OutputStream::write(unsigned char b)
    throw(IOException, IllegalBlockingModeException)
{
    return write((const unsigned char*) &b, sizeof(b), 0, sizeof(b));
}




//
// write()
//
ssize_t OutputStream::write(const unsigned char* b, size_t size)
    throw(IOException, IllegalBlockingModeException)
{
    return write(b, size, 0, size);
}




//
// write()
//
ssize_t
OutputStream::write(const unsigned char* b, size_t size, size_t off, size_t len)
    throw(IOException, IllegalBlockingModeException, IndexOutOfBoundsException)
{
    // Comprueba los limites del array
    if (off + len > size)
    {
        throw IndexOutOfBoundsException("Index out of bounds");
    }

    // Delega la operacion en el canal, si este existe
    if (_channel)
    {
        SocketChannel* ch = dynamic_cast<SocketChannel*>(_channel);
        return ch->write(b+off, len);
    }

    // Escritura
    ssize_t n = ::write((int) _fd, (void*) (b + off), len);
    if (n == -1)
    {
        throw IOException("write() error", errno);
    }

    return n;
}




} //namespace TIDSocket
