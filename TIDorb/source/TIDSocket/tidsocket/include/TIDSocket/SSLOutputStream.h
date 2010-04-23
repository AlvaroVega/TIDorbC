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
