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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
