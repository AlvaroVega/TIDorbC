/////////////////////////////////////////////////////////////////////////
//
// File:        DynStructImpl.h
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


#ifndef __TIDORB_DYN_STRUCT_IMPL_H__
#define __TIDORB_DYN_STRUCT_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynStructImpl : public DynamicAny::DynStruct,
                      public CORBA::LocalObject,
                      public TIDThr::RefCounter, 
                      public DynComposite
{
   public:
      DynamicAny::DynAny_ptr copy()
         throw(CORBA::SystemException);

      // DynStruct operations
      virtual DynamicAny::FieldName current_member_name()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      virtual CORBA::TCKind current_member_kind()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      virtual DynamicAny::NameValuePairSeq* get_members()
         throw(CORBA::SystemException);

      virtual void set_members(const DynamicAny::NameValuePairSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      virtual DynamicAny::NameDynAnyPairSeq* get_members_as_dyn_any()
         throw(CORBA::SystemException);

      virtual void set_members_as_dyn_any(
         const DynamicAny::NameDynAnyPairSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // LocalObject operations
      CORBA::Boolean _is_a(const char* repositoryIdentifier)
         throw(CORBA::SystemException);

   public:
      // Destructor
      virtual ~DynStructImpl()
        throw (TIDThr::SystemException);

   protected:
      // Empty Constructor for generate copies
      DynStructImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

      // Constructor. Gets an any object for reading its value.
      // It assumes that the any contains an struct TypeCode.
      DynStructImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                    const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      // Constructor. Gets a simple TypeCode to create a new value.
      // It assumes that the TypeCode is tk_struct or tk_exception
      DynStructImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                    CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      CORBA::TypeCode_ptr get_component_type(CORBA::ULong position)
         throw(CORBA::SystemException);

   friend class DynAnyFactoryImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_STRUCT_IMPL_H__
