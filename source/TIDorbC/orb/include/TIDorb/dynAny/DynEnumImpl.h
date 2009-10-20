/////////////////////////////////////////////////////////////////////////
//
// File:        DynEnumImpl.h
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


#ifndef __TIDORB_DYN_ENUM_IMPL_H__
#define __TIDORB_DYN_ENUM_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynEnumImpl : public DynamicAny::DynEnum,
                    public CORBA::LocalObject,
                    public TIDThr::RefCounter, 
                    public DynSingleton
{
   public:
      // DynEnum operations
      char* get_as_string()
         throw(CORBA::SystemException);

      void set_as_string(const char* value)
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ULong get_as_ulong()
         throw(CORBA::SystemException);

      void set_as_ulong(CORBA::ULong value)
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

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

   public:
      // Destructor
      virtual ~DynEnumImpl()
        throw (TIDThr::SystemException);

   protected:
      // Constructor. Gets an any object for reading its value
      DynEnumImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                  const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      // Constructor. Gets a simple TypeCode to create a new value;
      // It assumes that the TypeCode contains is simple (octect, short ...).
      DynEnumImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                  CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

   private:
      DynEnumImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

   private:
      CORBA::ULong _enum_value;

   friend class DynAnyFactoryImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_ENUM_IMPL_H__
