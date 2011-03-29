///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLServerSocket.C
// Description: This file contains the ServerSocket class methods
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
#include "SocketStatus.h"
#include "PlainSocketImpl.h"
#include <sys/types.h>
#include <string.h>
#include <errno.h>
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




namespace TIDSocket
{



//
// Constructor
//
SSLServerSocket::SSLServerSocket(SSLContext* context)
  throw(IOException, SystemException)
  : ServerSocket(), _context(context)
{
  _s = NULL;
  _use_client_mode = false;
  _session_creation = false;
}




//
// Constructor
//
SSLServerSocket::SSLServerSocket(SSLContext* context, in_port_t port)
  throw(IOException, SystemException)
  : ServerSocket(port), _context(context)
{
  _s = NULL;
  _use_client_mode = false;
  _session_creation = false;
}




//
// Constructor
//
SSLServerSocket::SSLServerSocket(SSLContext* context, in_port_t port, int backlog)
  throw(IOException, SystemException)
  : ServerSocket(port, backlog), _context(context)
{
  _s = NULL;
  _use_client_mode = false;
  _session_creation = false;
}




//
// Constructor
//
SSLServerSocket::SSLServerSocket(SSLContext* context, in_port_t port, int backlog, 
                                 const InetAddress* addr)
  throw(IOException, SystemException)
  : ServerSocket(port, backlog, addr), _context(context)
{
  _s = NULL;
  _use_client_mode = false;
  _session_creation = false;
}




//
// Destructor
//
SSLServerSocket::~SSLServerSocket()
    throw(SystemException)
{
//   if (_s) 
//     delete _s;
}


//SSLSocket* SSLServerSocket::accept() 
Socket* SSLServerSocket::accept() 
  throw(IOException, IllegalBlockingModeException)
{
//   // ---------------------- NOT NECESSARY BY THE MOMENT ???----------------
//   DH *ret=0;
//   BIO *bio;
    
//   if ((bio = BIO_new_file("dh1024.pem", "r")) == NULL)
//     cerr << "[SSLServerSocket] Couldn't open DH file" << endl;
 
//   ret = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);

//   BIO_free(bio);

//   if(SSL_CTX_set_tmp_dh(_context->_ctx, ret) < 0)
//     cerr << "[SSLServerSocket] Couldn't set DH parameters" << endl;
//   // -------------------------------------------------------------



  SSLSocket* s = new SSLSocket(new PlainSocketImpl, _context);


  //  Synchronized synchronized(_sync);
  {

    // Check socket availability
    s->_impl->_fd = ::accept((int)_impl->_fd , 0, 0);
    
    if( s->_impl->_fd < 0) {
      delete s;
      throw IOException("Problem accepting", errno);
    }
    if( s->_impl->_fd == 0) {
      throw IOException("TIDSocket ERROR _fd is null");
    }
    // Set socket as connected
    s->setConnected(); // TODO: core when client restart a connection

    s->_sbio = BIO_new_socket(s->_impl->_fd, BIO_NOCLOSE);
    SSL_set_bio(s->_ssl, s->_sbio, s->_sbio);
    
    //SSL_set_accept_state(s->_ssl);

    int ret = SSL_accept(s->_ssl); 

    if( ret <= 0) {

      int ssl_error = SSL_get_error(s->_ssl, ret);
      int error = ERR_get_error();

      OSTRINGSTREAM reason;
      reason << "SSLServerSocket::accept error: ";

      switch (ssl_error) {
      case SSL_ERROR_NONE:
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

    // Store accepted sockets for future operations??
    
//     if (_s != NULL) 
//       delete _s; // BUG
    
    _s = s;

  }
  
  if (!_session_creation)
    s->setEnableSessionCreation(false);
    
  return s;
 
}

void SSLServerSocket::close()
  throw(IOException)
{
    try
    {
        // Cierra el socket
//         if (_s) {
//           _s->close();
//         }  

    }
    catch(TIDThr::Exception& e)
    {
        throw IOException(e.what());
    }
}


// Returns the names of the cipher suites which could be enabled for use
// on this connection.  
const char* SSLServerSocket::getSupportedCipherSuites()
{
  Synchronized synchronized(_sync);

  if (_s != NULL) 
    return SSL_get_cipher_list(_s->_ssl, 0);
  else
    return NULL;
}



// Returns the names of the SSL cipher suites which are currently
// enabled for use on this connection.
char* SSLServerSocket::getEnabledCipherSuites()
{
  Synchronized synchronized(_sync);

  if (_s != NULL) {
    char cipdesc[128];
    SSL_CIPHER *sslciph = SSL_get_current_cipher(_s->_ssl);
    
    SSL_CIPHER_description(sslciph, cipdesc, sizeof(cipdesc));
    
    return strdup(cipdesc);
  }
  else 
    return NULL;
}


// Sets the cipher suites enabled for use on this connection
void SSLServerSocket::setEnabledCipherSuites(const char** suites)
{
  // TODO
  // SSL_CTX_set_cipher_list  SSL_set_cipher_list
} 

  
// Returns the names of the protocols which could be enabled for use
// on an SSL connection.
char** SSLServerSocket::getSupportedProtocols()
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
char** SSLServerSocket::getEnabledProtocols()
{
  SSLProtocol _protocol = _context->getProtocol();
  
  char** protocols;

  protocols = (char**) malloc(sizeof(char**));

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
void SSLServerSocket::setEnabledProtocols(const char** protocols)
{
  // TODO: _context->setProtocols(protocols);
}


// Configures the socket to use client (or server) mode when
// handshaking.
void SSLServerSocket::setUseClientMode(bool mode)
{
  _use_client_mode = mode;
}


// Returns true if the socket is set to use client mode when
// handshaking.
bool SSLServerSocket::getUseClientMode()
{
  return _use_client_mode;
}


// Configures the socket to <i>require</i> client authentication. 
void SSLServerSocket::setNeedClientAuth(bool need)
{
  // TODO
}


// Returns true if the socket will <i>require</i> client authentication.
bool SSLServerSocket::getNeedClientAuth()
{
  // TODO
  return false;
}


// Configures the socket to <i>request</i> client authentication.
void SSLServerSocket::setWantClientAuth(bool want)
{
  // TODO
}


// Returns true if the socket will <i>request</i> client authentication. 
bool SSLServerSocket::getWantClientAuth()
{
  // TODO
  return false;
}


// Controls whether new SSL sessions may be established by this socket.
void SSLServerSocket::setEnableSessionCreation(bool flag)
{
  _session_creation = flag;
}


// Returns true if new SSL sessions may be established by this socket.
bool SSLServerSocket::getEnableSessionCreation()
{
  return _session_creation;
}






} //namespace TIDSocket
