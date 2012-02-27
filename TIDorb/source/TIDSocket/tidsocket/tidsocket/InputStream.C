///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InetStream.C
// Description: This file contains the InetStream class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
//   02/29/2008 by Alvaro Polo <apv@tid.es>
//      * Some ioctl constants defined conditionally for Darwin
//        (Mac OS X) systems
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
#include <sys/time.h>
#include <sys/types.h>
#if (defined __darwin || defined __CYGWIN__ || defined __ANDROID__ || defined __mips)
   #include <sys/ioctl.h>
   
   // I_NREAD was declared in CoreServices framework in OSX < 10.4
   // This framework cannot be used in 10.5 and later, so this
   // value is included directly
   #define I_NREAD ((65 << 8) | 1)
#else
   #include <stropts.h>
#endif
#include <unistd.h>
#include <errno.h>

#ifdef __sun
#include <sys/conf.h>
#include <sys/select.h>
#include <string.h>
#endif //__sun


#include <algorithm>

#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::min;
#endif




#define SKIP_BUFFER_SIZE 128U




namespace TIDSocket
{




//
// available()
//
size_t InputStream::available()
    throw(IOException)
{
    int size = 0;
    if (ioctl((int) _fd, I_NREAD, &size))
    {
        throw IOException("available() error", errno);
    }

    return (size_t) size;
}




//
// close()
//
void InputStream::close()
    throw(IOException)
{
}




//
// mark()
//
void InputStream::mark(streampos readlimit)
    throw()
{
}




//
// markSupported()
//
bool InputStream::markSupported()
    throw()
{
    return false;
}




//
// read()
//
int InputStream::read()
    throw(IOException, IllegalBlockingModeException)
{
    int value = 0;

    unsigned char* b = (unsigned char*) &value;
    ssize_t len = sizeof(unsigned char);

    if (read(b, sizeof(value), 0, len) != len)
    {
        value = -1;
    }

    return value;
}




//
// read()
//
ssize_t InputStream::read(unsigned char* b, size_t size)
    throw(IOException, IllegalBlockingModeException)
{
    return read(b, size, 0, size);
}




//
// read()
//
ssize_t InputStream::read(unsigned char* b, size_t size, size_t off, size_t len)
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
        return ch->read(b + off, len);
    }

    // Bytes leidos
    ssize_t n = -1;

    // Lee del stream si no se ha alcanzado EOF

    /* caceres@tid.es 14/7/2004
    if (! eof())
    {
    */
        // Timeout in microseconds (by default, infinite timeout)
        struct timeval  tval_timeout;
        struct timeval* timeoutptr = NULL;
        if (_timeout)
        {
            tval_timeout.tv_sec  = ((long) _timeout) / 1000L;
            tval_timeout.tv_usec = 1000L * (((long) _timeout) % 1000L);
            timeoutptr = &tval_timeout;
        }

        // Comprueba si el stream esta listo para lectura
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET((int) _fd, &rset);
        int error = select((int) _fd + 1, &rset, NULL, NULL, timeoutptr);
        if (error <= 0 || !FD_ISSET((int) _fd, &rset))
        {
            // Corrige errno para indicar vencimiento del timeout
            errno = (error == 0) ? ETIMEDOUT : errno;
        }
        else
        {
            // Lee del stream
            n = ::read((int) _fd, (void*) (b + off), len);
        }

        // Comprobacion de errores
        if (n == -1)
        {
            if (errno == ETIMEDOUT)
                throw SocketTimeoutException("read() timeout");
            else if (errno == EINTR)
                n = read(b, size, off, len);
            else
                throw IOException("read() error", errno);
        }

    /* caceres@tid.es: 14/7/2004
    }
    */

    return n;
}




//
// reset()
//
void InputStream::reset()
    throw(IOException)
{
}




//
// skip()
//
size_t InputStream::skip(size_t n)
    throw(IOException)
{
    /* caceres@tid.es 14/7/2004
    ignore((int) n);
    return (size_t) gcount();
    */

    unsigned char skip_buffer[SKIP_BUFFER_SIZE];

    if (n == 0)
    {
        return n;
    }

    size_t buff_available       = available();
    size_t to_skip              = min(n, buff_available);
    size_t to_read              = to_skip;
    size_t to_read_in_this_loop = 0;


    while (to_read > 0)
    {
        to_read_in_this_loop = min((unsigned int)to_read, SKIP_BUFFER_SIZE);

        size_t n_read = 0;
        while (n_read < to_read_in_this_loop)
        {
            size_t read_res = read(skip_buffer, SKIP_BUFFER_SIZE, 0,
                                   to_read_in_this_loop - n_read);
            n_read += read_res;
        }

        to_read -= to_read_in_this_loop;
    }

    return to_skip;
}




} //namespace TIDSocket
