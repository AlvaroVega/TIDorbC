/////////////////////////////////////////////////////////////////////////
//
// File:        DynSequenceImpl.C
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

#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif


#define DYN_SEQUENCE_REP_ID     "IDL:omg.org/DynamicAny/DynSequence:1.0"


namespace TIDorb {
namespace dynAny {




DynSequenceImpl::DynSequenceImpl(DynamicAny::DynAnyFactory_ptr factory,
                                 TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb)
{
  this->TIDThr::RefCounter::_add_ref();
}




DynSequenceImpl::DynSequenceImpl(DynamicAny::DynAnyFactory_ptr factory,
                                 TIDorb::core::TIDORB* orb,
                                 CORBA::TypeCode_ptr type,
                                 CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb, type, real_type)
{
   try
   {
      _bound = real_type->length();
   }
   catch(CORBA::TypeCode::BadKind)
   {
      throw CORBA::BAD_TYPECODE();
   }

   _bounded = (_bound!= 0);
   _component_count = 0;

   this->TIDThr::RefCounter::_add_ref();
}




DynSequenceImpl::DynSequenceImpl(DynamicAny::DynAnyFactory_ptr factory,
                                 TIDorb::core::TIDORB* orb,
                                 const CORBA::Any& any,
                                 CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynComposite(factory, orb, any, real_type)
{
   try
   {
      _bound = real_type->length();
   }
   catch(CORBA::TypeCode::BadKind)
   {
      throw CORBA::BAD_TYPECODE();
   }

   _bounded = (_bound!= 0);
   extract_length();

   this->TIDThr::RefCounter::_add_ref();
}


DynSequenceImpl::~DynSequenceImpl()
  throw (TIDThr::SystemException)
{
}

void DynSequenceImpl::extract_length()
   throw(CORBA::SystemException)
{
   CORBA::ULong length = 0;
   _next_value->read_ulong(length);
   _component_count = length;
   
   if(length > 0)
   {
        _current = 0;
   }
   
   _next_value->fix_starting();
   
   if ((_bound != 0) && (_component_count > _bound))
   {
      throw CORBA::MARSHAL("Invalid bounded sequence length");
   }
   
   
}




DynamicAny::DynAny_ptr DynSequenceImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynSequenceImpl* new_dyn =
      new DynSequenceImpl(_factory, _orb, _dyn_type, _base_type);

   copy_to(*new_dyn);
   new_dyn->_bound   = _bound;
   new_dyn->_bounded = _bounded;

   return new_dyn;
}




CORBA::ULong DynSequenceImpl::get_length()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   return component_count();
}




void DynSequenceImpl::set_length(CORBA::ULong len)
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (len < 0)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid Sequence length"
   }

   if (len == _component_count)
   {
      return;
   }

   if (len == 0)
   {
      reset();
   }

   if (_bounded && (len > _bound))
   {
      throw DynamicAny::DynAny::InvalidValue();
   }

   _component_count = len;

   CORBA::Long actual_components = _components.size();

   if (len < actual_components)
   {
      //destroy excedent
      delete _complete_value;
      _complete_value = NULL;
      delete _next_value;
      _next_value = NULL;

      vector<DynamicAny::DynAny_var>::iterator iter;
      iter = _components.begin();
      while(iter != _components.end()){
        (*iter)->destroy();
        _components.erase(iter);
        iter = _components.begin();
      }
//       for (iter = _components.begin(); iter != _components.end(); iter++)
//       {
//          (*iter)->destroy();
//          _components.erase(iter);
//       }
   }
}




DynamicAny::AnySeq* DynSequenceImpl::get_elements()
   throw(CORBA::SystemException)
{
   return DynComposite::get_elements();
}




void DynSequenceImpl::set_elements(const DynamicAny::AnySeq& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   set_length(value.length());
   DynComposite::set_elements(value);
}




DynamicAny::DynAnySeq* DynSequenceImpl::get_elements_as_dyn_any()
   throw(CORBA::SystemException)
{
   return DynComposite::get_elements_as_dyn_any();
}




void DynSequenceImpl::set_elements_as_dyn_any(const DynamicAny::DynAnySeq& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   set_length(value.length());
   DynComposite::set_elements_as_dyn_any(value);
}




void DynSequenceImpl::from_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   DynComposite::from_any(value);
   extract_length();
}




void DynSequenceImpl::_read(TIDorb::portable::InputStream& in)
{
   DynComposite::_read(in);
   extract_length();
}




