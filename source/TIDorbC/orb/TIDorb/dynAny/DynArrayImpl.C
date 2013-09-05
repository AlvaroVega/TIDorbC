/////////////////////////////////////////////////////////////////////////
//
// File:        DynArrayImpl.C
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




DynArrayImpl::DynArrayImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
  throw(CORBA::SystemException)
  : DynComposite(factory, orb)
{
  this->TIDThr::RefCounter::_add_ref();
}




/**
 * Constructor. Gets an any object for reading its value.
 * It assumes that the any contains an array TypeCode.
 * @param any the any value.
 */

DynArrayImpl::DynArrayImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                           const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
  throw(CORBA::SystemException)
  : DynComposite(factory, orb, any, real_type)
{
  try {
        _component_count = real_type->length();
  } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
  }
  this->TIDThr::RefCounter::_add_ref();
}




// Constructor. Gets a TypeCode to create a new value.
// Warning: It assumes that the TypeCode is tk_sequence.

DynArrayImpl::DynArrayImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                           CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
  throw(CORBA::SystemException)
  : DynComposite(factory, orb, type, real_type)
{
  try {
        _component_count = real_type->length();
  } catch (const CORBA::TypeCode::BadKind& bk) {
        throw CORBA::BAD_TYPECODE();
  }
  this->TIDThr::RefCounter::_add_ref();
}

DynArrayImpl::~DynArrayImpl()
  throw (TIDThr::SystemException)
{
}


DynamicAny::DynAny_ptr DynArrayImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynArrayImpl* new_dyn =
      new DynArrayImpl(_factory, _orb, _dyn_type, _base_type);

   copy_to(*new_dyn);
   return new_dyn;
}




CORBA::TypeCode_ptr DynArrayImpl::get_component_type(CORBA::ULong position)
   throw(CORBA::SystemException)
{
   try {
        return _base_type->content_type();
   } catch (const CORBA::TypeCode::BadKind& bk) {
        // unreachable
        throw CORBA::BAD_TYPECODE();
   }
}




// CORBA 2.5

// DynArray Operations

DynamicAny::AnySeq* DynArrayImpl::get_elements()
   throw(CORBA::SystemException)
{
        return DynComposite::get_elements();
}




void DynArrayImpl::set_elements(const DynamicAny::AnySeq& value)
     throw(DynamicAny::DynAny::TypeMismatch,
           DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
        DynComposite::set_elements(value);
}




DynamicAny::DynAnySeq* DynArrayImpl::get_elements_as_dyn_any()
   throw(CORBA::SystemException)
{
  return DynComposite::get_elements_as_dyn_any();
}




void DynArrayImpl::set_elements_as_dyn_any(const DynamicAny::DynAnySeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
        DynComposite::set_elements_as_dyn_any(value);
}




void DynArrayImpl::insert_boolean_seq(const CORBA::BooleanSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_boolean))
  {
    insert_boolean_members(value);

  } else  {
   DynComposite::insert_boolean_seq(value);
  }
}




void DynArrayImpl::insert_octet_seq(const CORBA::OctetSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_octet))
  {
    insert_octet_members(value);

  } else  {
   DynComposite::insert_octet_seq(value);
  }
}




void DynArrayImpl::insert_char_seq(const CORBA::CharSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_char))
  {
    insert_char_members(value);

  } else  {
   DynComposite::insert_char_seq(value);
  }
}




void DynArrayImpl::insert_short_seq(const CORBA::ShortSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_short))
  {
    insert_short_members(value);

  } else  {
   DynComposite::insert_short_seq(value);
  }
}




void DynArrayImpl::insert_ushort_seq(const CORBA::UShortSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_ushort))
  {
    insert_ushort_members(value);

  } else  {
   DynComposite::insert_ushort_seq(value);
  }
}




void DynArrayImpl::insert_long_seq(const CORBA::LongSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_long))
  {
    insert_long_members(value);

  } else  {
   DynComposite::insert_long_seq(value);
  }
}




void DynArrayImpl::insert_ulong_seq(const CORBA::ULongSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_ulong))
  {
    insert_ulong_members(value);

  } else  {
   DynComposite::insert_ulong_seq(value);
  }
}




