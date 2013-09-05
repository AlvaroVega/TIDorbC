/////////////////////////////////////////////////////////////////////////
//
// File:        DynEnumImpl.C
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

#include "TIDorb/dynAny.h"
#include <string.h>


#define DYN_ENUM_REP_ID         "IDL:omg.org/DynamicAny/DynEnum:1.0"


namespace TIDorb {
namespace dynAny {




DynEnumImpl::DynEnumImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynSingleton(factory, orb)
{
   _enum_value = 0;
   this->TIDThr::RefCounter::_add_ref();
}




DynEnumImpl::DynEnumImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                         const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynSingleton(factory, orb)
{
   TIDorb::core::cdr::CDRInputStream*  in;
   TIDorb::core::cdr::CDROutputStream* out;

   _dyn_type  = any.type();
   _base_type = CORBA::TypeCode::_duplicate(real_type);

   out = new TIDorb::core::cdr::CDROutputStream(orb);

   any.delegate().write_value(*out);
  
   in = (TIDorb::core::cdr::CDRInputStream*)
                     (out->create_input_stream());

   in->read_ulong(_enum_value);

   delete in;
   delete out;

   this->TIDThr::RefCounter::_add_ref();
}




DynEnumImpl::DynEnumImpl(DynamicAny::DynAnyFactory_ptr factory,
                         TIDorb::core::TIDORB* orb,
                         CORBA::TypeCode_ptr type,
                         CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynSingleton(factory, orb)
{
   _dyn_type   = CORBA::TypeCode::_duplicate(type);
   _base_type  = CORBA::TypeCode::_duplicate(real_type);
   _enum_value = 0;

   this->TIDThr::RefCounter::_add_ref();
}




DynEnumImpl::~DynEnumImpl()
  throw (TIDThr::SystemException)
{
}




char* DynEnumImpl::get_as_string()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   try
   {
      return CORBA::string_dup(_base_type->member_name(_enum_value));
   }
   catch(...)
   {
      return NULL;
   }
}




void DynEnumImpl::set_as_string(const char* value)
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   long num_values = _base_type->member_count();
   for (long i=0; i < num_values; i++)
   {
      if (!strcmp(value, _base_type->member_name(i)))
      {
         _enum_value = i;
         return;
      }
   }

   throw DynamicAny::DynAny::InvalidValue();
}




CORBA::ULong DynEnumImpl::get_as_ulong()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   return _enum_value;
}




void DynEnumImpl::set_as_ulong(CORBA::ULong value)
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::ULong count = _base_type->member_count();
   if ((value < 0) || (value >= count))
   {
      throw DynamicAny::DynAny::InvalidValue();
   }

   _enum_value = value;
}




void DynEnumImpl::_read(TIDorb::portable::InputStream& in)
{
   try
   {
      CORBA::ULong value;
      in.read_ulong(value);
      set_as_ulong(value);
   }
   catch(DynamicAny::DynAny::InvalidValue& iv)
   {
      throw CORBA::MARSHAL(iv._name());
   }
}




void DynEnumImpl::_write(TIDorb::portable::OutputStream& out) const
{
   out.write_ulong(_enum_value);
}




void DynEnumImpl::assign(DynamicAny::DynAny_ptr dyn_any)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (!(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if (! _base_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynEnum_ptr enum_ptr;
   enum_ptr = dynamic_cast<DynamicAny::DynEnum_ptr>(dyn_any);
   if (enum_ptr)
   {
      _enum_value = enum_ptr->get_as_ulong();
   }
   else
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
}




void DynEnumImpl::from_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = value.type();
   if (! _base_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   TIDorb::portable::InputStream* is = NULL;
   try
   {
      is = value.delegate().create_input_stream();
      _read(*is);
      delete is;
   }
   catch(CORBA::MARSHAL)
   {
      delete is;
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Any* DynEnumImpl::to_any()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::Any* new_any = new CORBA::Any();

   // Fix bug [#392] Any::type(tc) reset any value
   //new_any->type(_dyn_type);
   new_any->delegate().set_type(_dyn_type);


   TIDorb::portable::OutputStream* output =
    new_any->delegate().create_output_stream();

   _write(*output);

   delete output;

   return new_any;
}




CORBA::Boolean DynEnumImpl::equal(DynamicAny::DynAny_ptr dyn_any)
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

   DynamicAny::DynEnum_ptr enum_ptr;
   enum_ptr = dynamic_cast<DynamicAny::DynEnum_ptr>(dyn_any);
   if (enum_ptr)
   {
      return (_enum_value == enum_ptr->get_as_ulong());
   }
   else
   {
      throw CORBA::BAD_PARAM(
                   "DynAny does not implement DynEnum when its type is enum");
   }
}




DynamicAny::DynAny_ptr DynEnumImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   DynEnumImpl* new_dyn = new DynEnumImpl(_factory, _orb);
   new_dyn->_enum_value = _enum_value;
   new_dyn->_dyn_type   = CORBA::TypeCode::_duplicate(_dyn_type);
   new_dyn->_base_type  = CORBA::TypeCode::_duplicate(_base_type);

   return new_dyn;
}




CORBA::Boolean DynEnumImpl::_is_a(const char* repositoryIdentifier)
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

   if (!strcmp(repositoryIdentifier, DYN_ENUM_REP_ID))
   {
      return true;
   }
   return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
   //   return DynSingleton::_is_a(repositoryIdentifier);
}




} //namespace dynAny
} //namespace TIDorb
