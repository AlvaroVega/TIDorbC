/////////////////////////////////////////////////////////////////////////
//
// File:        PortableServer.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "CORBA.h"
#include "PortableServer.h"


namespace PortableServer
{


char* ObjectId_to_string (const ObjectId& oid)
{
  char *str = new char[oid.length()+1];

  for (CORBA::ULong i = 0; i < oid.length(); i++) {
    if (oid[i]=='\0'){
      free(str);
      throw CORBA::BAD_PARAM();
    }
    str[i] = (char) oid[i]; 
  }
  str[oid.length()] = '\0';
  return str;
}


CORBA::WChar* ObjectId_to_wstring(const ObjectId& oid)
{
    return NULL;
}


ObjectId* string_to_ObjectId(const char* str)
{
  //   if (str == NULL) 
  //     throw CORBA::BAD_PARAM();

  PortableServer::ObjectId *oid = new PortableServer::ObjectId;
  CORBA::ULong len = strlen(str);
  oid->length(len);
  
  for (CORBA::ULong i = 0; i < len; i++) {
    (*oid)[i] = (CORBA::Octet)str[i];
  }
  return oid;
}


ObjectId* wstring_to_ObjectId(const CORBA::WChar* str)
{
    return NULL;
}


} //namespace PortableServer
