/////////////////////////////////////////////////////////////////////////
//
// File:        DynValueImpl.C
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
#include "TIDorb/core/util/GenericValue.h"


#define DYN_VALUE_REP_ID       "IDL:omg.org/DynamicAny/DynValue:1.0"


using TIDorb::core::cdr::ValueTypeInfo;
using TIDorb::core::util::GenericValue;


namespace TIDorb {
namespace dynAny {




DynValueImpl::DynValueImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb), _is_null(true)
{
  this->TIDThr::RefCounter::_add_ref();
}




DynValueImpl::DynValueImpl(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           const CORBA::Any& any,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb, any, real_type), _is_null(false)
{
   extractTypeInfo();
   this->TIDThr::RefCounter::_add_ref();
}




DynValueImpl::DynValueImpl(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           CORBA::TypeCode_ptr type,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
  : DynComposite(factory, orb, type, real_type), _is_null(false)
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




DynValueImpl::~DynValueImpl()
  throw (TIDThr::SystemException)
{
}




void DynValueImpl::set_to_null()
{
   if (_destroyed) {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   _is_null = true;
   _component_count = 0;
   _current = -1;
}




void DynValueImpl::set_to_value()
{
   if (_destroyed) {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   _is_null = false;
   _component_count = _base_type->member_count();
   _current = (_component_count == 0)? -1 : 0;
}




void DynValueImpl::insert_val(CORBA::ValueBase_ptr value)
   throw (DynamicAny::DynAny::TypeMismatch,
          DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   // TODO: implementar DynValueImpl::insert_val()
   throw CORBA::NO_IMPLEMENT();
}	          




CORBA::ValueBase_ptr DynValueImpl::get_val()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   // TODO: implementar DynValueImpl::get_val()
   throw CORBA::NO_IMPLEMENT();
}




DynamicAny::DynAny_ptr DynValueImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynValueImpl* new_dyn =
      new DynValueImpl(_factory, _orb, _dyn_type, _base_type);

   new_dyn->_is_null = _is_null;

   copy_to(*new_dyn);

   return new_dyn;
}




DynamicAny::FieldName DynValueImpl::current_member_name()
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

   // at this point alwais curremt_member is valid
   return CORBA::string_dup(_base_type->member_name(_current));
}




CORBA::TCKind DynValueImpl::current_member_kind()
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




DynamicAny::NameValuePairSeq* DynValueImpl::get_members()
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




void DynValueImpl::set_members(const DynamicAny::NameValuePairSeq& value)
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
   }

   set_elements(*members_values);
   delete members_values;

}




DynamicAny::NameDynAnyPairSeq* DynValueImpl::get_members_as_dyn_any()
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
         (*members)[i].id = CORBA::string_dup(_base_type->member_name(i));
         (*members)[i].value = (*dyns)[i];
       }

   }
     
   delete dyns;
   return members;

}




void DynValueImpl::set_members_as_dyn_any(
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

   }

   set_elements_as_dyn_any(*members_values);
   delete members_values;

}




CORBA::TypeCode_ptr DynValueImpl::get_component_type(CORBA::ULong position)
   throw(CORBA::SystemException)
{
   return _base_type->member_type(position);
}




CORBA::Boolean DynValueImpl::_is_a(const char* repositoryIdentifier)
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

   if (!strcmp(repositoryIdentifier, DYN_VALUE_REP_ID))
   {
      return true;
   }
   return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
   //   return DynComposite::_is_a(repositoryIdentifier);
}




void DynValueImpl::_read(TIDorb::portable::InputStream& in)
{
   DynComposite::_read(in);
   extractTypeInfo();
}




void DynValueImpl::_write(TIDorb::portable::OutputStream& out) const
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

//    if (_complete_value)
//    {
//       // value marshaled
//       TIDorb::core::cdr::PointerCDR position = _complete_value->get_pointer();

//       CORBA::ValueBase_var value = new GenericValue(_base_type.in(), position);

//       out.write_Value(value.in());
//    }
//    else
//    {
//       // value in the dynAny structure
      ValueTypeInfo value_info;

//       if(!_is_null) {
//         value_info.set_null();
//       } else {
        const char* rep_id = _base_type->id();
        value_info.set_repository_ids(&rep_id, 1);
//       }

      TIDorb::core::cdr::CDROutputStream& cdr_output =
        *(TIDorb::core::cdr::CDROutputStream*) (&out);

      value_info.write(cdr_output);

      DynComposite::_write(out);
//    }
}




void DynValueImpl::extractTypeInfo()
{
    TIDorb::core::cdr::ValueTypeInfo value_info;

    value_info.read(*_next_value);

    _next_value->fix_starting();

    if (value_info.is_null()) {
       set_to_null();
    } else if (value_info.isIndirection()) {
       throw CORBA::INTERNAL("Unexpected Indirection");
    } else {

       const char* id = value_info.get_id();

       if (!id) {
           throw CORBA::MARSHAL();
       }

       if (value_info.isFragmented()) {
           throw  CORBA::NO_IMPLEMENT();
           //"Fragmented value reading not implemented"
       }


       const char* value_rep_id = _base_type->id();

       if ((!value_info.is_truncable(value_rep_id))) {
           throw CORBA::MARSHAL(); // "Cannot truncate value
       }

       set_to_value();
    }
}



} //namespace dynAny
} //namespace TIDorb
