/////////////////////////////////////////////////////////////////////////
//
// File:        DynAnyFactoryImpl.C
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
#include <stdio.h>
#include <string.h>


#define BUFFSIZE                2000
#define DYN_FACTORY_REP_ID      "IDL:omg.org/DynamicAny/DynAnyFactory:1.0"


namespace TIDorb {
namespace dynAny {




DynAnyFactoryImpl::DynAnyFactoryImpl()
   throw(CORBA::SystemException)
{
   _orb = NULL;
   _destroyed = false;
}




DynAnyFactoryImpl::~DynAnyFactoryImpl()
{
}




void DynAnyFactoryImpl::destroy()
{
  TIDThr::Synchronized sync(_mutex);

  _orb = NULL;
  _destroyed = true;
}




void DynAnyFactoryImpl::set_orb(CORBA::ORB_ptr orb)
   throw(CORBA::SystemException)
{
   //jagd
   //_orb = dynamic_cast<TIDorb::core::TIDORB*>(orb);
   _orb = (TIDorb::core::TIDORB*)(orb);
}




DynamicAny::DynAny_ptr
DynAnyFactoryImpl::create_dyn_any(const CORBA::Any& value)
   throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException)
{
   TIDThr::Synchronized sync(_mutex);

   if(_destroyed)
     throw CORBA::OBJECT_NOT_EXIST();

   CORBA::TypeCode_var type = value.type();
   switch (type->kind())
   {
      // basic typecodes
      case CORBA::tk_null:
      case CORBA::tk_any:
      case CORBA::tk_objref:
      case CORBA::tk_TypeCode:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long:
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_octet:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_longdouble:
      case CORBA::tk_wchar:
      case CORBA::tk_string:
      case CORBA::tk_wstring:
         return new DynAnyImpl(this, _orb, value, type);

      case CORBA::tk_alias:
         return create_alias_dyn_any(value);

      // complex typecodes

      case CORBA::tk_sequence:
         return new DynSequenceImpl(this, _orb, value, type);

      case CORBA::tk_struct:
         return new DynStructImpl(this, _orb, value, type);

      case CORBA::tk_except:
         return new DynExceptImpl(this, _orb, value, type);

      case CORBA::tk_enum:
         return new DynEnumImpl(this, _orb, value, type);

      case CORBA::tk_array:
         return new DynArrayImpl(this, _orb, value, type);

      case CORBA::tk_union:
         return new DynUnionImpl(this, _orb, value, type);

      case CORBA::tk_value:
         return new DynValueImpl(this, _orb, value, type);

      case CORBA::tk_fixed:
         return new DynFixedImpl(this, _orb, value, type);

      default:
         throw DynamicAny::DynAnyFactory::InconsistentTypeCode();// Bad TypeCode
   }
}




DynamicAny::DynAny_ptr
DynAnyFactoryImpl::create_alias_dyn_any(const CORBA::Any& value)
   throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException)
{
  TIDThr::Synchronized sync(_mutex);

  if(_destroyed)
    throw CORBA::OBJECT_NOT_EXIST();

   CORBA::TypeCode_var base_type;
   try
   {
      base_type = value.type();
      do
      {
         base_type = base_type->content_type();
      }
      while (base_type->kind() == CORBA::tk_alias);
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      // "Alias typecode fault in content_type() method: %s", bk._name());
      throw CORBA::BAD_TYPECODE();
   }

   switch (base_type->kind())
   {
      // basic typecodes
      case CORBA::tk_null:
      case CORBA::tk_any:
      case CORBA::tk_objref:
      case CORBA::tk_TypeCode:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long:
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_octet:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_longdouble:
      case CORBA::tk_wchar:
      case CORBA::tk_string:
      case CORBA::tk_wstring:
         return new DynAnyImpl(this, _orb, value, base_type);

      // complex typecodes
      case CORBA::tk_struct:
         return new DynStructImpl(this, _orb, value, base_type);

      case CORBA::tk_sequence:
         return new DynSequenceImpl(this, _orb, value, base_type);

      case CORBA::tk_except:
         return new DynExceptImpl(this, _orb, value, base_type);

      case CORBA::tk_enum:
         return new DynEnumImpl(this, _orb, value, base_type);

      case CORBA::tk_array:
         return new DynArrayImpl(this, _orb, value, base_type);

      case CORBA::tk_union:
         return new DynUnionImpl(this, _orb, value, base_type);

      case CORBA::tk_fixed:
         return new DynFixedImpl(this, _orb, value, base_type);

      case CORBA::tk_value:
         return new DynValueImpl(this, _orb, value, base_type);


      default:
         throw DynamicAny::DynAnyFactory::InconsistentTypeCode();
    }
}




