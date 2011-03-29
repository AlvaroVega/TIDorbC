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
