///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLOutputStream.h
// Description: This file contains the OutputStream class methods
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


#ifndef __TID_SOCKET_SSL_OUTPUT_STREAM_H__
#define __TID_SOCKET_SSL_OUTPUT_STREAM_H__


#include <sys/types.h>
#include <openssl/ssl.h>

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


class SSLOutputStream:  public virtual OutputStream
{
  friend class Socket;
  friend class SSLSocket;
  
public:
  // Constructor (timeout in milliseconds)
  SSLOutputStream(int fd, time_t timeout, SSL* ssl, BIO *bio)
    throw() :
  OutputStream(fd, timeout), _ssl(ssl), _sbio(bio) {}

  // Destructor
  virtual ~SSLOutputStream()
    throw() {}

public:

  // Closes this output stream and releases any system resources
  // associated with this stream
  void close()
    throw(IOException);

  // Writes len bytes from the specified byte array starting at offset
  // off to this output stream
  ssize_t write(const unsigned char* b,size_t size,size_t off,size_t len)
    throw(IOException, IllegalBlockingModeException,
          IndexOutOfBoundsException);

protected:

  // SSL 
  SSL*        _ssl;
  BIO*        _sbio;

};


} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_OUTPUT_STREAM_H__
