///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLInputStream.h
// Description: This file contains the SSLInputStream class methods
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


#ifndef __TID_SOCKET_SSL_INPUT_STREAM_H__
#define __TID_SOCKET_SSL_INPUT_STREAM_H__


#include <sys/types.h>
#include <openssl/ssl.h>

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


class SSLInputStream: public virtual InputStream
{
  friend class Socket;
  friend class SSLSocket;
  
public:

  // Constructor (timeout in milliseconds)
  SSLInputStream(int fd, time_t timeout, SSL* ssl)
    throw() :
    InputStream(fd, timeout), _ssl(ssl) {}

  // Destructor
  virtual ~SSLInputStream()
    throw() {}

public:

  // Closes this input stream and releases any system resources associated
  // with the stream
  void close()
    throw(IOException);

  // Reads up to len bytes of data from the input stream
  // into an array of bytes
  ssize_t read(unsigned char* b, size_t size, size_t off, size_t len)
    throw(IOException, IllegalBlockingModeException,
          IndexOutOfBoundsException);

protected:

  // SSL 
  SSL *_ssl;

};


} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_INPUT_STREAM_H__
