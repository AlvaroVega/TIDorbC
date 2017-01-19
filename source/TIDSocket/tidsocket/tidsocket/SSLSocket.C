///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSocket.C
// Description: This file contains the SSLSocket class methods
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
#include <openssl/err.h>

#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES

#include <sstream>
#define OSTRINGSTREAM std::ostringstream
using std::ends;

#else

#include <strstream.h>
#define OSTRINGSTREAM ostrstream

#endif


using TIDThr::Synchronized;




namespace TIDSocket {


//
// Constructor
//
SSLSocket::SSLSocket(SSLContext* context)
  throw(IOException, SystemException)
  : Socket(), _context(context), _session(NULL)
{
  // TODO: check if context is != NULL

  /* Connect the SSL socket */
  _ssl  = SSL_new(_context->_ctx);
  _sbio = BIO_new_socket(_impl->_fd, BIO_NOCLOSE);
  SSL_set_bio(_ssl, _sbio, _sbio);

  //SSL_set_connect_state(_ssl);

  // Verify certificate (TODO previos neeeds SSL_CTX_load_verify_locations)
  if (SSL_get_verify_result(_ssl) != X509_V_OK) {
    throw IOException("SSLSocket: Certificate doesn't verified");
  }

  _session_creation = true;
  _use_client_mode = true;
}




//
// Constructor
//
SSLSocket::SSLSocket(const InetAddress& address, in_port_t port, SSLContext* context)
  throw(IOException, SystemException)
  : Socket(address, port), _context(context), _session(NULL)
{
  // TODO: check if context is != NULL

  /* Connect the SSL socket */
  _ssl  = SSL_new(_context->_ctx);
  _sbio = BIO_new_socket(_impl->_fd, BIO_NOCLOSE);
  SSL_set_bio(_ssl, _sbio, _sbio);

  //SSL_set_connect_state(_ssl);

  // Verify certificate
  if (SSL_get_verify_result(_ssl) != X509_V_OK) {
    throw IOException("SSLSocket: Certificate doesn't verified");
  }

  _session_creation = true;
  _use_client_mode = true;
}




//
// Constructor
//
SSLSocket::SSLSocket(const InetAddress& address,   in_port_t port,
                     const InetAddress& localAddr, in_port_t localPort,
                     SSLContext* context)
  throw(IOException, SystemException)
  : Socket(address, port, localAddr, localPort), _context(context), _session(NULL)
{
  // TODO: check if context is != NULL

  /* Connect the SSL socket */
  _ssl  = SSL_new(_context->_ctx);
  _sbio = BIO_new_socket(_impl->_fd, BIO_NOCLOSE);
  SSL_set_bio(_ssl, _sbio, _sbio);

  //SSL_set_connect_state(_ssl);

  // Verify certificate
  if (SSL_get_verify_result(_ssl) != X509_V_OK) {
    throw IOException("SSLSocket: Certificate doesn't verified");
  }

  _session_creation = true;
  _use_client_mode = true;
}




//
// Constructor
//
SSLSocket::SSLSocket(const char* host, in_port_t port, SSLContext* context)
  throw(UnknownHostException, IOException, SystemException)
  : Socket(host, port), _context(context), _session(NULL)
{
  // TODO: check if context is != NULL

  /* Connect the SSL socket */
  _ssl  = SSL_new(_context->_ctx);
  _sbio = BIO_new_socket(_impl->_fd, BIO_NOCLOSE); 
  SSL_set_bio(_ssl, _sbio, _sbio);

  //SSL_set_connect_state(_ssl);

  // Verify certificate
  if (SSL_get_verify_result(_ssl) != X509_V_OK) {
    throw IOException("SSLSocket: Certificate doesn't verified");
  }

  _session_creation = true;
  _use_client_mode = true;
}




//
// Constructor
//
SSLSocket::SSLSocket(const char* host, in_port_t port,
                     const InetAddress& localAddr, in_port_t localPort,
                     SSLContext* context)
  throw(IOException, SystemException)
  : Socket(host, port, localAddr, localPort), _context(context), _session(NULL)
{
  // TODO: check if context is != NULL

  /* Connect the SSL socket */
  _ssl  = SSL_new(_context->_ctx);
  _sbio = BIO_new_socket(_impl->_fd, BIO_NOCLOSE);
  SSL_set_bio(_ssl, _sbio, _sbio);

  //SSL_set_connect_state(_ssl);

  // Verify certificate
  if (SSL_get_verify_result(_ssl) != X509_V_OK) {
    throw IOException("SSLSocket: Certificate doesn't verified");
  }

  _session_creation = true;
  _use_client_mode = true;
}




//
// Destructor
//
SSLSocket::~SSLSocket()
  throw(SystemException)
{
  // TODO:
  SSL_free(_ssl); // deleted by somebody
  //BIO_free(_sbio);
}


// Create an unconnected socket with a user-specified SocketImpl
// (caller must NOT delete SocketImpl object)
SSLSocket::SSLSocket(const SocketImpl* impl, SSLContext* context)
  throw(SocketException, SystemException)
  : Socket(impl), _context(context), _session(NULL)
{
  // TODO: check if context is != NULL

  /* Connect the SSL socket */
  _ssl  = SSL_new(_context->_ctx);

  // Verify certificate
  if (SSL_get_verify_result(_ssl) != X509_V_OK) {
    throw IOException("SSLSocket: Certificate doesn't verified");
  }

  _session_creation = true;
  _use_client_mode = true;
}



//
// close()
//
void SSLSocket::close()
    throw(IOException)
{
    try
    {
        Synchronized synchronized(_sync);
        {
            // Cierra el socket y anula todos los flags de estado excepto CLOSED
            _impl->close();
            _status = TID_SOCKET_STATUS_CLOSED;
        }
    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}





// Returns the names of the cipher suites which could be enabled for use
// on this connection.  
const char* SSLSocket::getSupportedCipherSuites()
{
  
  return SSL_get_cipher_list(_ssl, 0);
}



// Returns the names of the SSL cipher suites which are currently
// enabled for use on this connection.
char* SSLSocket::getEnabledCipherSuites()
{
  char cipdesc[128];
  const SSL_CIPHER *sslciph = SSL_get_current_cipher(_ssl);
  
  if (sslciph != NULL) {
    SSL_CIPHER_description(sslciph, cipdesc, sizeof(cipdesc));
    
    return strdup(cipdesc);
  }
  else
    return NULL;

}


// Sets the cipher suites enabled for use on this connection
void SSLSocket::setEnabledCipherSuites(const char** suites)
{
  // TODO
  // SSL_CTX_set_cipher_list  SSL_set_cipher_list
}

  
// Returns the names of the protocols which could be enabled for use
// on an SSL connection.
char** SSLSocket::getSupportedProtocols()
{
  char** protocols;
  
  protocols = (char**) malloc(sizeof(char**));
  *protocols = (char*) malloc(sizeof(char*)*4);
  protocols[0] = strdup("SSLv2");
  protocols[1] = strdup("SSLv3");
  protocols[2] = strdup("SSLv23");
  protocols[3] = strdup("TLSv1");
  return protocols;
}


// Returns the names of the protocol versions which are currently
// enabled for use on this connection.
char** SSLSocket::getEnabledProtocols()
{
  SSLProtocol _protocol = _context->getProtocol();
  
  char** protocols;

  protocols = (char**) malloc(sizeof(char**));
  *protocols = (char*) malloc(sizeof(char*));

  switch (_protocol) {
  case SSLv2: 
    protocols[0] = strdup("SSLv2");
    break;
  case SSLv3: 
    protocols[0] = strdup("SSLv3");
    break;
  case SSLv23: 
    protocols[0] = strdup("SSLv23");
    break;
  case TLSv1: 
    protocols[0] = strdup("TLSv1");
    break;
  default: 
    protocols[0] = strdup("");
  }

  return protocols;
}


// Sets the protocol versions enabled for use on this connection.
void SSLSocket::setEnabledProtocols(const char** protocols)
{
  // TODO: _context->setProtocols(protocols);
}


// Returns the SSL Session in use by this connection. 
TIDSocket::SSLSession* SSLSocket::getSession()
{

  if ( (_session == NULL) && (_session_creation) ) {

    Synchronized synchronized(_sync);

    TIDSocket::InetSocketAddress* inet = 
      ((TIDSocket::InetSocketAddress*)getRemoteSocketAddress());

    _session = new TIDSocket::SSLSession(_context, _ssl, inet->getHostName(), getPort());
    
    _context->getClientSessionContext()->addSession(_session->getId(), _session);
  
    delete inet;
  }

  return _session;
}


// // Registers an event listener to receive notifications that an
// // SSL handshake has completed on this connection.
// void SSLSocket::addHandshakeCompletedListener(HandshakeCompletedListener listener)
// {

// }


// // Removes a previously registered handshake completion listener.
// void SSLSocket::removeHandshakeCompletedListener(HandshakeCompletedListener listener)
// {

// }


// Starts an SSL handshake on this connection.
void SSLSocket::startHandshake() 
  throw(IOException)
{
  Synchronized synchronized(_sync);

  // TODO: 
  if (_session != NULL)
    SSL_set_session(_ssl, _session->_session);
  

  // TODO: add some checks...
  if (!_use_client_mode)
    return;

  int ret = SSL_connect(_ssl);

  if (ret != 1) {
  
    int ssl_error = SSL_get_error(_ssl, ret);
    int error = ERR_get_error();
    
    OSTRINGSTREAM reason;
    reason << "SSLSocket::startHandshake connect error: ";
  
    switch (ssl_error) { 
      reason << "SSL_ERROR_NONE reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_READ:
      reason << "SSL_ERROR_WANT_READ reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_WRITE:
      reason << "SSL_ERROR_WANT_WRITE reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_ZERO_RETURN:
      reason << "SSL_ERROR_ZERO_RETURN reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_CONNECT:
      reason << "SSL_ERROR_WANT_CONNECT reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_WANT_X509_LOOKUP:
      reason << "SSL_ERROR_WANT_X509_LOOKUP reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_SYSCALL:
      reason << "SSL_ERROR_SYSCALL reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    case SSL_ERROR_SSL:
      reason << "SSL_ERROR_SSL reason: " << ERR_reason_error_string(error) << ends;
      throw IOException(reason.str().c_str());
      break;
    default:
      reason << "unknown: " << ends;
      throw IOException(reason.str().c_str());
    }
  }

//   if (!SSL_session_reused(_ssl))
//     cerr << " new session negotiated" << endl;

  //check_cert(ssl,host);

  
}

// Tries to renegotiate SSL values (session, keys) on this connection.
void SSLSocket::renegotitate() throw(IOException)
{

  /* assume ssl is connected and error free up to here */
  //set_blocking(ssl); /* this is unnecessary if it is already blocking  */

  SSL_renegotiate(_ssl);

  SSL_do_handshake(_ssl);
  // if (_ssl->state != SSL_ST_OK)
  //   throw IOException("SSLSocket: Failed to send renegotiation request");

  // _ssl->state |= SSL_ST_ACCEPT;
  SSL_do_handshake(_ssl);

  // if (_ssl->state != SSL_ST_OK)
  //   throw IOException("SSLSocket: Failed to complete renegotiation");
  /* our renegotiation is complete */
  
}



// Configures the socket to use client (or server) mode when
// handshaking.
void SSLSocket::setUseClientMode(bool mode)
{
  _use_client_mode = mode;
}


// Returns true if the socket is set to use client mode when
// handshaking.
bool SSLSocket::getUseClientMode()
{
  return _use_client_mode;
}


// Configures the socket to <i>require</i> client authentication. 
void SSLSocket::setNeedClientAuth(bool need)
{
  // TODO
}


// Returns true if the socket will <i>require</i> client authentication.
bool SSLSocket::getNeedClientAuth()
{
  // TODO
  return false;
}


// Configures the socket to <i>request</i> client authentication.
void SSLSocket::setWantClientAuth(bool want)
{
  // TODO
}


// Returns true if the socket will <i>request</i> client authentication. 
bool SSLSocket::getWantClientAuth()
{
  // TODO
  return false;
}


// Controls whether new SSL sessions may be established by this socket.
void SSLSocket::setEnableSessionCreation(bool flag)
{
  _session_creation = flag;
}


// Returns true if new SSL sessions may be established by this socket.
bool SSLSocket::getEnableSessionCreation()
{
  return _session_creation;
}


//
// getInputStream()
//
InputStream* SSLSocket::getInputStream()
  throw(IOException)
{
  if (! isConnected()) {
    throw IOException("SSLSocket not connected");
  }

  InputStream* stream = NULL;
  
  try {
    Synchronized synchronized(_sync);

    stream = new SSLInputStream((int) _impl->_fd, _impl->_timeout, _ssl);
    stream->_channel = _channel;

  } catch(TIDThr::Exception& e) {
    throw IOException(e.what());
  }
  return stream;
}


//
// getoutputStream()
//
OutputStream* SSLSocket::getOutputStream()
  throw(IOException)
{
  if (! isConnected()) {
    throw IOException("SSLSocket not connected");
  }

  OutputStream* stream = NULL;
  
  try {
    Synchronized synchronized(_sync);

    stream = new SSLOutputStream((int) _impl->_fd, _impl->_timeout, _ssl, _sbio);
    stream->_channel = _channel;

  } catch(TIDThr::Exception& e) {
    throw IOException(e.what());
  }
  return stream;
}


//
// toString()
//
const char* SSLSocket::toString()
{
  
  return SSL_state_string_long(_ssl);
}


SSLContext* SSLSocket::getSSLContext()
{
  return _context;
}


} //namespace TIDSocket
