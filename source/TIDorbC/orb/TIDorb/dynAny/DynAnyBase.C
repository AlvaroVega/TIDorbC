/////////////////////////////////////////////////////////////////////////
//
// File:        DynAnyBase.C
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


#define DYN_ANY_BASE_REP_ID     "IDL:omg.org/DynamicAny/DynAny:1.0"


namespace TIDorb {
namespace dynAny {




DynAnyBase::DynAnyBase(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
{
   _orb          = orb;
   _factory      = DynamicAny::DynAnyFactory::_duplicate(factory);
   _destroyed    = false;
   _user_destroy = true;
   _dyn_type     = NULL;
   _base_type    = NULL;
}




DynAnyBase::DynAnyBase(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                       CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
{
   _orb          = orb;
   _factory      = DynamicAny::DynAnyFactory::_duplicate(factory);
   _destroyed    = false;
   _user_destroy = true;
   _dyn_type     = CORBA::TypeCode::_duplicate(type);
   _base_type    = CORBA::TypeCode::_duplicate(real_type);
}




DynAnyBase::~DynAnyBase()
{
}




void DynAnyBase::destroy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_user_destroy)
   {
      destroy_now();
   }
}




void DynAnyBase::destroy_now()
   throw()
{
   _orb       = NULL;
   _destroyed = true;
   _dyn_type = NULL;
   _base_type = NULL;
   _factory = NULL;
}




void DynAnyBase::set_user_destroy(CORBA::Boolean value)
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST();
   }

   _user_destroy = value;
}




CORBA::TypeCode_ptr DynAnyBase::_type() const
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   return CORBA::TypeCode::_duplicate(_dyn_type);
}




CORBA::TypeCode_ptr DynAnyBase::type()
   throw(CORBA::SystemException)
{
   return _type();
}




void DynAnyBase::type_mismatch()
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   throw DynamicAny::DynAny::TypeMismatch();
}




CORBA::ULong DynAnyBase::component_count()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   return 0;
}




CORBA::Boolean DynAnyBase::seek(CORBA::Long index)
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   return false;
}




void DynAnyBase::rewind()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }
}




CORBA::Boolean DynAnyBase::next()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   // no components
   return false;
}




DynamicAny::DynAny_ptr DynAnyBase::current_component()
   throw(DynamicAny::DynAny::TypeMismatch,CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




void DynAnyBase::insert_abstract(CORBA::AbstractBase_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue,CORBA::SystemException)
{
   // TODO: implement DynAnyBase::insert_abstract()
   throw CORBA::NO_IMPLEMENT();
}




void DynAnyBase::insert_val(CORBA::ValueBase_ptr value)
        throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   // TODO: implement DynAnyBase::insert_val()
   throw CORBA::NO_IMPLEMENT();
}




void DynAnyBase::insert_boolean_seq(const CORBA::BooleanSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_octet_seq(const CORBA::OctetSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_char_seq(const CORBA::CharSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_short_seq(const CORBA::ShortSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_ushort_seq(const CORBA::UShortSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_long_seq(const CORBA::LongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_ulong_seq(const CORBA::ULongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_float_seq(const CORBA::FloatSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_double_seq(const CORBA::DoubleSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_longlong_seq(const CORBA::LongLongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_ulonglong_seq(const CORBA::ULongLongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_longdouble_seq(const CORBA::LongDoubleSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynAnyBase::insert_wchar_seq(const CORBA::WCharSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




CORBA::AbstractBase_ptr DynAnyBase::get_abstract()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   // TODO: implement DynAnyBase::get_abstract()
   throw CORBA::NO_IMPLEMENT();
}




CORBA::ValueBase_ptr DynAnyBase::get_val()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::BooleanSeq* DynAnyBase::get_boolean_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::OctetSeq* DynAnyBase::get_octet_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::CharSeq* DynAnyBase::get_char_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::ShortSeq* DynAnyBase::get_short_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::UShortSeq* DynAnyBase::get_ushort_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::LongSeq* DynAnyBase::get_long_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::ULongSeq* DynAnyBase::get_ulong_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::FloatSeq* DynAnyBase::get_float_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::DoubleSeq* DynAnyBase::get_double_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::LongLongSeq* DynAnyBase::get_longlong_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::ULongLongSeq* DynAnyBase::get_ulonglong_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::LongDoubleSeq* DynAnyBase::get_longdouble_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::WCharSeq* DynAnyBase::get_wchar_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




} //namespace dynAny
} //namespace TIDorb
