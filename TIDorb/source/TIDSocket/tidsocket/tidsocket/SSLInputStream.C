///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InetStream.C
// Description: This file contains the InetStream class methods
// Rel:         01.00
// Created:     May, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
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
#include <sys/time.h>
#include <sys/types.h>
#include <stropts.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/err.h>

#ifdef __sun
#include <sys/conf.h>
#include <sys/select.h>
#include <string.h>
#endif //__sun


#include <algorithm>

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES

#include <sstream>
#define OSTRINGSTREAM std::ostringstream
using std::ends;

#else

#include <strstream.h>
#define OSTRINGSTREAM ostrstream

#endif

#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::min;
#endif




namespace TIDSocket
{





//
// close()
//
void SSLInputStream::close()
    throw(IOException)
{
  int ret = SSL_shutdown(_ssl);

  if (ret == 2) 
    ret = SSL_shutdown(_ssl);

  if (!ret) {

    int ssl_error = SSL_get_error(_ssl, ret);
    int error = ERR_get_error();
    
    OSTRINGSTREAM reason;
    reason << "SSLInputStream::close error: ";

    switch(ssl_error) {
    case SSL_ERROR_NONE:
      reason << "SSL_ERROR_NONE reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_READ:
      reason << "SSL_ERROR_WANT_READ reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_WRITE:
      reason << "SSL_ERROR_WANT_WRITE reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_ZERO_RETURN:
      reason << "SSL_ERROR_ZERO_RETURN reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_CONNECT:
      reason << "SSL_ERROR_WANT_CONNECT reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_X509_LOOKUP:
      reason << "SSL_ERROR_WANT_X509_LOOKUP reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_SYSCALL:
      reason << "SSL_ERROR_SYSCALL reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_SSL:
      reason << "SSL_ERROR_SSL reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    default:
      reason << "unknown: " << ends;
      throw IOException(reason.str().c_str());
    }
  }
  

}



//
// read()
//
ssize_t SSLInputStream::read(unsigned char* b, size_t size, size_t off, size_t len)
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

    // Timeout in microseconds (by default, infinite timeout)
    struct timeval  tval_timeout;
    struct timeval* timeoutptr = NULL;
    if (_timeout) {
      tval_timeout.tv_sec  = ((long) _timeout) / 1000L;
      tval_timeout.tv_usec = 1000L * (((long) _timeout) % 1000L);
      timeoutptr = &tval_timeout;
    }


    //SSL_set_mode(_ssl, SSL_MODE_AUTO_RETRY);
    
    // Comprueba si el stream esta listo para lectura
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET((int) _fd, &rset);
    //int error = select((int) _fd + 1, &rset, NULL, NULL, timeoutptr);
    int error = 1;
    if (error <= 0 || !FD_ISSET((int) _fd, &rset)) {
      // Corrige errno para indicar vencimiento del timeout
      errno = (error == 0) ? ETIMEDOUT : errno;
    }
    else {
      // Lee del stream
      n = ::SSL_read(_ssl, (void*) (b + off), len);
    }

    // Comprobacion de errores
    if (n <= 0) {
      
      int ssl_error = SSL_get_error(_ssl, n);
      int error = ERR_get_error();

      OSTRINGSTREAM reason;
      reason << "SSLInputStream::read  error: ";

      switch(ssl_error) {
      case SSL_ERROR_NONE:
        reason << "SSL_ERROR_NONE reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_WANT_READ:
        reason << "SSL_ERROR_WANT_READ reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_WANT_WRITE:
        reason << "SSL_ERROR_WANT_WRITE reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_ZERO_RETURN:
        reason << "SSL_ERROR_ZERO_RETURN reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_WANT_CONNECT:
        reason << "SSL_ERROR_WANT_CONNECT reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_WANT_X509_LOOKUP:
        reason << "SSL_ERROR_WANT_X509_LOOKUP reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_SYSCALL:
        reason << "SSL_ERROR_SYSCALL reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      case SSL_ERROR_SSL:
        reason << "SSL_ERROR_SSL reason: " << ERR_reason_error_string(error) << ends;
        throw IOException(reason.str().c_str());
        break;
      default:
        reason << "unknown: " << ends;
        throw IOException(reason.str().c_str());
      }

    }

    return n;
}


} //namespace TIDSocket
