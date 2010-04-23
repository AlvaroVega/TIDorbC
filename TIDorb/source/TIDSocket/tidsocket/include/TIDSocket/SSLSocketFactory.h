///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSocketFactory.h
// Description: This file contains the SSLSocketFactory class methods
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


#ifndef __TID_SOCKET_SSL_SOCKET_FACTORY_H__
#define __TID_SOCKET_SSL_SOCKET_FACTORY_H__


namespace TIDSocket
{

class SSLSocket;
class SSLContext;


class SSLSocketFactory
{


private:

  static SSLSocketFactory* st_default_ssl_socket_factory;

  SSLContext* _context;

public:

  SSLSocketFactory(SSLContext* context)
    throw(IOException, SystemException);

  virtual ~SSLSocketFactory()
    throw(SystemException);

  static SSLSocketFactory* getDefault();

  char** getDefaultCipherSuites();

  char** getSupportedCipherSuites();

  SSLSocket* createSocket(const char* host, int port)
    throw (IOException);
  
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SSL_SOCKET_FACTORY_H__
