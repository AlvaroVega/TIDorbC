/////////////////////////////////////////////////////////////////////////
//
// File:        DynFixedImpl.C
// Description:
//              
// Rel:         01.00
// Created:     August, 2008
//
// Revised:     Alvaro Vega Garcia (avega at tid dot es)
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

#include "TIDorb/dynAny.h"


#define DYN_FIXED_REP_ID         "IDL:omg.org/DynamicAny/DynFixed:1.0"


namespace TIDorb {
namespace dynAny {




DynFixedImpl::DynFixedImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                           const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
  throw(CORBA::SystemException)
  : DynSingleton(factory, orb)
{
  _fixed_value = 0;
  _digits = 0;
  _scale = 0;
  this->TIDThr::RefCounter::_add_ref();
}




DynFixedImpl::DynFixedImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                           CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
  throw(CORBA::SystemException)
  : DynSingleton(factory, orb)
{
  _fixed_value = "0.0";
  _digits = 0;
  _scale = 0;
  this->TIDThr::RefCounter::_add_ref();
}




DynFixedImpl::DynFixedImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
  throw(CORBA::SystemException)
  : DynSingleton(factory, orb)
{
  _fixed_value = "0.0";
  _digits = 0;
  _scale = 0;
  this->TIDThr::RefCounter::_add_ref();
}




DynFixedImpl::~DynFixedImpl()
  throw (TIDThr::SystemException)
{
}




char* DynFixedImpl::get_value()
  throw(CORBA::SystemException)
{
  return _fixed_value.to_string();
}




CORBA::Boolean DynFixedImpl::set_value(const char* val)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed) {
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
  }

  if (val == NULL) {
    throw CORBA::BAD_PARAM("NULL string value");
  }

  _fixed_value.from_string(val);
  return true;
}




void DynFixedImpl::assign(DynamicAny::DynAny_ptr dyn_any)
  throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
  if (_destroyed) {
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
  }

  
  if (!(dyn_any)) {
    throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
  }

  CORBA::TypeCode_var type = dyn_any->type();
  if (! _base_type->equivalent(type)) {
    throw DynamicAny::DynAny::TypeMismatch();
  }
  
  DynamicAny::DynFixed_ptr fixed_ptr;
  fixed_ptr = dynamic_cast<DynamicAny::DynFixed_ptr>(dyn_any);
  if (fixed_ptr) {
    this->set_value(fixed_ptr->get_value());
  }
  else {
    throw DynamicAny::DynAny::TypeMismatch();
  }


}




void DynFixedImpl::from_any(const CORBA::Any& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed) {
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
  }
  
  CORBA::TypeCode_var type = value.type();
  if (! _base_type->equivalent(type)) {
    throw DynamicAny::DynAny::TypeMismatch();
  }
  
  TIDorb::portable::InputStream* is = NULL;
  try {
    is = value.delegate().create_input_stream();
    _read(*is);
    delete is;
  }
  catch(CORBA::MARSHAL) {
    delete is;
    throw DynamicAny::DynAny::InvalidValue();
  }
}




CORBA::Any* DynFixedImpl::to_any()
  throw(CORBA::SystemException)
{
  if (_destroyed) {
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
  }
  
  CORBA::Any* new_any = new CORBA::Any();
  
  new_any->delegate().set_type(_dyn_type);
  
  
  TIDorb::portable::OutputStream* output =
    new_any->delegate().create_output_stream();
  
  _write(*output);
  
  delete output;
  
  return new_any;
}




CORBA::Boolean DynFixedImpl::equal(DynamicAny::DynAny_ptr dyn_any)
  throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (!(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if (! _base_type->equivalent(type))
   {
      return false;
   }

   DynamicAny::DynFixed_ptr fixed_ptr;
   fixed_ptr = dynamic_cast<DynamicAny::DynFixed_ptr>(dyn_any);
   if (fixed_ptr)
   {
     return (strcmp(this->get_value(), fixed_ptr->get_value()) == 0);
   }
   else
   {
      throw CORBA::BAD_PARAM(
                   "DynAny does not implement DynEnum when its type is enum");
   }
}




DynamicAny::DynAny_ptr DynFixedImpl::copy()
  throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   DynFixedImpl* new_dyn = new DynFixedImpl(_factory, _orb);
   new_dyn->_fixed_value = _fixed_value;
   new_dyn->_dyn_type   = CORBA::TypeCode::_duplicate(_dyn_type);
   new_dyn->_base_type  = CORBA::TypeCode::_duplicate(_base_type);

   return new_dyn;
}




void DynFixedImpl::_read(TIDorb::portable::InputStream& in)
{
   try
   {
      in.read_fixed(_fixed_value, _digits, _scale);
   }
   catch(DynamicAny::DynAny::InvalidValue& iv)
   {
      throw CORBA::MARSHAL(iv._name());
   }
}




void DynFixedImpl::_write(TIDorb::portable::OutputStream& out) const
{
  out.write_fixed(_fixed_value, _digits, _scale);
}




CORBA::Boolean DynFixedImpl::_is_a(const char* repositoryIdentifier)
  throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (repositoryIdentifier == NULL)
   {
      throw CORBA::BAD_PARAM("Null string reference");
   }

   if (!strcmp(repositoryIdentifier, DYN_FIXED_REP_ID))
   {
      return true;
   }
   return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
}




} //namespace dynAny
} //namespace TIDorb
