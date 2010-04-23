///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSessionContext.C
// Description: This file contains the SSLSessionContext class methods
// Rel:         01.00
// Created:     Sep, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es )
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

using TIDThr::Synchronized;


namespace TIDSocket {


//
// Constructor
//
SSLSessionContext::SSLSessionContext()
  throw(IOException, SystemException)
{

}




SSLSessionContext::~SSLSessionContext()
  throw(SystemException)
{
  _sessions.clear();
}



// Returns the SSLSession bound to the specified session id. 
SSLSession* SSLSessionContext::getSession(SessionId sessionId)
{
  SSLSessionsTable::iterator it = _sessions.find(sessionId);
  if (it != _sessions.end())
    return (*it).second;
  else 
    return 0;
}


// Returns an Enumeration of all session id's grouped under this 
// SSLSessionContext. 
//Enumeration SSLSessionContext::getIds();
//{

//}


// Sets the timeout limit for SSLSession objects grouped under this 
// SSLSessionContext. 
void SSLSessionContext::setSessionTimeout(int seconds)
  throw (IllegalArgumentException)
{
  _session_timeout = seconds;

  SSLSessionsTable::iterator begin = _sessions.begin();
  SSLSessionsTable::iterator end   = _sessions.end();
  SSLSessionsTable::iterator it;

  for (it = begin; it != end; it++) {
    ((*it).second)->setTimeout(seconds);
  }
}
   
// Returns the timeout limit of SSLSession objects grouped under this 
// SSLSessionContext. 
int SSLSessionContext::getSessionTimeout()
{
  return _session_timeout;
}
  
// Sets the size of the cache used for storing SSLSession objects grouped 
// under this SSLSessionContext. 
void SSLSessionContext::setSessionCacheSize(int size)
  throw (IllegalArgumentException)
{
  // TODO
}


// Returns the size of the cache used for storing SSLSession objects grouped 
// under this SSLSessionContext. 
int SSLSessionContext::getSessionCacheSize()
{
  // TODO
  return 0;
}  

void SSLSessionContext::addSession(SessionId sessionId, 
                                   SSLSession* session)
{
  _sessions[sessionId] = session;
}



} //namespace TIDSocket