void DynSequenceImpl::_write(TIDorb::portable::OutputStream& out) const
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_complete_value)
   {
      // value marshaled
      TIDorb::core::cdr::CDRInputStream* value_copy = _complete_value->copy();

      value_copy->rewind();

      //jagd
      //TIDorb::core::typecode::TypeCodeImpl* impl =
      //   dynamic_cast<TIDorb::core::typecode::TypeCodeImpl*>((CORBA::TypeCode_ptr)_base_type);
      TIDorb::core::typecode::TypeCodeImpl* impl =
         (TIDorb::core::typecode::TypeCodeImpl*)((CORBA::TypeCode_ptr)_base_type);

      //jagd
      //TIDorb::core::cdr::CDROutputStream& out_stream =
      //   dynamic_cast <TIDorb::core::cdr::CDROutputStream&> (out);
      TIDorb::core::cdr::CDROutputStream& out_stream =
         *(TIDorb::core::cdr::CDROutputStream*)(&out);

      impl->remarshal_value(*value_copy, out_stream);

      delete value_copy;
   }
   else
   {
      // value in the dynAny structure
      out.write_long(_component_count);
      DynComposite::_write(out);
   }
}




CORBA::TypeCode_ptr DynSequenceImpl::get_component_type(CORBA::ULong position)
   throw(CORBA::SystemException)
{
   try
   {
      return _base_type->content_type();
   }
   catch(CORBA::TypeCode::BadKind)
   {
      // unreachable
      throw CORBA::BAD_TYPECODE();
   }
}




void DynSequenceImpl::insert_boolean_seq(const CORBA::BooleanSeq& value)
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
      set_length(value.length());
      insert_boolean_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_boolean_seq(value);
   }
}




void DynSequenceImpl::insert_octet_seq(const CORBA::OctetSeq& value)
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
      set_length(value.length());
      insert_octet_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_octet_seq(value);
   }
}




void DynSequenceImpl::insert_char_seq(const CORBA::CharSeq& value)
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
      set_length(value.length());
      insert_char_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_char_seq(value);
   }
}




void DynSequenceImpl::insert_short_seq(const CORBA::ShortSeq& value)
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
      set_length(value.length());
      insert_short_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_short_seq(value);
   }
}




void DynSequenceImpl::insert_ushort_seq(const CORBA::UShortSeq& value)
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
      set_length(value.length());
      insert_ushort_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_ushort_seq(value);
   }
}




void DynSequenceImpl::insert_long_seq(const CORBA::LongSeq& value)
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
      set_length(value.length());
      insert_long_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_long_seq(value);
   }
}




void DynSequenceImpl::insert_ulong_seq(const CORBA::ULongSeq& value)
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
      set_length(value.length());
      insert_ulong_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_ulong_seq(value);
   }
}




void DynSequenceImpl::insert_float_seq(const CORBA::FloatSeq& value)
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
      set_length(value.length());
      insert_float_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_float_seq(value);
   }
}




void DynSequenceImpl::insert_double_seq(const CORBA::DoubleSeq& value)
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
      set_length(value.length());
      insert_double_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_double_seq(value);
   }
}




void DynSequenceImpl::insert_longlong_seq(const CORBA::LongLongSeq& value)
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
      set_length(value.length());
      insert_longlong_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_longlong_seq(value);
   }
}




void DynSequenceImpl::insert_ulonglong_seq(const CORBA::ULongLongSeq& value)
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
      set_length(value.length());
      insert_ulonglong_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_ulonglong_seq(value);
   }
}




void DynSequenceImpl::insert_longdouble_seq(const CORBA::LongDoubleSeq& value)
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
      set_length(value.length());
      insert_longdouble_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_longdouble_seq(value);
   }
}




void DynSequenceImpl::insert_wchar_seq(const CORBA::WCharSeq& value)
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
      set_length(value.length());
      insert_wchar_members(value);
   }
   else
   {
      set_length(value.length());
      DynComposite::insert_wchar_seq(value);
   }
}




CORBA::BooleanSeq* DynSequenceImpl::get_boolean_seq()
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




CORBA::OctetSeq* DynSequenceImpl::get_octet_seq()
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




CORBA::CharSeq* DynSequenceImpl::get_char_seq()
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




CORBA::ShortSeq* DynSequenceImpl::get_short_seq()
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




CORBA::UShortSeq* DynSequenceImpl::get_ushort_seq()
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




CORBA::LongSeq* DynSequenceImpl::get_long_seq()
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




CORBA::ULongSeq* DynSequenceImpl::get_ulong_seq()
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




CORBA::FloatSeq* DynSequenceImpl::get_float_seq()
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




CORBA::DoubleSeq* DynSequenceImpl::get_double_seq()
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




CORBA::LongLongSeq* DynSequenceImpl::get_longlong_seq()
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




CORBA::ULongLongSeq* DynSequenceImpl::get_ulonglong_seq()
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




CORBA::LongDoubleSeq* DynSequenceImpl::get_longdouble_seq()
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




CORBA::WCharSeq* DynSequenceImpl::get_wchar_seq()
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


CORBA::Boolean DynSequenceImpl::_is_a(const char* repositoryIdentifier)
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

   if (!strcmp(repositoryIdentifier, DYN_SEQUENCE_REP_ID))
   {
      return true;
   }
   return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
   //   return DynComposite::_is_a(repositoryIdentifier);
}




} //namespace dynAny
} //namespace TIDorb
