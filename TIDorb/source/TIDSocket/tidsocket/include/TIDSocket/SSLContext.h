///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLContext.h
// Description: This file contains the SSLContext class methods
// Rel:         01.00
// Created:     May, 2009
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


#ifndef __TID_SOCKET_SSL_CONTEXT_H__
#define __TID_SOCKET_SSL_CONTEXT_H__


#include <openssl/ssl.h>


using TIDThr::RefCounter;
using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{

class SSLServerSocketFactory;
class SSLSocketFactory;
class SSLSessionContext;


enum SSLProtocol { SSLv2 = 0, SSLv3, SSLv23, TLSv1 };

void ssl_init() 
   throw (SystemException);



//static int verify_cb(int ok, X509_STORE_CTX *ctx);

class SSLContext : public virtual RefCounter
{

/*   friend SSLServerSocketFactory; */
/*   friend SSLSocketFactory; */
  friend void ssl_init () throw (SystemException);

public: 
  SSL_CTX*                _ctx;

private:
  SSLProtocol             _protocol;
  //Provider              _provider; // Default and unique: TIDorb + OpenSSL

  SSLServerSocketFactory* _ssl_server_socket_factory;
  SSLSocketFactory*       _ssl_socket_factory;

  const char*             _password;

  bool                    _context_initialized;

  SSLSessionContext*      _client_session_context;
  SSLSessionContext*      _server_session_context;


  // Library correctly initializated
  static bool              st_initialized;

  
  static SSLContext*      st_ssl_context; // ¿¿!!!!!

  

public:

  // Generates a SSLContext object that implements the specified secure socket protocol. 
  SSLContext( /* SSLContextSpi contextSpi, Provider provider, */
             SSLProtocol protocol)
    throw(IOException, SystemException);
  
  // Destroys the server socket
  virtual ~SSLContext()
    throw(SystemException);
  
public:

  // Returns the client session context, which represents the set of SSL sessions
  // available for use during the handshake phase of client-side SSL sockets.
  SSLSessionContext* getClientSessionContext();


  // Generates a SSLContext object that implements the specified secure socket protocol.
  static SSLContext* getInstance(SSLProtocol protocol)
    throw(NoSuchAlgorithmException);

//   // Generates a SSLContext object that implements the specified secure socket 
//   // protocol from the specified provider.
//   static SSLContext* getInstance(const char* protocol, Provider provider)
//     throw(NoSuchAlgorithmException);

//   // Generates a SSLContext object that implements the specified secure socket 
//   // protocol from the specified provider.
//   static SSLContext* getInstance(const char* protocol, const char* provider)
//     throw(NoSuchAlgorithmException);

  // Returns the protocol name of this SSLContext object.
  //const char* getProtocol();
  SSLProtocol getProtocol();

//   // Returns the provider of this SSLContext object.
//   Provider getProvider();

  // Returns the server session context, which represents the set of SSL sessions 
  // available for use during the handshake phase of server-side SSL sockets.
  SSLSessionContext* getServerSessionContext();



  // Returns a ServerSocketFactory object for this context.
  SSLServerSocketFactory* getServerSocketFactory();

  // Returns a SocketFactory object for this context.
  SSLSocketFactory* getSocketFactory();

  // Initializes this context.
  void init(/* KeyManager[] km, */ 
            /* TrustManager[] tm, */
            /* SecureRandom random */
            const char* cafile,
            const char* keyfile,
            const char* password)
    throw(KeyManagementException);

  // Initializes this context.
  void init(/* KeyManager[] km, */ 
            /* TrustManager[] tm, */
            /* SecureRandom random */
            int timeout,
            const char* cafile,
            const char* keyfile,
            const char* password)
    throw(KeyManagementException);

  // Set verify mode
  void set_verify(int mode);

protected:
  static int verify_cb(int ok, X509_STORE_CTX *ctx);

  int password_cb(char *buf, int num,
                  int rwflag, void *userdata);

};

} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_CONTEXT_H__
