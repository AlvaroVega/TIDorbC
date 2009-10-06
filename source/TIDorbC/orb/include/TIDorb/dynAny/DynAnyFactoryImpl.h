/////////////////////////////////////////////////////////////////////////
//
// File:        DynAnyFactoryImpl.h
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


#ifndef __TIDORB_DYN_ANY_FACTORY_IMPL_H__
#define __TIDORB_DYN_ANY_FACTORY_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynAnyFactoryImpl : public DynamicAny::DynAnyFactory,
                          public CORBA::LocalObject
{
   public:
      // DynAnyFactory operations
      DynamicAny::DynAny_ptr
      create_dyn_any(const CORBA::Any& value)
         throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException);

      DynamicAny::DynAny_ptr
      create_dyn_any_from_type_code(CORBA::TypeCode_ptr type)
         throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException);

      // LocalObject operations
      CORBA::Boolean _is_a(const char* repositoryIdentifier)
         throw(CORBA::SystemException);

      CORBA::Boolean _is_equivalent(CORBA::Object_ptr other)
         throw(CORBA::SystemException);

   public:
      DynAnyFactoryImpl()
         throw(CORBA::SystemException);

      virtual ~DynAnyFactoryImpl();

      void set_orb(CORBA::ORB_ptr orb)
         throw(CORBA::SystemException);

      void destroy();

      static CORBA::Boolean is_basic(CORBA::TypeCode_ptr type) throw (CORBA::SystemException);

      DynamicAny::DynAny_ptr create_alias_dyn_any_from_type_code(
         CORBA::TypeCode_ptr type)
         throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException);

   protected:
      DynamicAny::DynAny_ptr
      create_alias_dyn_any(const CORBA::Any& value)
         throw(DynamicAny::DynAnyFactory::InconsistentTypeCode, CORBA::SystemException);

      static CORBA::Boolean alias_with_basic_tc(CORBA::TypeCode_ptr type) throw (CORBA::SystemException);

   private:
      TIDThr::RecursiveMutex   _mutex;
      TIDorb::core::TIDORB_ref _orb;
      bool                     _destroyed;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_ANY_FACTORY_IMPL_H__
