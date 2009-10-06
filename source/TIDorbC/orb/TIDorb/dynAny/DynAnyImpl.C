/////////////////////////////////////////////////////////////////////////
//
// File:        DynAnyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/dynAny.h"
#include <string.h>
#include <wchar.h>

#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDorb {
namespace dynAny {




DynAnyImpl::DynAnyImpl(DynamicAny::DynAnyFactory_ptr factory,
                       TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb), 
     //jagd
     //_value_delegate(dynamic_cast<TIDorb::core::AnyImpl&>(_value.delegate()))
     _value_delegate(*(TIDorb::core::AnyImpl*)(&_value.delegate()))
{
  this->TIDThr::RefCounter::_add_ref();
}




DynAnyImpl::DynAnyImpl(DynamicAny::DynAnyFactory_ptr factory,
                       TIDorb::core::TIDORB* orb,
                       const CORBA::Any& any,
                       CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, CORBA::TypeCode_var(any.type()), real_type),
     //jagd 
     //_value_delegate(dynamic_cast<TIDorb::core::AnyImpl&>(_value.delegate()))
     _value_delegate(*(TIDorb::core::AnyImpl*)(&_value.delegate()))
{
   init_value(any);
   this->TIDThr::RefCounter::_add_ref();
}




DynAnyImpl::DynAnyImpl(DynamicAny::DynAnyFactory_ptr factory,
                       TIDorb::core::TIDORB* orb,
                       CORBA::TypeCode_ptr type,
                       CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, type, real_type),
     //jagd 
     //_value_delegate(dynamic_cast<TIDorb::core::AnyImpl&>(_value.delegate()))
     _value_delegate(*(TIDorb::core::AnyImpl*)(&_value.delegate()))
{
   init_value();
   this->TIDThr::RefCounter::_add_ref();
}




DynAnyImpl::~DynAnyImpl()
  throw (TIDThr::SystemException)
{
}




void DynAnyImpl::_read(TIDorb::portable::InputStream& in)
{
  // FIX bug [#393] Error processing a return of DII invocation
  // Call read_value with skip optimization
  _value_delegate.read_value(in, _base_type, true);
}




void DynAnyImpl::_write(TIDorb::portable::OutputStream& out) const
{
   _value_delegate.write_value(out);
}




void DynAnyImpl::assign(DynamicAny::DynAny_ptr dyn_any)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   //jagd
   //if (CORBA::is_nil(dyn_any))
   if (!(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if (! _base_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   try
   {
      switch (_base_type->kind())
      {
         case CORBA::tk_null:
         case CORBA::tk_void:
         {
            // Fix bug [#392] Any::type(tc) reset any value
            //_value_delegate.type(_base_type);
            _value_delegate.set_type(_base_type);
            return;
         }

         case CORBA::tk_any:
         {
            _value_delegate.insert_any(dyn_any->get_any()); // non-copying
            return;
         }

         case CORBA::tk_objref:
         {
            CORBA::Object_var reference = dyn_any->get_reference();
            _value_delegate.insert_Object(reference);
            return;
         }

         case CORBA::tk_TypeCode:
         {
            CORBA::TypeCode_var typecode = dyn_any->get_typecode();
            _value_delegate.insert_TypeCode(typecode);
            return;
         }

         case CORBA::tk_short:
         {
            _value_delegate.insert_short(dyn_any->get_short());
            return;
         }

         case CORBA::tk_long:
         {
            _value_delegate.insert_long(dyn_any->get_long());
            return;
         }

         case CORBA::tk_ushort:
         {
            _value_delegate.insert_ushort(dyn_any->get_ushort());
            return;
         }

         case CORBA::tk_ulong:
         {
            _value_delegate.insert_ulong(dyn_any->get_ulong());
            return;
         }

         case CORBA::tk_float:
         {
            _value_delegate.insert_float(dyn_any->get_float());
            return;
         }

         case CORBA::tk_double:
         {
            _value_delegate.insert_double(dyn_any->get_double());
            return;
         }

         case CORBA::tk_boolean:
         {
            _value_delegate.insert_boolean(dyn_any->get_boolean());
            return;
         }

         case CORBA::tk_char:
         {
            _value_delegate.insert_char(dyn_any->get_char());
            return;
         }

         case CORBA::tk_octet:
         {
            _value_delegate.insert_octet(dyn_any->get_octet());
            return;
         }

         case CORBA::tk_longlong:
         {
            _value_delegate.insert_longlong(dyn_any->get_longlong());
            return;
         }

         case CORBA::tk_ulonglong:
         {
            _value_delegate.insert_ulonglong(dyn_any->get_ulonglong());
            return;
         }

         case CORBA::tk_longdouble:
         {
            _value_delegate.insert_longdouble(dyn_any->get_longdouble());
            return;
         }


         case CORBA::tk_wchar:
         {
            _value_delegate.insert_wchar(dyn_any->get_wchar());
            return;
         }

         case CORBA::tk_string:
         {
            char* str = dyn_any->get_string();
            _value_delegate.insert_string(str);
            CORBA::string_free(str);
            return;
         }

         case CORBA::tk_wstring:
         {
            CORBA::WChar* str = dyn_any->get_wstring();
            _value_delegate.insert_wstring(str);
            CORBA::wstring_free(str);
            return;
         }
        default:
            break;
      }
   }
   catch(DynamicAny::DynAny::InvalidValue& iv)
   {
      throw CORBA::BAD_PARAM(iv._name());
   }

   // set the any type to the "dyn_type"
   // Fix bug [#392] Any::type(tc) reset any value
   //_value_delegate.set_equivalent_type(_base_type);
   _value_delegate.type(_base_type);
}




void DynAnyImpl::from_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = value.type();
   if (! _dyn_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   init_value(value);
}




CORBA::Any* DynAnyImpl::to_any()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::Any* new_value = _orb->create_any();
   try
   {
     TIDorb::core::AnyImpl& any =
       //dynamic_cast<TIDorb::core::AnyImpl&>(new_value->delegate());
       *(TIDorb::core::AnyImpl*)&(new_value->delegate());

      // Fix bug [#392] Any::type(tc) reset any value
      //any.type(_dyn_type);
      //any.set_equivalent_type(_dyn_type);
      any.set_type(_dyn_type);
      any.assign(_value_delegate);
   }
   catch(bad_cast)
   {
      throw CORBA::INTERNAL();
   }
   
   return new_value;
}




CORBA::Boolean DynAnyImpl::equal(DynamicAny::DynAny_ptr dyn_any)
   throw(CORBA::SystemException)
{
   if (CORBA::is_nil(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if(! _base_type->equivalent(type))
   {
      return false;
   }

   try
   {
      switch (_base_type->kind())
      {
         case CORBA::tk_null:
         case CORBA::tk_void:
         {
            return true;
         }

         case CORBA::tk_any:
         {
            CORBA::Any_var dyn_any_value = dyn_any->get_any();
            const CORBA::Any* value;
            return (_value_delegate.extract_any(value)
                    && (*value == *dyn_any_value));
         }

         case CORBA::tk_objref:
         {
            CORBA::Object_var dyn_any_reference = dyn_any->get_reference();
            CORBA::Object_var value;
            return (_value_delegate.extract_Object(value)
                    && value->_is_equivalent(dyn_any_reference));
         }

         case CORBA::tk_TypeCode:
         {
            CORBA::TypeCode_var dyn_any_value = dyn_any->get_typecode();
            CORBA::TypeCode_var value;
            return (_value_delegate.extract_TypeCode(value)
                    && (value->equal(dyn_any_value)));
         }

         case CORBA::tk_short:
         {
            CORBA::Short value;
            return (_value_delegate.extract_short(value)
                    && (value == dyn_any->get_short()));
         }

         case CORBA::tk_long:
         {
            CORBA::Long value;
            return (_value_delegate.extract_long(value)
                    && (value == dyn_any->get_long()));
         }

         case CORBA::tk_ushort:
         {
            CORBA::UShort value;
            return (_value_delegate.extract_ushort(value)
                    && (value == dyn_any->get_ushort()));
         }

         case CORBA::tk_ulong:
         {
            CORBA::ULong value;
            return (_value_delegate.extract_ulong(value)
                    && (value == dyn_any->get_ulong()));
         }

         case CORBA::tk_float:
         {
            CORBA::Float value;
            return (_value_delegate.extract_float(value)
                    && (value == dyn_any->get_float()));
         }

         case CORBA::tk_double:
         {
            CORBA::Double value;
            return (_value_delegate.extract_double(value)
                    && (value == dyn_any->get_double()));
         }

         case CORBA::tk_boolean:
         {
            CORBA::Boolean value;
            return (_value_delegate.extract_boolean(value)
                    && (value == dyn_any->get_boolean()));
         }

         case CORBA::tk_char:
         {
            CORBA::Char value;
            return (_value_delegate.extract_char(value)
                    && (value == dyn_any->get_char()));
         }

         case CORBA::tk_octet:
         {
            CORBA::Octet value;
            return (_value_delegate.extract_octet(value)
                    && (value == dyn_any->get_octet()));
         }

         case CORBA::tk_longlong:
         {
            CORBA::LongLong value;
            return (_value_delegate.extract_longlong(value)
                    && (value == dyn_any->get_longlong()));
         }

         case CORBA::tk_ulonglong:
         {
            CORBA::ULongLong value;
            return (_value_delegate.extract_ulonglong(value)
                    && (value == dyn_any->get_ulonglong()));
         }

         case CORBA::tk_longdouble:
         {
            CORBA::LongDouble value;
            return (_value_delegate.extract_longdouble(value)
                    && (value == dyn_any->get_ulonglong()));
         }

         case CORBA::tk_wchar:
         {
            CORBA::WChar value;
            return (_value_delegate.extract_wchar(value)
                    && (value == dyn_any->get_wchar()));
         }

         case CORBA::tk_string:
         {
            const CORBA::Char* value;
            char* dyn_any_string = dyn_any->get_string();
            CORBA::Boolean result = (_value_delegate.extract_string(value)
                                     && !strcmp(value, dyn_any_string));
            CORBA::string_free(dyn_any_string);
            return result;
         }

         case CORBA::tk_wstring:
         {
            const CORBA::WChar* value;
            CORBA::WChar* dyn_any_string = dyn_any->get_wstring();
            CORBA::Boolean result = (_value_delegate.extract_wstring(value)
                                     && !wcscmp(value, dyn_any_string));
            CORBA::wstring_free(dyn_any_string);
            return result;
         }
         default: 
            break;
      }
   }
   catch(DynamicAny::DynAny::InvalidValue& iv)
   {
      throw CORBA::BAD_PARAM(iv._name());
   }
   catch(DynamicAny::DynAny::TypeMismatch& tm)
   {
      throw CORBA::INTERNAL(tm._name());
   }

   throw CORBA::INTERNAL("DynAnyImpl with no basic TypeCode");
}




void DynAnyImpl::destroy_now()
   throw(CORBA::SystemException)
{
   DynAnyBase::destroy_now();
}




DynamicAny::DynAny_ptr DynAnyImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   DynAnyImpl* new_dyn = new DynAnyImpl(_factory, _orb, _dyn_type, _base_type);
   try
   {
      new_dyn->assign((DynamicAny::DynAny_ptr) this);
   }
   catch(DynamicAny::DynAny::TypeMismatch)
   {
   }

   return new_dyn;
}




void DynAnyImpl::insert_boolean(CORBA::Boolean value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_boolean)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_boolean(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_octet(CORBA::Octet value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_octet)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_octet(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_char(char value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_char)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_char(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_short(CORBA::Short value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_short)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_short(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_ushort(CORBA::UShort value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_ushort)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_ushort(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_long(CORBA::Long value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_long)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_long(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_ulong(CORBA::ULong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_ulong)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_ulong(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_float(CORBA::Float value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_float)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_float(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_double(CORBA::Double value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_double)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_double(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_string(const char* value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value == NULL)
   {
      throw CORBA::BAD_PARAM("Null string reference", 0, CORBA::COMPLETED_NO);
   }

   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_string)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      if (_base_type->length() == 0) // unbounded string
      {
         _value_delegate.insert_string(value);
         return;
      }

      if (strlen(value) > _base_type->length())
      {
         throw CORBA::BAD_PARAM("String out of bounds",0, CORBA::COMPLETED_NO);
      }

      _value_delegate.insert_string(value);
   }
   catch(CORBA::TypeCode::BadKind)
   {
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_wstring(const CORBA::WChar* value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value == NULL)
   {
      throw CORBA::BAD_PARAM("Null string reference", 0, CORBA::COMPLETED_NO);
   }

   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_wstring)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      if (_base_type->length() == 0) // unbounded wstring
      {
         _value_delegate.insert_wstring(value);
         return;
      }

      if (wcslen(value) > (_base_type->length()/sizeof(CORBA::WChar)))
      {
         throw CORBA::BAD_PARAM("WString out of bounds", 0, CORBA::COMPLETED_NO);
      }

      _value_delegate.insert_wstring(value);
   }
   catch(CORBA::TypeCode::BadKind)
   {
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_reference(CORBA::Object_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_objref)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_Object(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_typecode(CORBA::TypeCode_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   //jagd
   //if (CORBA::is_nil(value))
   if (!(value))
   {
      throw CORBA::BAD_PARAM("Null TypeCode reference", 0, CORBA::COMPLETED_NO);
   }

   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_TypeCode)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_TypeCode(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_longlong(CORBA::LongLong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_longlong)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_longlong(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_ulonglong(CORBA::ULongLong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_ulonglong)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_ulonglong(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_longdouble(CORBA::LongDouble value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_longdouble)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_longdouble(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}





void DynAnyImpl::insert_wchar(CORBA::WChar value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_wchar)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_wchar(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_any)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_any(value); // copying
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::insert_dyn_any(DynamicAny::DynAny_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   //jagd
   //if (CORBA::is_nil(value))
   if (!(value))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_any)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      _value_delegate.insert_any(value->to_any()); // non-copying
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Boolean DynAnyImpl::get_boolean()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_boolean)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Boolean value;
      if (! _value_delegate.extract_boolean(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Octet DynAnyImpl::get_octet()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_octet)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Octet value;
      if (! _value_delegate.extract_octet(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




char DynAnyImpl::get_char()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_char)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      char value;
      if (! _value_delegate.extract_char(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Short DynAnyImpl::get_short()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_short)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Short value;
      if (! _value_delegate.extract_short(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::UShort DynAnyImpl::get_ushort()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_ushort)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::UShort value;
      if (! _value_delegate.extract_ushort(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Long DynAnyImpl::get_long()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_long)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Long value;
      if (! _value_delegate.extract_long(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::ULong DynAnyImpl::get_ulong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_ulong)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::ULong value;
      if (! _value_delegate.extract_ulong(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Float DynAnyImpl::get_float()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_float)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Float value;
      if (! _value_delegate.extract_float(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Double DynAnyImpl::get_double()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_double)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Double value;
      if (! _value_delegate.extract_double(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




char* DynAnyImpl::get_string()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_string)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      const CORBA::Char* value;
      if (! _value_delegate.extract_string(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return CORBA::string_dup(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::WChar* DynAnyImpl::get_wstring()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_wstring)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      const CORBA::WChar* value;
      if (! _value_delegate.extract_wstring(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return CORBA::wstring_dup(value);
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Object_ptr DynAnyImpl::get_reference()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_objref)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::Object_ptr value;
      if (! _value_delegate.extract_Object(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::TypeCode_ptr DynAnyImpl::get_typecode()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_TypeCode)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::TypeCode_ptr value;
      if (! _value_delegate.extract_TypeCode(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::LongLong DynAnyImpl::get_longlong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_longlong)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::LongLong value;
      if (! _value_delegate.extract_longlong(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::ULongLong DynAnyImpl::get_ulonglong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_ulonglong)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::ULongLong value;
      if (! _value_delegate.extract_ulonglong(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::LongDouble DynAnyImpl::get_longdouble()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_longdouble)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::LongDouble value;
      if (! _value_delegate.extract_longdouble(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::WChar DynAnyImpl::get_wchar()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_wchar)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      CORBA::WChar value;
      if (! _value_delegate.extract_wchar(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }
      return value;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Any* DynAnyImpl::get_any()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_any)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      const CORBA::Any* value;

      if (! _value_delegate.extract_any(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }

      CORBA::Any* new_any = _orb->create_any();
      *new_any = *value;

      return new_any;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




DynamicAny::DynAny_ptr DynAnyImpl::get_dyn_any()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_base_type->kind() != CORBA::tk_any)
   {
      throw DynamicAny::DynAny::TypeMismatch(); // "Unexpected type"
   }

   try
   {
      const CORBA::Any* value;
      if (! _value_delegate.extract_any(value))
      {
         throw DynamicAny::DynAny::InvalidValue();
      }

      DynamicAny::DynAny_ptr new_dyn_any = _factory->create_dyn_any(*value);
      delete value;
      return new_dyn_any;
   }
   catch(CORBA::BAD_OPERATION& bo)
   {
      throw DynamicAny::DynAny::InvalidValue();
   }
}




void DynAnyImpl::init_value(const CORBA::Any& any)
   throw(CORBA::SystemException)
{
   try
   {
      TIDorb::core::AnyImpl& the_any =
         dynamic_cast<TIDorb::core::AnyImpl&>(any.delegate());

      if (_dyn_type->kind() == CORBA::tk_alias)
      {
         TIDorb::portable::InputStream* istream = the_any.create_input_stream();
      	 // FIX bug [#393] Error processing a return of DII invocation
         // Call read_value with skip optimization
         _value_delegate.read_value(*istream, _base_type, true);
         delete istream;
      }
      else
      {
         _value_delegate.assign(the_any);
         // Fix bug [#392] Any::type(tc) reset any value
         //_value_delegate.set_equivalent_type(_base_type);
         _value_delegate.type(_base_type);
      }
   }
   catch(bad_cast)
   {
      throw CORBA::INTERNAL();
   }
}




void DynAnyImpl::init_value()
   throw(CORBA::SystemException)
{
   switch (_base_type->kind())
   {
      case CORBA::tk_null:
      case CORBA::tk_void:
      {
         return;
      }

      // basic typecodes
      case CORBA::tk_short:
      {
         _value_delegate.insert_short((CORBA::Short) 0);
         return;
      }

      case CORBA::tk_long:
      {
         _value_delegate.insert_long((CORBA::Long) 0);
         return;
      }

      case CORBA::tk_ushort:
      {
         _value_delegate.insert_ushort((CORBA::UShort) 0);
         return;
      }

      case CORBA::tk_ulong:
      {
         _value_delegate.insert_ulong((CORBA::ULong) 0);
         return;
      }

      case CORBA::tk_float:
      {
         _value_delegate.insert_float((CORBA::Float) 0);
         return;
      }

      case CORBA::tk_double:
      {
         _value_delegate.insert_double((CORBA::Double) 0);
         return;
      }

      case CORBA::tk_boolean:
      {
         _value_delegate.insert_boolean(false);
         return;
      }

      case CORBA::tk_char:
      {
         _value_delegate.insert_char((CORBA::Char) 0);
         return;
      }

      case CORBA::tk_octet:
      {
         _value_delegate.insert_octet((CORBA::Octet) 0);
         return;
      }

      case CORBA::tk_longlong:
      {
         _value_delegate.insert_longlong((CORBA::LongLong) 0);
         return;
      }

      case CORBA::tk_ulonglong:
      {
         _value_delegate.insert_ulonglong((CORBA::ULongLong) 0);
         return;
      }

      case CORBA::tk_longdouble:
      {
         _value_delegate.insert_longdouble((CORBA::LongDouble) 0);
         return;
      }

      case CORBA::tk_wchar:
      {
         _value_delegate.insert_wchar((CORBA::WChar) 0);
         return;
      }

      case CORBA::tk_string:
      {
         _value_delegate.insert_string((const CORBA::Char*) "");
         return;
      }

      case CORBA::tk_wstring:
      {
         _value_delegate.insert_wstring((const CORBA::WChar*) L"");
         return;
      }

      case CORBA::tk_objref:
      {
         //jagd
         //CORBA::Object_var obj = CORBA::Object::_nil();
         CORBA::Object_var obj = 0;
         _value_delegate.insert_Object(obj);
         return;
      }

      case CORBA::tk_any:
      {
         CORBA::Any* any = _orb->create_any();
         _value_delegate.insert_any(any);
         return;
      }

      case CORBA::tk_TypeCode:
      {
         CORBA::TypeCode_ptr type= CORBA::TypeCode::_duplicate(CORBA::_tc_null);
         _value_delegate.insert_TypeCode(type);
         return;
      }

      default:
         throw CORBA::INTERNAL("TypeCode not expected");
   }
}




} //namespace dynAny
} //namespace TIDorb
