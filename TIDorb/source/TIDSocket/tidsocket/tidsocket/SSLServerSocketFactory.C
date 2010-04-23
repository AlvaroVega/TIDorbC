///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLServerSocketFactory.C
// Description: This file contains the SSLServerSocketFactory class methods
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
SSLServerSocketFactory* SSLServerSocketFactory::st_default_ssl_server_socket_factory(NULL);




//
// Constructor
//
SSLServerSocketFactory::SSLServerSocketFactory(SSLContext* context)
  throw(IOException, SystemException)
  : _context(context)
{

}




//
// Destructor
//
SSLServerSocketFactory::~SSLServerSocketFactory()
  throw(SystemException)
{
  delete _context;
}


SSLServerSocketFactory* SSLServerSocketFactory::getDefault()
{
  if (st_default_ssl_server_socket_factory == NULL) {
    // Ddefine the default SSLContext
    SSLContext* _default_context  = new SSLContext(SSLv23);
    st_default_ssl_server_socket_factory = new SSLServerSocketFactory(_default_context);
  }

  return st_default_ssl_server_socket_factory;
}

char** SSLServerSocketFactory::getDefaultCipherSuites()
{
  // TODO: get it from _context?
  return NULL;
}

char** SSLServerSocketFactory::getSupportedCipherSuites()
{
  // TODO: get it from _context?
  return NULL;
}


SSLServerSocket* 
SSLServerSocketFactory::createServerSocket()
  throw (IOException)
{
  return new SSLServerSocket(_context);
}

SSLServerSocket* 
SSLServerSocketFactory::createServerSocket(int port)
  throw (IOException)
{
  return new SSLServerSocket(_context, port);
}


SSLServerSocket* 
SSLServerSocketFactory::createServerSocket(int port, int backlog)
  throw (IOException)
{
  return new SSLServerSocket(_context, backlog, port);
}


SSLServerSocket* 
SSLServerSocketFactory::createServerSocket(int port, int backlog,
                                           const InetAddress* addr)
  throw (IOException)
{
  return new SSLServerSocket(_context, port, backlog, addr);
}


} //namespace TIDSocket
