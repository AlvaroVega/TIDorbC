/////////////////////////////////////////////////////////////////////////
//
// File:        OID.C
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

#include "TIDorb/core/poa/OID.h"

TIDorb::core::poa::OID::OID() : hash_created(false), str(NULL) {};

TIDorb::core::poa::OID::OID(const PortableServer::ObjectId& val) 
  : value(val), hash_created (false), str(NULL)
{};

TIDorb::core::poa::OID::OID(const OID& other) 
  : value (other.value), hash_code(other.hash_code), 
    hash_created(other.hash_created), str(CORBA::string_dup(other.str))
{};

//PRA
TIDorb::core::poa::OID::~OID()
{
  CORBA::string_free(str);
};
//EPRA

const PortableServer::ObjectId& TIDorb::core::poa::OID::get_object_id() const
{
  return value;
};

PortableServer::ObjectId_ptr TIDorb::core::poa::OID::toByteArray() const
{
  return new PortableServer::ObjectId(value);
};


CORBA::Boolean TIDorb::core::poa::OID::operator==(const TIDorb::core::poa::OID& other) const {
  CORBA::ULong value_length = value.length();
  if(value_length != other.value.length())
  {
    return false;
  }
  if(value_length == 0)
    return true; // Performance improvement
  return (memcmp(value.get_buffer(), other.value.get_buffer(), value_length) == 0);
};

CORBA::Boolean TIDorb::core::poa::OID::operator<(const TIDorb::core::poa::OID& other) const {
  CORBA::ULong value_length = value.length();
  CORBA::ULong other_value_length = other.value.length();
  if (value_length > other_value_length)
    return false;
  if(value_length < other_value_length)
    return true;
  // Here value_length is equal to other_value_length
  if (value_length == 0) 
    return false; // Performance improvement
  return memcmp(value.get_buffer(), other.value.get_buffer(), value_length) < 0;
};

TIDorb::core::poa::OID& TIDorb::core::poa::OID::operator=(const TIDorb::core::poa::OID& other) 
{
  reset();
  value = other.value;
  str = CORBA::string_dup(other.str);
  hash_created = other.hash_created;
  hash_code = other.hash_code;
  return *this;
};

CORBA::ULong TIDorb::core::poa::OID::hashCode() const
{
  if(!hash_created) {
    if(value.length() == 0)
      return 0;

    ((OID *)this)->hash_code = 31 * hash_code + atol((const char*)value.get_buffer());
    ((OID *)this)->hash_created = true;
  }

  return hash_code;
};

void TIDorb::core::poa::OID::fromString(const char* fromstr)
{
  reset();
  
  char* aux;
  size_t value_length;
  
  TIDorb::util::Base64Codec::decode(fromstr, aux, value_length);  
  value.replace(value_length, value_length, (CORBA::Octet*) aux, true);
};

const char* TIDorb::core::poa::OID::toString() const
{
  if(str == NULL) {
    if (value.length() > 0)  {
      char* aux;
      TIDorb::util::Base64Codec::encode((const char*)value.get_buffer(),value.length(),aux);
      ((OID*) this)->str=aux;
    } else {
      ((OID*) this)->str = CORBA::string_dup("");
    }
  }

  return str;
};

void TIDorb::core::poa::OID::reset()
{
  value.length(0);
  hash_created = false;
  CORBA::string_free(str);
  str = NULL;
}

void TIDorb::core::poa::OID::read(TIDorb::core::cdr::CDRInputStream& input)
{

 // read byte[] with oid
  
  CORBA::ULong buffer_length; 
  input.read_ulong(buffer_length);

  if(buffer_length == 0)
    throw CORBA::MARSHAL("Malformed POAKey");

  value.length(buffer_length);
  
  input.read_octet_array(value.get_buffer(),0,buffer_length);
  
}

void TIDorb::core::poa::OID::write(TIDorb::core::cdr::CDROutputStream& output)
{
  PortableServer::_ObjectIdHelper::write(output, value);
}
