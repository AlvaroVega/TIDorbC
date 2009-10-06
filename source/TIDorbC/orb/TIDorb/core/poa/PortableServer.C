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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
