///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_TIDSSLSocket.h
// Description: 
// Rel:         01.00
// Created:     May, 2009
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


#ifndef __TID_SOCKET_SSL_SOCKET_H__
#define __TID_SOCKET_SSL_SOCKET_H__


#include <sys/types.h>
#include <openssl/ssl.h>

using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket {

class SSLInputStream;
class SSLOutputStream;
class SSLSession;


class SSLSocket : public virtual Socket
{

  friend class SSLServerSocket;
  
protected:

  SSLContext* _context;
  SSL*        _ssl;  
  BIO*        _sbio;

  SSLSession* _session;

  bool        _use_client_mode;
  bool        _session_creation;

public:

  // Create an unconnected socket, with the system-default SocketImpl
  SSLSocket(SSLContext* context)
    throw(IOException, SystemException);
  
  // Create a stream socket and connects it to the specified port number
  // at the specified IP address
  SSLSocket(const InetAddress& address, in_port_t port, SSLContext* context)
    throw(IOException, SystemException);
  
  // Create a socket and connects it to the specified remote address on
  // the specified remote port
  SSLSocket(const InetAddress& address,   in_port_t port,
            const InetAddress& localAddr, in_port_t localPort,
            SSLContext* context) 
    throw(IOException, SystemException);
  
  // Create a stream socket and connects it to the specified port number
  // on the named host
  SSLSocket(const char* host, in_port_t port, SSLContext* context)
    throw(UnknownHostException, IOException, SystemException); 
  
  // Create a socket and connects it to the specified remote host on the
  // specified remote port
  SSLSocket(const char* host,             in_port_t port,
            const InetAddress& localAddr, in_port_t localPort,
            SSLContext* context) 
    throw(IOException, SystemException);
  
  // Destroy the socket
  virtual ~SSLSocket()
    throw(SystemException);
  
protected:
  // Create an unconnected socket with a user-specified SocketImpl
  // (caller must NOT delete SocketImpl object)
  SSLSocket(const SocketImpl* impl, SSLContext* context)
    throw(SocketException, SystemException);

  
public:

  void close()
    throw(IOException);
  
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

  // Returns the SSL Session in use by this connection. 
  TIDSocket::SSLSession* getSession();

//   // Registers an event listener to receive notifications that an
//   // SSL handshake has completed on this connection.
//   void addHandshakeCompletedListener(HandshakeCompletedListener listener);

//   // Removes a previously registered handshake completion listener.
//   void removeHandshakeCompletedListener(HandshakeCompletedListener listener);

  // Starts an SSL handshake on this connection.
  void startHandshake() throw(IOException);

  // Tries to renegotiate SSL values (session, keys) on this connection.
  void renegotitate() throw(IOException);

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


  InputStream* getInputStream()
    throw(IOException);

  OutputStream* getOutputStream()
    throw(IOException);

  const char* toString();

  SSLContext* getSSLContext();

};


} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_SOCKET_H__
