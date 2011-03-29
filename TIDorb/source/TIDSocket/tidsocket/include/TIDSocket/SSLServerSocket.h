///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLServerSocket.h
// Description: This file contains the SSLServerSocket class methods
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


#ifndef __TID_SOCKET_SSL_SERVER_SOCKET_H__
#define __TID_SOCKET_SSL_SERVER_SOCKET_H__


using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{


class SSLServerSocket : public virtual ServerSocket
{

private:

  SSLContext* _context;
  SSLSocket*  _s;  // socket(s)? accepted

  bool        _use_client_mode;
  bool        _session_creation;

public:

  // Creates an unbound server socket
  SSLServerSocket(SSLContext* context)
    throw(IOException, SystemException);
  
  // Creates a server socket on a specified port
  SSLServerSocket(SSLContext* context, in_port_t port)
    throw(IOException, SystemException);
  
  // Creates a server socket and binds it to the specified local port
  // number, with the specified backlog
  SSLServerSocket(SSLContext* context, in_port_t port, int backlog)
    throw(IOException, SystemException);
  
  // Creates a server socket with the specified port, listen backlog,
  // and local IP address to bind to
  SSLServerSocket(SSLContext* context, in_port_t port, int backlog, const InetAddress* addr)
    throw(IOException, SystemException);

  // Destroys the server socket
  virtual ~SSLServerSocket()
    throw(SystemException);

public:
  
  Socket* accept()
    throw(IOException, IllegalBlockingModeException);

  // Closes this socket
  void close()
    throw(IOException);

/*   SSLSocket* accept()  */
/*     throw(IOException, IllegalBlockingModeException); */

  // Returns the names of the cipher suites which could be enabled for use
  // on this connection.  
  const char* getSupportedCipherSuites();

  // Returns the names of the SSL cipher suites which are currently
  // enabled for use on this connection.
  char* getEnabledCipherSuites();

  // Sets the cipher suites enabled for use on this connection
  void setEnabledCipherSuites(const char** suites);
  
  // Returns the names of the protocols which could be enabled for use
  // on an SSL connection.
  char** getSupportedProtocols();

  // Returns the names of the protocol versions which are currently
  // enabled for use on this connection.
  char** getEnabledProtocols();

  // Sets the protocol versions enabled for use on this connection.
  void setEnabledProtocols(const char** protocols);

  // Configures the socket to use client (or server) mode when
  // handshaking.
  void setUseClientMode(bool mode);

  // Returns true if the socket is set to use client mode when
  // handshaking.
  bool getUseClientMode();

  // Configures the socket to <i>require</i> client authentication. 
  void setNeedClientAuth(bool need);

  // Returns true if the socket will <i>require</i> client authentication.
  bool getNeedClientAuth();

  // Configures the socket to <i>request</i> client authentication.
  void setWantClientAuth(bool want);

  // Returns true if the socket will <i>request</i> client authentication. 
  bool getWantClientAuth();

  // Controls whether new SSL sessions may be established by this socket.
  void setEnableSessionCreation(bool flag);

  // Returns true if new SSL sessions may be established by this socket.
  bool getEnableSessionCreation();


};


} //namespace TIDSocket


#endif //__TID_SOCKET_SERVER_SOCKET_H__
