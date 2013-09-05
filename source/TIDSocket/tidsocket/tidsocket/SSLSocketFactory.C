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
