///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSession.h
// Description: This file contains the SSLContext class methods
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


#ifndef __TID_SOCKET_SSL_SESSION_H__
#define __TID_SOCKET_SSL_SESSION_H__

#include <string>



#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <iostream>
#else
#include <iostream.h>
#endif


#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::string;
#endif


using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{
typedef string                             SessionId;

class SSLSession : public virtual RefCounter
{
  friend class SSLSocket;
  friend class SSLSessionContext;

protected:

  SSL_SESSION* _session;
  const SSL*   _ssl;

  SSLContext*  _context;

  char*        _peer_host;
  int          _peer_port;

public:

  // Creates an unbound server socket
  SSLSession(SSLContext* context, const SSL* _ssl, 
             const char* peer_host, int peer_port)
    throw(IOException, SystemException);

  // Destroys the server socket
  virtual ~SSLSession()
    throw(SystemException);

public:

  // Gets the size of the largest application buffer that may occur when using 
  // this session.
  int getApplicationBufferSize();

  // Returns the name of the SSL cipher suite which is used for all connections 
  // in the session.
  const char* getCipherSuite();

  // Returns the time at which this Session representation was created, in 
  // milliseconds since midnight, January 1, 1970 UTC.
  long getCreationTime();

  // Returns the identifier assigned to this Session.
  SessionId getId();
  
  // Returns the last time this Session representation was accessed by the 
  // session level infrastructure, in milliseconds since midnight, January 1, 1970 UTC.
  long getLastAccessedTime();

//   // Returns the certificate(s) that were sent to the peer during handshaking.
//   Certificate[] getLocalCertificates();

//    // Returns the principal that was sent to the peer during handshaking.
//   Principal getLocalPrincipal();

   // Gets the size of the largest SSL/TLS packet that may occur when using this session.
  int getPacketBufferSize();

//    // Returns the identity of the peer which was identified as part of defining the session.
//   X509Certificate[] getPeerCertificateChain();

//    // Returns the identity of the peer which was established as part of defining the session.
//   Certificate[] getPeerCertificates();

   // Returns the host name of the peer in this session.
  const char* getPeerHost();

   // Returns the port number of the peer in this session.
  int getPeerPort();

//    // Returns the identity of the peer which was established as part of defining the session.
//   Principal getPeerPrincipal();

   // Returns the standard name of the protocol used for all connections in the session.
  const char* getProtocol();

//    // Returns the context in which this session is bound.
//   SSLSessionContext getSessionContext();

//    // Returns the object bound to the given name in the session's application layer data.
//   Object getValue(String name);

   // Returns an array of the names of all the application layer data objects bound into the Session.
  char** getValueNames();

   // Invalidates the session.
  void 	invalidate();

   // Returns whether this session is valid and available for resuming or joining.
  bool isValid();

//    // Binds the specified value object into the session's application layer data with the given name.
//   void putValue(String name, Object value);

//    // Removes the object bound to the given name in the session's application layer data.
//   void removeValue(String name);


 protected:
  void setTimeout(int seconds);

};


} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_CONTEXT_H__
