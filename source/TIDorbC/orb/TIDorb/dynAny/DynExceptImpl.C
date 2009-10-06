/////////////////////////////////////////////////////////////////////////
//
// File:        DynExceptionImpl.C
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


namespace TIDorb {
namespace dynAny {




DynExceptImpl::DynExceptImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynStructImpl(factory, orb)
{
}




DynExceptImpl::DynExceptImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb,
                             const CORBA::Any& any,
                             CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynStructImpl(factory, orb, any, real_type)
{
   if (! validate_name())
   {
      throw CORBA::MARSHAL("Unexpected exception id in any");
   }
}




DynExceptImpl::DynExceptImpl(DynamicAny::DynAnyFactory_ptr factory,
                             TIDorb::core::TIDORB* orb,
                             CORBA::TypeCode_ptr type,
                             CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynStructImpl(factory, orb, type, real_type)
{
}




CORBA::Boolean DynExceptImpl::validate_name()
   throw(CORBA::SystemException)
{
   try
   {
      CORBA::String_var str;
      _next_value->read_string(str.out());
      return (!strcmp(str, _base_type->id()));
   }
   catch(CORBA::TypeCode::BadKind)
   {
      throw CORBA::BAD_TYPECODE();
   }
}


void DynExceptImpl::from_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue,CORBA::SystemException)
{
   DynStructImpl::from_any(value);

   if (!validate_name())
   {
      throw CORBA::MARSHAL("Unexpected exception id in any");
   }
}

void DynExceptImpl::_read(TIDorb::portable::InputStream& in)
{
   DynStructImpl::_read(in);

   if (!validate_name())
   {
      throw CORBA::MARSHAL("Unexpected exception id in any");
   }
}




void DynExceptImpl::_write(TIDorb::portable::OutputStream& out) const
{
   try
   {
      out.write_string(_base_type->id());
   }
   catch(CORBA::TypeCode::BadKind)
   {
      throw CORBA::BAD_TYPECODE();
   }

   DynStructImpl::_write(out);
}




DynamicAny::DynAny_ptr DynExceptImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynExceptImpl* new_dyn =
      new DynExceptImpl(_factory, _orb, _dyn_type, _base_type);

   copy_to(*new_dyn);

   if (new_dyn->_next_value)
   {
      new_dyn->_next_value->skip_string();
   }

   return new_dyn;
}




} //namespace dynAny
} //namespace TIDorb
