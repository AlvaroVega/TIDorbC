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
#include <sys/types.h>
#ifdef __darwin
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