DynamicAny::DynAny_ptr
DynAnyFactoryImpl::create_dyn_any_from_type_code(CORBA::TypeCode_ptr type)
   throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException)
{
  TIDThr::Synchronized sync(_mutex);

  if(_destroyed)
    throw CORBA::OBJECT_NOT_EXIST();

   //jagd 
   //if (CORBA::is_nil(type))
   if (!(type))
   {
      throw CORBA::BAD_TYPECODE("Null TypeCode reference");
   }

   switch (type->kind())
   {
      // basic typecodes
      case CORBA::tk_null:
      case CORBA::tk_any:
      case CORBA::tk_objref:
      case CORBA::tk_TypeCode:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long:
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_octet:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_longdouble:
      case CORBA::tk_wchar:
      case CORBA::tk_string:
      case CORBA::tk_wstring:
         return new DynAnyImpl(this, _orb, type, type);

      case CORBA::tk_alias:
         return create_alias_dyn_any_from_type_code(type);

      // complex typecodes
      case CORBA::tk_struct:
         return new DynStructImpl(this, _orb, type, type);

      case CORBA::tk_sequence:
         return new DynSequenceImpl(this, _orb, type, type);

      case CORBA::tk_except:
         return new DynExceptImpl(this, _orb, type, type);

      case CORBA::tk_enum:
         return new DynEnumImpl(this, _orb, type, type);

      case CORBA::tk_array:
         return new DynArrayImpl(this, _orb, type, type);

      case CORBA::tk_union:
         return new DynUnionImpl(this, _orb, type, type);

      case CORBA::tk_fixed:
         return new DynFixedImpl(this, _orb, type, type);

      case CORBA::tk_value:
         return new DynValueImpl(this, _orb, type, type);

      default:
         throw DynamicAny::DynAnyFactory::InconsistentTypeCode();
   }
}




DynamicAny::DynAny_ptr
DynAnyFactoryImpl::create_alias_dyn_any_from_type_code(CORBA::TypeCode_ptr type)
   throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException)
{
  //jagd
  //if (CORBA::is_nil(type))
  if (!(type))
  {
    throw CORBA::BAD_TYPECODE("Null TypeCode reference");
  }

  CORBA::TypeCode_var base_type;
  try
  {
    base_type = type->content_type();
    while (base_type->kind() == CORBA::tk_alias)
    {
      base_type = base_type->content_type();
    }
  }
  catch(CORBA::TypeCode::BadKind& bk)
  {
    // "Alias typecode fault in content_type() method: %s", bk._name());
     throw CORBA::BAD_TYPECODE();
  }

  switch (base_type->kind())
  {
    // basic typecodes
    case CORBA::tk_null:
    case CORBA::tk_any:
    case CORBA::tk_objref:
    case CORBA::tk_TypeCode:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_long:
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
    case CORBA::tk_string:
    case CORBA::tk_wstring:
       return new DynAnyImpl(this, _orb, type, base_type);

    // complex typecodes
    case CORBA::tk_struct:
       return new DynStructImpl(this, _orb, type, base_type);

    case CORBA::tk_sequence:
       return new DynSequenceImpl(this, _orb, type, base_type);

    case CORBA::tk_enum:
       return new DynEnumImpl(this, _orb, type, base_type);

    case CORBA::tk_except:
       return new DynExceptImpl(this, _orb, type, base_type);

    case CORBA::tk_array:
       return new DynArrayImpl(this, _orb, type, base_type);

    case CORBA::tk_union:
       return new DynUnionImpl(this, _orb, type, base_type);

    case CORBA::tk_fixed:
       return new DynFixedImpl(this, _orb, type, base_type);
  
    case CORBA::tk_value:
       return new DynValueImpl(this, _orb, type, base_type);

    default:
       throw DynamicAny::DynAnyFactory::InconsistentTypeCode();
  }
}


CORBA::Boolean DynAnyFactoryImpl::is_basic(CORBA::TypeCode_ptr type)
   throw (CORBA::SystemException)
{
   //jagd
   //if (CORBA::is_nil(type))
   if (!(type))
   {
      throw CORBA::BAD_PARAM("Null TypeCode reference");
   }

   switch (type->kind())
   {
      // basic typecodes
      case CORBA::tk_null:
      case CORBA::tk_any:
      case CORBA::tk_objref:
      case CORBA::tk_TypeCode:
      case CORBA::tk_void:
      case CORBA::tk_short:
      case CORBA::tk_long:
      case CORBA::tk_ushort:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_double:
      case CORBA::tk_boolean:
      case CORBA::tk_char:
      case CORBA::tk_octet:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
      case CORBA::tk_wchar:
      case CORBA::tk_string:
      case CORBA::tk_wstring:
         return true;

      case CORBA::tk_alias:
         return alias_with_basic_tc(type);

      default:
         return false;
   }
}




CORBA::Boolean DynAnyFactoryImpl::alias_with_basic_tc(CORBA::TypeCode_ptr type)
   throw (CORBA::SystemException)
{
   //jagd
   //if (CORBA::is_nil(type))
   if (!(type))
   {
       throw CORBA::BAD_PARAM("Null TypeCode reference");
   }

   CORBA::TypeCode_var base_type;
   try
   {
      base_type = type->content_type();
      while (base_type->kind() == CORBA::tk_alias)
      {
         base_type = base_type->content_type();
      }
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      //         "Alias typecode fault in content_type() method: %s", bk._name());
      throw CORBA::BAD_TYPECODE();
   }

   return is_basic(base_type);
}




CORBA::Boolean DynAnyFactoryImpl::_is_a(const char* repositoryIdentifier)
   throw(CORBA::SystemException)
{
   if (repositoryIdentifier == NULL)
   {
      throw CORBA::BAD_PARAM("Null string reference");
   }

   if (!strcmp(repositoryIdentifier, DYN_FACTORY_REP_ID))
   {
      return true;
   }
   else
   {
     return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
   }
}




CORBA::Boolean DynAnyFactoryImpl::_is_equivalent(CORBA::Object_ptr other)
   throw(CORBA::SystemException)
{
   //jagd
   //if (CORBA::is_nil(other))
   if (!(other))
   {
      throw CORBA::BAD_PARAM("Null Object reference");
   }

   return (this == other);
}




} //namespace dynAny
} //namespace TIDorb
