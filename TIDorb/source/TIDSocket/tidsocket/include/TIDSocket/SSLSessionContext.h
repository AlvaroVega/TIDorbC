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
