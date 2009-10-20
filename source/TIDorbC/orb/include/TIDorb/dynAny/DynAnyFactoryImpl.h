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
