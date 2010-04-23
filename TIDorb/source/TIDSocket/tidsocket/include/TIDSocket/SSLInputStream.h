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
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
