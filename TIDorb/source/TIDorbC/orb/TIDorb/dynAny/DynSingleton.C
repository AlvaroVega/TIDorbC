/////////////////////////////////////////////////////////////////////////
//
// File:        DynSingleton.C
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


namespace TIDorb {
namespace dynAny {




DynSingleton::DynSingleton(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb)
{
}




DynSingleton::DynSingleton(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           CORBA::TypeCode_ptr type,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, type, real_type)
{
}




DynSingleton::~DynSingleton()
{
}




void DynSingleton::insert_boolean(CORBA::Boolean value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_octet(CORBA::Octet value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_char(char value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_short(CORBA::Short value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_ushort(CORBA::UShort value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_long(CORBA::Long value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_ulong(CORBA::ULong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_float(CORBA::Float value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_double(CORBA::Double value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_string(const char* value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_wstring(const CORBA::WChar* value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_reference(CORBA::Object_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_typecode(CORBA::TypeCode_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_longlong(CORBA::LongLong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_ulonglong(CORBA::ULongLong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_longdouble(CORBA::LongDouble value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_wchar(CORBA::WChar value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




void DynSingleton::insert_dyn_any(DynamicAny::DynAny_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
}




CORBA::Boolean DynSingleton::get_boolean()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::Octet DynSingleton::get_octet()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




char DynSingleton::get_char()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::Short DynSingleton::get_short()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::UShort DynSingleton::get_ushort()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::Long DynSingleton::get_long()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::ULong DynSingleton::get_ulong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::Float DynSingleton::get_float()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::Double DynSingleton::get_double()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




char* DynSingleton::get_string()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::WChar* DynSingleton::get_wstring()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::Object_ptr DynSingleton::get_reference()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::TypeCode_ptr DynSingleton::get_typecode()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




CORBA::LongLong DynSingleton::get_longlong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::ULongLong DynSingleton::get_ulonglong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::LongDouble DynSingleton::get_longdouble()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::WChar DynSingleton::get_wchar()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::Any* DynSingleton::get_any()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return NULL;
}




DynamicAny::DynAny_ptr DynSingleton::get_dyn_any()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   type_mismatch();
   return 0;
}




CORBA::ULong DynSingleton::component_count()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   return 0;
}




CORBA::Boolean DynSingleton::seek(CORBA::Long index)
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   return false;
}




void DynSingleton::rewind()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }
}




CORBA::Boolean DynSingleton::next()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   return false;
}




DynamicAny::DynAny_ptr DynSingleton::current_component()
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   throw DynamicAny::DynAny::TypeMismatch(); // "DynAny without components"
}




} //namespace dynAny
} //namespace TIDorb
