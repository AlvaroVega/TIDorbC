///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSession.C
// Description: This file contains the SSLSession class methods
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
#include "SocketStatus.h"
#include "PlainSocketImpl.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>




using TIDThr::Synchronized;


namespace TIDSocket {


//
// Constructor
//
SSLSession::SSLSession(SSLContext* context,
                       const SSL* ssl, 
                       const char* peer_host, int peer_port)
  throw(IOException, SystemException)

{
  _context = context;
  _ssl = ssl;
#if (defined OPENSSL_VERSION_NUMBER >= 0x00908000L)
  _session = SSL_get_session(ssl);
#else
  _session = SSL_get_session((SSL*)ssl);
#endif
  _peer_host = strdup(peer_host);
  _peer_port = peer_port;
}



SSLSession::~SSLSession()
  throw(SystemException)
{
  SSL_SESSION_free(_session);
  free(_peer_host);
}


// Gets the size of the largest application buffer that may occur when using 
// this session.
int SSLSession::getApplicationBufferSize()
{
  // TODO
  return 0;
}


// Returns the name of the SSL cipher suite which is used for all connections 
// in the session.
const char* SSLSession::getCipherSuite()
{
  // TODO
  return SSL_CIPHER_get_name(_session->cipher);
}


// Returns the time at which this Session representation was created, in 
// milliseconds since midnight, January 1, 1970 UTC.
long SSLSession::getCreationTime()
{
  return SSL_SESSION_get_time(_session);
}


// Returns the identifier assigned to this Session.
SessionId SSLSession::getId()
{
  char* id_session = (char*) malloc((_session->session_id_length)+1);
//   cerr << "SSLSession: id_length"  << _session->session_id_length << endl;
//   cerr << "SSLSession: id"  << _session->session_id[0] << endl;
//   cerr << "SSLSession: id"  << _session->session_id[1] << endl;
//   cerr << "SSLSession: id"  << _session->session_id[2] << endl;
//   cerr << "SSLSession: id"  << _session->session_id[3] << endl;
  memcpy(id_session, _session->session_id, _session->session_id_length);
  id_session[_session->session_id_length] = '\0';
  
  string res(id_session);
  
  free(id_session);

  return res;
}

  
// Returns the last time this Session representation was accessed by the 
// session level infrastructure, in milliseconds since midnight, January 1, 1970 UTC.
long SSLSession::getLastAccessedTime()
{
  return 0;
}


//   // Returns the certificate(s) that were sent to the peer during handshaking.
//   Certificate[] getLocalCertificates()
// {

// }


//    // Returns the principal that was sent to the peer during handshaking.
//   Principal getLocalPrincipal()
// {

// }


// Gets the size of the largest SSL/TLS packet that may occur when using this session.
int SSLSession::getPacketBufferSize()
{
  // TODO
  return 0; // _ssl;
}


// // Returns the identity of the peer which was identified as part of defining the session.
// X509Certificate[] getPeerCertificateChain()
// {
  
  //  TODO: SSL_get_peer_certificate

// }


// // Returns the identity of the peer which was established as part of defining the session.
// Certificate[] getPeerCertificates()
// {
//   X509_NAME_print(_session->peer);

  //  TODO: SSL_get_peer_certificate

// }


// Returns the host name of the peer in this session.
const char* SSLSession::getPeerHost()
{
  return (const char*)_peer_host;
}


// Returns the port number of the peer in this session.
int SSLSession::getPeerPort()
{
  return _peer_port;
}


// // Returns the identity of the peer which was established as part of defining the session.
// Principal SSLSession::getPeerPrincipal()
// {
  
// }


// Returns the standard name of the protocol used for all connections in the session.
const char* SSLSession::getProtocol()
{
  // TODO
  return NULL;
}


//    // Returns the context in which this session is bound.
//   SSLSessionContext getSessionContext()
// {

// }


//    // Returns the object bound to the given name in the session's application layer data.
//   Object getValue(String name)
// {

// }


// Returns an array of the names of all the application layer data objects bound into the Session.
char** SSLSession::getValueNames()
{
  // TODO
  return NULL;
}


// Invalidates the session.
void SSLSession::invalidate()
{
  // TODO
}


// Returns whether this session is valid and available for resuming or joining.
bool SSLSession::isValid()
{
  // TODO
  return true;
}


// // Binds the specified value object into the session's application layer data with the given name.
// void SSLSession::putValue(String name, Object value)
// {
//   // TODO
// }


// // Removes the object bound to the given name in the session's application layer data.
// void SSLSession::removeValue(String name)
// {
//   // TODO
// }

void SSLSession::setTimeout(int seconds)
{
  SSL_SESSION_set_timeout(_session, seconds);
}



} // TIDSocket
