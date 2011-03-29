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
