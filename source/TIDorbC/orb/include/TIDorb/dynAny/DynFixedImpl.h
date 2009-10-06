/////////////////////////////////////////////////////////////////////////
//
// File:        DynFixedImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/dynAny.h"


#ifndef __TIDORB_DYN_FIXED_IMPL_H__
#define __TIDORB_DYN_FIXED_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynFixedImpl : public DynamicAny::DynFixed,
                     public CORBA::LocalObject,
                     public TIDThr::RefCounter,
                     public DynSingleton
{
   public:
      // DynFixed operations
      char* get_value()
         throw(CORBA::SystemException);

      CORBA::Boolean set_value(const char* val)
        throw(DynamicAny::DynAny::TypeMismatch,
              DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // DynAny operations
      void assign(DynamicAny::DynAny_ptr dyn_any)
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

      void from_any(const CORBA::Any& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Any* to_any()
         throw(CORBA::SystemException);

      CORBA::Boolean equal(DynamicAny::DynAny_ptr dyn_any)
         throw(CORBA::SystemException);

      DynamicAny::DynAny_ptr copy()
         throw(CORBA::SystemException);

      // Streamable operations
      void _read(TIDorb::portable::InputStream& in);
      void _write(TIDorb::portable::OutputStream& out) const;

      // LocalObject operations
      CORBA::Boolean _is_a(const char* repositoryIdentifier)
         throw(CORBA::SystemException);

   protected:
      // Constructor. Gets an any object for reading its value.
      DynFixedImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                   const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      DynFixedImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                   CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

   public:
      virtual ~DynFixedImpl()
        throw (TIDThr::SystemException);

   private:
      DynFixedImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

   private:
      CORBA::Fixed _fixed_value;
      CORBA::UShort _digits;
      CORBA::Short _scale;

      friend class DynAnyFactoryImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_FIXED_IMPL_H__
