///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLSocketFactory.C
// Description: This file contains the SSLSocketFactory class methods
// Rel:         01.00
// Created:     May, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es )
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
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>




using TIDThr::Synchronized;




namespace TIDSocket {




//
// Socket class static members
//
SSLSocketFactory* SSLSocketFactory::st_default_ssl_socket_factory(NULL);




//
// Constructor
//
SSLSocketFactory::SSLSocketFactory(SSLContext* context)
  throw(IOException, SystemException)
  : _context(context)
{

}




//
// Destructor
//
SSLSocketFactory::~SSLSocketFactory()
  throw(SystemException)
{
  delete _context;
}


SSLSocketFactory* SSLSocketFactory::getDefault()
{
  if (st_default_ssl_socket_factory == NULL) {
    // Ddefine the default SSLContext
    SSLContext* _default_context  = new SSLContext(SSLv23);
    st_default_ssl_socket_factory = new SSLSocketFactory(_default_context);
  }

  return st_default_ssl_socket_factory;
}

char** SSLSocketFactory::getDefaultCipherSuites()
{
  // TODO: get it from _context?
  return NULL;
}

char** SSLSocketFactory::getSupportedCipherSuites()
{
  // TODO: get it from _context?
  return NULL;
}

SSLSocket* SSLSocketFactory::createSocket(const char* host, int port)
  throw (IOException)
{
  return new SSLSocket(host, port, _context);
}



} //namespace TIDSocket
