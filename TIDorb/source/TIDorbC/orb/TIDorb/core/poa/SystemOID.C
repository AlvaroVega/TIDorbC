/////////////////////////////////////////////////////////////////////////
//
// File:        SystemOID.C
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

#include "TIDorb/core/poa/SystemOID.h"

#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

TIDorb::core::poa::SystemOID::SystemOID()
{
  _poa_id = 0;
  _serial = 0;

  value.length(2* TIDorb::core::cdr::CDR::LONGLONG_SIZE);

  write_ulonglong(_poa_id, value, 0);
  write_ulonglong(_serial, value, TIDorb::core::cdr::CDR::LONGLONG_SIZE);
};

TIDorb::core::poa::SystemOID::SystemOID(CORBA::ULongLong poa_id, CORBA::ULongLong serial)
{
  _poa_id = poa_id;
  _serial = serial;
  value.length(2*TIDorb::core::cdr::CDR::LONGLONG_SIZE);

  write_ulonglong(_poa_id, value, 0);
  write_ulonglong(_serial, value, TIDorb::core::cdr::CDR::LONGLONG_SIZE);
};

TIDorb::core::poa::SystemOID::SystemOID(const TIDorb::core::poa::SystemOID& val) 
: TIDorb::core::poa::OID(val)
{
  _poa_id = val._poa_id;
  _serial = val._serial;
};

TIDorb::core::poa::SystemOID* TIDorb::core::poa::SystemOID::next()
{
  return new SystemOID(_poa_id, _serial+1);
};

CORBA::ULongLong TIDorb::core::poa::SystemOID::getPOAId()
{
  return _poa_id;
};

CORBA::ULongLong TIDorb::core::poa::SystemOID::getSerial()
{
  return _serial;
};

TIDorb::core::poa::SystemOID* TIDorb::core::poa::SystemOID::fromOID
(const TIDorb::core::poa::OID& oid)
{
  try {
    //jagd
    const SystemOID& other = dynamic_cast < const SystemOID & > (oid);
    //const SystemOID& other = *( const SystemOID* )(&oid);
     return new SystemOID(other);
  } catch(const bad_cast& bc) {
    return fromByteArray(oid.get_object_id());
  }
};

TIDorb::core::poa::SystemOID* TIDorb::core::poa::SystemOID::fromByteArray(const CORBA::OctetSeq& buffer)
{
  //if((buffer == NULL) || (buffer.length != 2 * TIDorb::core::cdr::CDR::LONGLONG_SIZE))
  if (buffer.length() != 2 * TIDorb::core::cdr::CDR::LONGLONG_SIZE)
  {
    TIDorb::core::poa::SystemOID *aux_systemoid=new TIDorb::core::poa::SystemOID();
    aux_systemoid=NULL;
    return aux_systemoid;
  }
  CORBA::ULongLong poa_id = read_ulonglong(buffer,0);

  CORBA::ULongLong serial = read_ulonglong(buffer, TIDorb::core::cdr::CDR::LONGLONG_SIZE);

  TIDorb::core::poa::SystemOID* aux_systemoid=new SystemOID(poa_id, serial);

  return aux_systemoid;
};

void TIDorb::core::poa::SystemOID::write_ulonglong(CORBA::ULongLong value,CORBA::OctetSeq& buffer, CORBA::ULong offset)
{
  // SystemOID always in little-endian
  
  if(TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER)
  {
    // do byte swap
    register CORBA::Octet* value_bytes = (CORBA::Octet*) &value;
    CORBA::ULong position = offset;
    buffer[position++] = value_bytes[7];
    buffer[position++] = value_bytes[6];
    buffer[position++] = value_bytes[5];
    buffer[position++] = value_bytes[4];
    buffer[position++] = value_bytes[3];
    buffer[position++] = value_bytes[2];
    buffer[position++] = value_bytes[1];
    buffer[position++] = value_bytes[0];
  } else {
    *((CORBA::ULongLong*) (buffer.get_buffer() +offset)) = value;
  }
};

CORBA::ULongLong TIDorb::core::poa::SystemOID::read_ulonglong(const CORBA::OctetSeq& buffer, CORBA::ULong offset)
{
  // SystemOID always in little-endian
  
  CORBA::ULongLong value;

  if(TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER)
  {
    // do byte swap
    
    register CORBA::Octet* value_bytes = (CORBA::Octet*) &value;
    
    CORBA::ULong position = offset;
    
    value_bytes[7] = buffer[position++];
    value_bytes[6] = buffer[position++];
    value_bytes[5] = buffer[position++];
    value_bytes[4] = buffer[position++];
    value_bytes[3] = buffer[position++];
    value_bytes[2] = buffer[position++];
    value_bytes[1] = buffer[position++];
    value_bytes[0] = buffer[position++];
  } else {
    value = *((CORBA::ULongLong*) (buffer.get_buffer() +offset));
  }
  
  return value;
};


