/////////////////////////////////////////////////////////////////////////
//
// File:        DynStructImpl.C
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


#define DYN_STRUCT_REP_ID       "IDL:omg.org/DynamicAny/DynStruct:1.0"


namespace TIDorb {
namespace dynAny {




DynStructImpl::DynStructImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb)
{
  this->TIDThr::RefCounter::_add_ref();
}




DynStructImpl::DynStructImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb,
                             const CORBA::Any& any,
                             CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb, any, real_type)
{
   try
   {
      _component_count = real_type->member_count();
   }
   catch(CORBA::TypeCode::BadKind)
   {
      throw CORBA::BAD_TYPECODE();
   }

   _current = (_component_count == 0) ? -1 : 0;

   this->TIDThr::RefCounter::_add_ref();
}




DynStructImpl::DynStructImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb,
                             CORBA::TypeCode_ptr type,
                             CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb, type, real_type)
{
   try
   {
      _component_count = real_type->member_count();
   }
   catch(CORBA::TypeCode::BadKind)
   {
      throw CORBA::BAD_TYPECODE();
   }

   _current = (_component_count == 0) ? -1 : 0;

   this->TIDThr::RefCounter::_add_ref();
}




DynStructImpl::~DynStructImpl()
  throw (TIDThr::SystemException)
{
}




DynamicAny::DynAny_ptr DynStructImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynStructImpl* new_dyn =
      new DynStructImpl(_factory, _orb, _dyn_type, _base_type);

   copy_to(*new_dyn);
   return new_dyn;
}




DynamicAny::FieldName DynStructImpl::current_member_name()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No member (current = -1)"
   }

   // at this point always current_member is valid
   return CORBA::string_dup(_base_type->member_name(_current));
}




CORBA::TCKind DynStructImpl::current_member_kind()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No member (current = -1)"
   }

   CORBA::TypeCode_var type = _base_type->member_type(_current);
   return type->kind();
}




DynamicAny::NameValuePairSeq* DynStructImpl::get_members()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynamicAny::AnySeq* any_members = DynComposite::get_elements();
   DynamicAny::NameValuePairSeq* members =
      new DynamicAny::NameValuePairSeq(_component_count);

   if (_component_count > 0){

       members->length(_component_count);
       for (CORBA::ULong i=0; i < _component_count; i++)
         {
           (*members)[i].id = CORBA::string_dup(_base_type->member_name(i));
           (*members)[i].value = (*any_members)[i];
         }
   }

   delete any_members;
   return members;
}




void DynStructImpl::set_members(const DynamicAny::NameValuePairSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   reset();

   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::AnySeq* members_values= new DynamicAny::AnySeq(_component_count);

   if (_component_count > 0){

     members_values->length(_component_count);
     
     for (CORBA::ULong i=0; i < _component_count; i++)
       {
         (*members_values)[i] = value[i].value;
         
         if (strcmp(_base_type->member_name(i), value[i].id))
           {
             throw DynamicAny::DynAny::InvalidValue();
           }
       }

     set_elements(*members_values);
   }
   delete members_values;

}




DynamicAny::NameDynAnyPairSeq* DynStructImpl::get_members_as_dyn_any()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynamicAny::DynAnySeq* dyns = DynComposite::get_elements_as_dyn_any();
   DynamicAny::NameDynAnyPairSeq* members =
      new DynamicAny::NameDynAnyPairSeq(_component_count);

   if (_component_count > 0){

     members->length(_component_count);
     for (CORBA::ULong i=0; i < _component_count; i++)
       {
         (*members)[i].id= CORBA::string_dup(_base_type->member_name(i));
         (*members)[i].value =
           DynamicAny::DynAny::_duplicate((*dyns)[i]);
       }

   }

   delete dyns;
   return members;

}




void DynStructImpl::set_members_as_dyn_any(
   const DynamicAny::NameDynAnyPairSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAnySeq* members_values =
      new DynamicAny::DynAnySeq(_component_count);

   if (_component_count > 0) {

     members_values->length(_component_count);

     for (CORBA::ULong i=0; i < _component_count; i++)
       {
         (*members_values)[i] = value[i].value;
         
         if (strcmp(_base_type->member_name(i), value[i].id))
           {
             throw DynamicAny::DynAny::InvalidValue();
           }
       }
     set_elements_as_dyn_any(*members_values);
   }

   delete members_values;

}




CORBA::TypeCode_ptr DynStructImpl::get_component_type(CORBA::ULong position)
   throw(CORBA::SystemException)
{
   return _base_type->member_type(position);
}




CORBA::Boolean DynStructImpl::_is_a(const char* repositoryIdentifier)
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

   if (!strcmp(repositoryIdentifier, DYN_STRUCT_REP_ID))
   {
      return true;
   }
   return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
   //   return DynComposite::_is_a(repositoryIdentifier);
}




} //namespace dynAny
} //namespace TIDorb
