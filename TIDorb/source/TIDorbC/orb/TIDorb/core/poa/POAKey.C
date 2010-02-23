/////////////////////////////////////////////////////////////////////////
//
// File:        POAKey.C
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

#include "TIDorb/core/poa/POAKey.h"

TIDorb::core::poa::POAKey::POAKey() 
  throw (TIDThr::SystemException)
: _poa_id(0UL), _oid(new OID()), key_name(0)
{
}




    
TIDorb::core::poa::POAKey::POAKey(PortableServer::POA* poa, 
                                  CORBA::ULongLong poa_id, 
                                  TIDorb::core::poa::OID* oid)
throw (TIDThr::SystemException)
: _poa_id(poa_id),
  _oid(oid),
  key_name(NULL)
{
  TIDorb::core::poa::POAImpl* poa_aux=dynamic_cast<TIDorb::core::poa::POAImpl*>(poa);
  _poas = poa_aux->the_path();
  
}




TIDorb::core::poa::POAKey::POAKey(const TIDorb::core::poa::POAKey& other)
throw (TIDThr::SystemException)
: _poas(other._poas),
  _poa_id(other._poa_id),
  key_name(NULL)
{ 
  _oid = new OID(*(other._oid));
}




TIDorb::core::poa::POAKey::POAKey(const TIDorb::core::poa::POAPath& poas, 
                                  CORBA::ULongLong poa_id,
                                  TIDorb::core::poa::OID* oid)
throw (TIDThr::SystemException)

: _poas(poas),
  _poa_id(poa_id),
  _oid(oid),
  key_name(NULL)
{
}




TIDorb::core::poa::POAKey::~POAKey()
 throw (TIDThr::SystemException)
{
  CORBA::string_free(key_name);
  delete _oid;
}




const char* TIDorb::core::poa::POAKey::getPOA(CORBA::ULong poa_level) const
throw (TIDThr::SystemException) 
{
  return _poas[poa_level];
}




CORBA::ULong TIDorb::core::poa::POAKey::numberOfPOAs() const {
  return _poas.length();
}  




const TIDorb::core::poa::OID& TIDorb::core::poa::POAKey::getOID() const {
  return *_oid;
}




CORBA::ULongLong TIDorb::core::poa::POAKey::getPOAId() const {
  return _poa_id;
}




void TIDorb::core::poa::POAKey::setOID(TIDorb::core::poa::OID* oid) {
  if((_oid) && (_oid != oid))
    delete _oid;
    
  _oid = oid;
}




void TIDorb::core::poa::POAKey::write(TIDorb::core::cdr::CDROutputStream& output) 
{
    
  // KEY is always 1.2
    
  const TIDorb::core::comm::iiop::Version& output_version = output.get_version();
    
  output.set_version(TIDorb::core::comm::iiop::Version::VERSION_1_2);
    
  // write string[] with poas path
  

  output.write_ulong(_poas.length());
    
  for(size_t i = 0; i < _poas.length(); i++)
    output.write_string(_poas[i]);

    
  // lifespan time
      
  output.write_ulonglong(_poa_id);
    
  // oid
    
  _oid->write(output);
    
  output.set_version(output_version);
}




void TIDorb::core::poa::POAKey::read(TIDorb::core::cdr::CDRInputStream& input) 
{
  // KEY is always 1.2
    
  const TIDorb::core::comm::iiop::Version& input_version = input.get_version();
    
  input.set_version(TIDorb::core::comm::iiop::Version::VERSION_1_2);
    
  // read string[] with poas path
    
  CORBA::ULong length;
  input.read_ulong(length);
  
  if ((length < 0) || (length > 65536)) // To avoid std::bad_alloc setting _poas length
    throw CORBA::MARSHAL();//"Malformed POAKey(bad number of poas)");

  _poas.length(length);
  
  char* aux;
  
  for(CORBA::ULong i = 0; i < length; i++) {
     input.read_string(aux);
     _poas[i] = aux;
  }

  input.read_ulonglong(_poa_id);
  
  _oid->read(input);
    
  input.set_version(input_version);
}  



 
bool TIDorb::core::poa::POAKey::samePOA(const TIDorb::core::poa::POAKey& other) const 
{
  return samePOA(other._poa_id, other._poas);
}




bool TIDorb::core::poa::POAKey::samePOA
  (CORBA::ULongLong poa_id, 
   const TIDorb::core::poa::POAPath& poas) const
{
  if(_poa_id != poa_id) 
    return false;
  if (_poas.length() != poas.length()) 
    return false;
  
  for (CORBA::ULong i = 0; i < _poas.length(); i++) {
    if (!(strcmp(_poas[i],poas[i])==0))
      return false;
  }
  return true;
}




CORBA::Boolean TIDorb::core::poa::POAKey::operator==(const TIDorb::core::poa::POAKey& other) const 
{
  if(_poa_id != other._poa_id) 
          return false;
      if (_poas.length() != other._poas.length()) 
          return false;
      for (CORBA::ULong i = 0; i < _poas.length(); i++) {
          if (!(strcmp(_poas[i],other._poas[i])==0))
            return false;
      }
      return true;  
}




TIDorb::core::poa::POAKey& TIDorb::core::poa::POAKey::operator=(const TIDorb::core::poa::POAKey& other) 
{
  _poas = other._poas;
  _poa_id = other._poa_id;
  setOID(other._oid);
  delete key_name;
  key_name = NULL;
  return *this;
}




TIDorb::core::poa::POAKey* TIDorb::core::poa::POAKey::clone() {
  TIDorb::core::poa::POAKey* poa_aux = new POAKey();
  poa_aux->operator=(*this);
  return poa_aux;
}



    
char* TIDorb::core::poa::POAKey::getPOAPath() const
{
  TIDorb::util::StringBuffer os;

  os << "/";
  for(size_t i = 0; i< _poas.length(); i++){
    os << _poas[i] << "/";
  } 
  
  return CORBA::string_dup(os.str().data());
}




const char* TIDorb::core::poa::POAKey::toString() 
{
  if (key_name == NULL) {
    TIDorb::util::StringBuffer buffer;
    buffer << "ObjectKey[POA: /";
    
    for (size_t i = 0; i < _poas.length(); i++) {
      buffer << (const char*) _poas[i] << '/';
    }
    
    if (_poa_id == 0L) {
      buffer << "; Type: PERSISTENT ";
    } else {
      buffer << "; Type: TRANSIENT(POA id: " << _poa_id << "); ";
    }
    
    buffer << _oid->toString() << ']';
    
    key_name = CORBA::string_dup(buffer.str().data());
  }

  return key_name;
}
