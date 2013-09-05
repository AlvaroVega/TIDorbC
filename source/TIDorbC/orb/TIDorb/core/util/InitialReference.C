/////////////////////////////////////////////////////////////////////////
//
// File:        InitalReference.C
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

#include "TIDorb/core/util.h"


TIDorb::core::util::InitialReference::InitialReference(){}

TIDorb::core::util::InitialReference::InitialReference(const char* name, const char* url)
  : object_name(name), object_url(url) {}

TIDorb::core::util::InitialReference::InitialReference(const InitialReference& v)
  : object_name(v.object_name), object_url(v.object_url) {}
  
void TIDorb::core::util::InitialReference::parse(const char* value)
  throw(CORBA::SystemException)
{
  string value_str = value;
  
  string::size_type position = value_str.find('=');
	
  if((position == string::npos) || (position != value_str.rfind('='))) {	
    throw CORBA::BAD_PARAM("Invalid -ORBInitRef value: it does not match <ObjectId>=<URL> pattern");
  }
	
  object_name = CORBA::string_dup(value_str.substr(0, position - 1).c_str());
  object_url  = CORBA::string_dup(value_str.substr(position + 1).c_str());
}
