///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLContext.C
// Description: This file contains the SSLContext class methods
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
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>



using TIDThr::Synchronized;

static void sigpipe_handle(int x){
}


namespace TIDSocket {


SSLContext* SSLContext::st_ssl_context(NULL);  // TODO: remove

bool SSLContext::st_initialized(false);

void ssl_init()
  throw (SystemException)
{
  if(SSLContext::st_initialized) {
    return;
  }
  
  /* Global system initialization */
  SSL_library_init();   // idem: OpenSSL_add_all_algorithms();
  SSL_load_error_strings();

  SSLContext::st_initialized = true;

}




SSLContext::SSLContext( /* SSLContextSpi contextSpi, Provider provider, */
                       SSLProtocol protocol)
  throw (IOException, SystemException)
{

  if(!st_initialized) {
    throw SystemException("SSLContext is not initialized");
  }

  /* Set up a SIGPIPE handler */
  signal(SIGPIPE,sigpipe_handle);

  SSL_METHOD *meth; 

  _protocol = protocol;
  
  switch (protocol) {
  case SSLv2: 
    meth = SSLv2_method();
    break;
  case SSLv3: 
    meth = SSLv3_method();
    break;
  case SSLv23: 
    meth = SSLv23_method(); // Compatible TAO & MICO
    break;
  case TLSv1: 
    meth = TLSv1_method(); // No backward compability
    break;
    
  default:
    //throw NO_IMPLEMENT();
    meth = NULL;
    throw NoSuchAlgorithmException();
    break;
  }


  /* Create our context */
  _ctx = SSL_CTX_new(meth);   

  _ssl_server_socket_factory = new SSLServerSocketFactory(this);
  _ssl_socket_factory = new SSLSocketFactory(this);

  _context_initialized = false;

  _server_session_context = new SSLSessionContext();
  _client_session_context = new SSLSessionContext();

}
  
// Destroys the server socket
SSLContext::~SSLContext()
  throw(SystemException)
{
  SSL_CTX_free(_ctx);

  delete _server_session_context;
  delete _client_session_context;

  //free(_protocol);
}


// Returns the client session context, which represents the set of SSL sessions
// available for use during the handshake phase of client-side SSL sockets.
SSLSessionContext* SSLContext::getClientSessionContext()
{
  return _client_session_context;
}

// Generates a SSLContext object that implements the specified secure socket protocol.
SSLContext* SSLContext::getInstance(SSLProtocol protocol)
  throw (NoSuchAlgorithmException)
{
  try {
    if (st_ssl_context == NULL)
      st_ssl_context = new SSLContext(protocol);
    return st_ssl_context;
  } catch (const IOException& ex) {
    throw NoSuchAlgorithmException("SSLContext instance doesn't know protocol");
  }
}

// // Generates a SSLContext object that implements the specified secure socket 
// // protocol from the specified provider.
// static SSLContext* SSLContext::getInstance(const char* protocol, Provider provider)
//   throw(NoSuchAlgorithmException)
// {

// }

// // Generates a SSLContext object that implements the specified secure socket 
// // protocol from the specified provider.
// static SSLContext* SSLContext::getInstance(const char* protocol, const char* provider)
//   throw(NoSuchAlgorithmException)
// {

// } 

// Returns the protocol name of this SSLContext object.
//const char* SSLContext::getProtocol()
SSLProtocol SSLContext::getProtocol()
{
  return _protocol;
}

// // Returns the provider of this SSLContext object.
// Provider SSLContext::getProvider()
// {

// }

// Returns the server session context, which represents the set of SSL sessions 
// available for use during the handshake phase of server-side SSL sockets.
SSLSessionContext* SSLContext::getServerSessionContext()
{
  return _server_session_context;
}

// Returns a ServerSocketFactory object for this context.
SSLServerSocketFactory* SSLContext::getServerSocketFactory()
{
  return _ssl_server_socket_factory;
}

// Returns a SocketFactory object for this context.
SSLSocketFactory* SSLContext::getSocketFactory()
{
  return _ssl_socket_factory;
}


// Initializes this context.
void SSLContext::init(/* KeyManager[] km,*/ 
                      /* TrustManager[] tm, */
                      /* SecureRandom random */ 
                      const char* cafile,
                      const char* keyfile,
                      const char* password)
  throw(KeyManagementException)
{
  // TODO
  if(!st_initialized) {
    throw SystemException("SSLContext is not initialized");
  }

  if (_context_initialized)
    return;

  // TODO: try to improve: cache(static) with all possible methods??
//   // ���Some static Context ???
//   st_ssl_context = new SSLContext();

  //if(!(SSL_CTX_use_certificate_chain_file(_ctx, keyfile)))
  if(!(SSL_CTX_use_certificate_file(_ctx, keyfile, SSL_FILETYPE_PEM))) {
    throw KeyManagementException("SSLContext: Can't read certificate file or not valid");
  }

//   _password = password;
//   //SSL_CTX_set_default_passwd_cb(_ctx, password_cb);
//   SSL_CTX_set_default_passwd_cb_userdata(_ctx, _password);

  if(!(SSL_CTX_use_PrivateKey_file(_ctx, password, SSL_FILETYPE_PEM)) ) {
    throw KeyManagementException("SSLContext: Can't read key file or not valid");
  }


//   /* Set our cipher list */
//   if(ciphers){
//     SSL_CTX_set_cipher_list(ctx,ciphers);
//   }
  
  if ( strcmp(cafile, "") != 0) {
    if (!SSL_CTX_load_verify_locations(_ctx, cafile, ".")) {
      throw KeyManagementException("SSLContext: Can't read ca file or not valid");
    }
  }

  _context_initialized = true;

}


// Initializes this context.
void SSLContext::init(/* KeyManager[] km,*/ 
                      /* TrustManager[] tm, */
                      /* SecureRandom random */ 
                      int timeout,
                      const char* cafile,
                      const char* keyfile,
                      const char* password)
  throw(KeyManagementException)
{
  
  init(cafile, keyfile, password);

  SSL_CTX_set_timeout(_ctx, timeout);
}


void SSLContext::set_verify(int mode)
{
  //SSL_CTX_set_verify(_ctx, mode, NULL);
  SSL_CTX_set_verify(_ctx, mode, SSLContext::verify_cb);
}

int SSLContext::password_cb(char *buf, int num,
                            int rwflag, void *userdata)
{
//   if (num < strlen(_password)+1)
//     return(0);

//   strcpy(buf, _password);
//   return(strlen(_password));
  return 0;
}

int SSLContext::verify_cb(int ok, X509_STORE_CTX *ctx) 
{ 
  cerr << "SSLContext::verify_cb" << endl;
  return 1; 


//   char    buf[256];
//   X509   *err_cert;
//   int     err, depth;
//   SSL    *ssl;
//   mydata_t *mydata;
  
//   err_cert = X509_STORE_CTX_get_current_cert(ctx);
//   err = X509_STORE_CTX_get_error(ctx);
//   depth = X509_STORE_CTX_get_error_depth(ctx);
  
//   /*
//    * Retrieve the pointer to the SSL of the connection currently treated
//    * and the application specific data stored into the SSL object.
//    */
//   ssl = X509_STORE_CTX_get_ex_data(ctx, SSL_get_ex_data_X509_STORE_CTX_idx());
//   mydata = SSL_get_ex_data(ssl, mydata_index);
  
//   X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);
  
//   /*
//    * Catch a too long certificate chain. The depth limit set using
//    * SSL_CTX_set_verify_depth() is by purpose set to "limit+1" so
//    * that whenever the "depth>verify_depth" condition is met, we
//    * have violated the limit and want to log this error condition.
//    * We must do it here, because the CHAIN_TOO_LONG error would not
//    * be found explicitly; only errors introduced by cutting off the
//    * additional certificates would be logged.
//    */
//   if (depth > mydata->verify_depth) {
//     preverify_ok = 0;
//     err = X509_V_ERR_CERT_CHAIN_TOO_LONG;
//     X509_STORE_CTX_set_error(ctx, err);
//   }
//   if (!preverify_ok) {
//     printf("verify error:num=%d:%s:depth=%d:%s\n", err,
//            X509_verify_cert_error_string(err), depth, buf);
//   }
//   else if (mydata->verbose_mode) {
//     printf("depth=%d:%s\n", depth, buf);
//   }
  
//   /*
//    * At this point, err contains the last verification error. We can use
//    * it for something special
//    */
//   if (!preverify_ok && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT)) {
//     X509_NAME_oneline(X509_get_issuer_name(ctx->current_cert), buf, 256);
//     printf("issuer= %s\n", buf);
//   }
  
//   if (mydata->always_continue)
//     return 1;
//   else
//     return preverify_ok;
  

}


} //namespace TIDSocket
