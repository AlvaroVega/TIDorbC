/////////////////////////////////////////////////////////////////////////
//
// File:        DynUnionImpl.h
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


#ifndef __TIDORB_DYN_UNION_IMPL_H__
#define __TIDORB_DYN_UNION_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynUnionImpl : public DynamicAny::DynUnion,
                     public CORBA::LocalObject,
                     public TIDThr::RefCounter, 
                     public DynAnyBase
{
   public:
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

      // DynUnion operations
      DynamicAny::DynAny_ptr get_discriminator()
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void set_discriminator(DynamicAny::DynAny_ptr d)
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

      void set_to_default_member()
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

      void set_to_no_active_member()
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

      CORBA::Boolean has_no_active_member()
         throw(CORBA::SystemException);

      CORBA::TCKind discriminator_kind()
         throw(CORBA::SystemException);

      DynamicAny::DynAny_ptr member()
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      DynamicAny::FieldName member_name()
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void member_name(DynamicAny::FieldName name)
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::TCKind member_kind()
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Boolean is_set_to_default_member();

      DynamicAny::DynAny_ptr current_component()
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

      // DynAny operations
      void insert_boolean(CORBA::Boolean value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_char(char value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_wchar(CORBA::WChar value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_octet(CORBA::Octet value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_short(CORBA::Short value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_ushort(CORBA::UShort value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_long(CORBA::Long value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_ulong(CORBA::ULong value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_longlong(CORBA::LongLong value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_ulonglong(CORBA::ULongLong value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_longdouble(CORBA::LongDouble value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_float(CORBA::Float value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_double(CORBA::Double value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_string(const char* value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_wstring(const CORBA::WChar* value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_any(const CORBA::Any& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_dyn_any(DynamicAny::DynAny_ptr value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_typecode(CORBA::TypeCode_ptr value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_reference(CORBA::Object_ptr value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Boolean get_boolean()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      char get_char()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::WChar get_wchar()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Octet get_octet()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Short get_short()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::UShort get_ushort()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Long get_long()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ULong get_ulong()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::LongLong get_longlong()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ULongLong get_ulonglong()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::LongDouble get_longdouble()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Float get_float()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Double get_double()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      char* get_string()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::WChar* get_wstring()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Any* get_any()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      DynamicAny::DynAny_ptr get_dyn_any()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::TypeCode_ptr get_typecode()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::Object_ptr get_reference()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // DynAny without components methods
      CORBA::ULong component_count()
         throw(CORBA::SystemException);

      CORBA::Boolean seek(CORBA::Long index)
         throw(CORBA::SystemException);

      void rewind()
         throw(CORBA::SystemException);

      CORBA::Boolean next()
         throw(CORBA::SystemException);

      // Streamable operations
      void _read(TIDorb::portable::InputStream& in);
      void _write(TIDorb::portable::OutputStream& out) const;

      // LocalObject operations
      CORBA::Boolean _is_a(const char* repositoryIdentifier)
         throw(CORBA::SystemException);

   public:
      virtual ~DynUnionImpl()
        throw (TIDThr::SystemException);

   protected:
      DynUnionImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

      // Constructor. Gets an any object for reading its value.
      // It assumes that the any contains an struct TypeCode.
      DynUnionImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                   const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      // Constructor. Gets a simple TypeCode to create a new value.
      DynUnionImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                   CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      void set_index(CORBA::Long index, DynamicAny::DynAny_ptr discriminator)
         throw(CORBA::SystemException);

      void destroy_now()
         throw(CORBA::SystemException);

      CORBA::Boolean all_cases_used()
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

      void set_discriminator_value(CORBA::Any& discriminator,
                                   CORBA::LongLong value)
         throw(CORBA::SystemException);

   private:
      // Active member
      DynAnyBase* _member;

      // Active discriminator
      DynAnyBase* _dyn_discriminator;

      // Member index in the union TypeCode
      CORBA::Long _member_index;

      // Current component
      CORBA::Long _current;

      // Component count
      CORBA::Long _component_count;

   friend class DynAnyFactoryImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_UNION_IMPL_H__
