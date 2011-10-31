///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLOutputStream.C
// Description: This file contains the SSLOutputStream class methods
// Rel:         01.00
// Created:     May, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
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
#if (defined __darwin || defined __CYGWIN__)
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
#include <openssl/err.h>

#ifdef __sun
#include <sys/conf.h>
#endif //__sun


#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES

#include <sstream>
#define OSTRINGSTREAM std::ostringstream
using std::ends;

#else

#include <strstream.h>
#define OSTRINGSTREAM ostrstream

#endif


namespace TIDSocket
{




//
// close()
//
void SSLOutputStream::close()
    throw(IOException)
{

  int ret = SSL_shutdown(_ssl);

  if (ret == 2) 
    ret = SSL_shutdown(_ssl);

  if (!ret) {

    int ssl_error = SSL_get_error(_ssl, ret);
    int error = ERR_get_error();
    
    OSTRINGSTREAM reason;
    reason << "SSLOutputStream::close error: ";
      
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
// write()
//
ssize_t
SSLOutputStream::write(const unsigned char* b, size_t size, size_t off, size_t len)
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

    
    //SSL_set_connect_state(_ssl);
    //SSL_set_mode(_ssl,SSL_MODE_ENABLE_PARTIAL_WRITE);


    //ssize_t n = ::SSL_write(_ssl, (void*) (b + off), len);
    // Temporal workaround: 2 SSL_write per operation in order to force flush
    ssize_t n = ::SSL_write(_ssl, (void*) (b + off), len-1);
    n += ::SSL_write(_ssl, (void*) (b + off + len - 1), 1);

    if (n <= 0)
    {
      int ssl_error = SSL_get_error(_ssl, n);
      int error = ERR_get_error();
      
      OSTRINGSTREAM reason;
      reason << "SSLOutputStream::write error: ";

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

    
    //BIO_flush(_sbio);
    return n;
}




} //namespace TIDSocket
