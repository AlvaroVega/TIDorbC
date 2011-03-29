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
