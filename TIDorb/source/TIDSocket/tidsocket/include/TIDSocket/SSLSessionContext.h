///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSessionContext.h
// Description: This file contains the SSLSessionContext class methods
// Rel:         01.00
// Created:     Sep, 2009
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


#ifndef __TID_SOCKET_SSL_SESSION_CONTEXT_H__
#define __TID_SOCKET_SSL_SESSION_CONTEXT_H__


#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>


#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{

class SSLSessionContext : public virtual RefCounter
{
  friend class SSLSocket;

private:

  int _session_timeout;


protected:

  //typedef char* SessionId; // TODO ???

  typedef map<SessionId, SSLSession*> SSLSessionsTable;

  SSLSessionsTable _sessions;




public:

  // Creates an unbound server socket
  SSLSessionContext()
    throw(IOException, SystemException);

  // Destroys the server socket
  virtual ~SSLSessionContext()
    throw(SystemException);

public:

  // Returns the SSLSession bound to the specified session id. 
  SSLSession* getSession(SessionId sessionId);

  // Returns an Enumeration of all session id's grouped under this 
  // SSLSessionContext. 
  //Enumeration getIds();

  // Sets the timeout limit for SSLSession objects grouped under this 
  // SSLSessionContext. 
  void setSessionTimeout(int seconds)
    throw (IllegalArgumentException);
   
  // Returns the timeout limit of SSLSession objects grouped under this 
  // SSLSessionContext. 
  int getSessionTimeout();
  
  // Sets the size of the cache used for storing SSLSession objects grouped 
  // under this SSLSessionContext. 
  void setSessionCacheSize(int size)
    throw (IllegalArgumentException);

  // Returns the size of the cache used for storing SSLSession objects grouped 
  // under this SSLSessionContext. 
  int getSessionCacheSize();
  

  void addSession(SessionId sessionId, SSLSession* session);

};


} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_SESSION_CONTEXT_H__
