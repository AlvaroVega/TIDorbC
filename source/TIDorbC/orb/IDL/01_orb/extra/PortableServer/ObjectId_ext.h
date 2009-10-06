#include "PortableServer.h"


#ifndef _PORTABLE_SERVER_OBJECT_ID_EXT_H_
#define _PORTABLE_SERVER_OBJECT_ID_EXT_H_


char*         ObjectId_to_string (const ObjectId& oid);
CORBA::WChar* ObjectId_to_wstring(const ObjectId& oid);

ObjectId*     string_to_ObjectId(const char* str);
ObjectId*     wstring_to_ObjectId(const CORBA::WChar* str);


#endif