void DynArrayImpl::insert_float_seq(const CORBA::FloatSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_float))
  {
    insert_float_members(value);

  } else  {
   DynComposite::insert_float_seq(value);
  }
}




void DynArrayImpl::insert_double_seq(const CORBA::DoubleSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_double))
  {
    insert_double_members(value);

  } else  {
   DynComposite::insert_double_seq(value);
  }
}




void DynArrayImpl::insert_longlong_seq(const CORBA::LongLongSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_longlong))
  {
    insert_longlong_members(value);

  } else  {
   DynComposite::insert_longlong_seq(value);
  }
}




void DynArrayImpl::insert_ulonglong_seq(const CORBA::ULongLongSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_ulonglong))
  {
    insert_ulonglong_members(value);

  } else  {
   DynComposite::insert_ulonglong_seq(value);
  }
}




void DynArrayImpl::insert_longdouble_seq(const CORBA::LongDoubleSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_longdouble))
  {
    insert_longdouble_members(value);

  } else  {
   DynComposite::insert_longdouble_seq(value);
  }
}




void DynArrayImpl::insert_wchar_seq(const CORBA::WCharSeq& value)
  throw(DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
  if (_destroyed)
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed.");

  CORBA::TypeCode_var c_type = get_component_type(0);
  if (c_type->equivalent(CORBA::_tc_wchar))
  {
    insert_wchar_members(value);

  } else  {
   DynComposite::insert_wchar_seq(value);
  }
}




CORBA::BooleanSeq* DynArrayImpl::get_boolean_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_boolean))
   {
      return DynComposite::get_boolean_members();
   }
   else
   {
      return DynComposite::get_boolean_seq();
   }
}




CORBA::OctetSeq* DynArrayImpl::get_octet_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_octet))
   {
      return DynComposite::get_octet_members();
   }
   else
   {
      return DynComposite::get_octet_seq();
   }
}




CORBA::CharSeq* DynArrayImpl::get_char_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_char))
   {
      return DynComposite::get_char_members();
   }
   else
   {
      return DynComposite::get_char_seq();
   }
}




CORBA::ShortSeq* DynArrayImpl::get_short_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_short))
   {
      return DynComposite::get_short_members();
   }
   else
   {
      return DynComposite::get_short_seq();
   }
}




CORBA::UShortSeq* DynArrayImpl::get_ushort_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_ushort))
   {
      return DynComposite::get_ushort_members();
   }
   else
   {
      return DynComposite::get_ushort_seq();
   }
}




CORBA::LongSeq* DynArrayImpl::get_long_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_long))
   {
      return DynComposite::get_long_members();
   }
   else
   {
      return DynComposite::get_long_seq();
   }
}




CORBA::ULongSeq* DynArrayImpl::get_ulong_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_ulong))
   {
      return DynComposite::get_ulong_members();
   }
   else
   {
      return DynComposite::get_ulong_seq();
   }
}




CORBA::FloatSeq* DynArrayImpl::get_float_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_float))
   {
      return DynComposite::get_float_members();
   }
   else
   {
      return DynComposite::get_float_seq();
   }
}




CORBA::DoubleSeq* DynArrayImpl::get_double_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_double))
   {
      return DynComposite::get_double_members();
   }
   else
   {
      return DynComposite::get_double_seq();
   }
}




CORBA::LongLongSeq* DynArrayImpl::get_longlong_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_longlong))
   {
      return DynComposite::get_longlong_members();
   }
   else
   {
      return DynComposite::get_longlong_seq();
   }
}




CORBA::ULongLongSeq* DynArrayImpl::get_ulonglong_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_ulonglong))
   {
      return DynComposite::get_ulonglong_members();
   }
   else
   {
      return DynComposite::get_ulonglong_seq();
   }
}




CORBA::LongDoubleSeq* DynArrayImpl::get_longdouble_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_longdouble))
   {
      return DynComposite::get_longdouble_members();
   }
   else
   {
      return DynComposite::get_longdouble_seq();
   }
}




CORBA::WCharSeq* DynArrayImpl::get_wchar_seq()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::TypeCode_var type = get_component_type(0);
   if (type->equivalent(CORBA::_tc_wchar))
   {
      return DynComposite::get_wchar_members();
   }
   else
   {
      return DynComposite::get_wchar_seq();
   }
}




} //namespace dynAny
} //namespace TIDorb
