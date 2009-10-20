/////////////////////////////////////////////////////////////////////////
//
// File:        DynAnyBase.h
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


#ifndef __TIDORB_DYN_ANY_BASE_H__
#define __TIDORB_DYN_ANY_BASE_H__


namespace TIDorb {
namespace dynAny {


class DynAnyBase : public virtual DynamicAny::DynAny,
                   public TIDorb::portable::Streamable
{
   public:
      virtual ~DynAnyBase();

      // DynAny operations

      CORBA::TypeCode_ptr type()
        throw(CORBA::SystemException);

      // Destroy this DynAny
      void destroy()
         throw(CORBA::SystemException);

      void insert_abstract(CORBA::AbstractBase_ptr value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_val(CORBA::ValueBase_ptr value)
        throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_boolean_seq(const CORBA::BooleanSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_octet_seq(const CORBA::OctetSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_char_seq(const CORBA::CharSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_short_seq(const CORBA::ShortSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_ushort_seq(const CORBA::UShortSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_long_seq(const CORBA::LongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_ulong_seq(const CORBA::ULongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_float_seq(const CORBA::FloatSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_double_seq(const CORBA::DoubleSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_longlong_seq(const CORBA::LongLongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_ulonglong_seq(const CORBA::ULongLongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_longdouble_seq(const CORBA::LongDoubleSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      void insert_wchar_seq(const CORBA::WCharSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::AbstractBase_ptr get_abstract()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ValueBase_ptr get_val()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::BooleanSeq* get_boolean_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::OctetSeq* get_octet_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::CharSeq* get_char_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::ShortSeq* get_short_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::UShortSeq* get_ushort_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::LongSeq* get_long_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::ULongSeq* get_ulong_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::FloatSeq* get_float_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::DoubleSeq* get_double_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::LongLongSeq* get_longlong_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::ULongLongSeq* get_ulonglong_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::LongDoubleSeq* get_longdouble_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      CORBA::WCharSeq* get_wchar_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue,CORBA::SystemException);

      // DynAny without components methods

      CORBA::ULong component_count()
         throw(CORBA::SystemException);

      CORBA::Boolean seek(CORBA::Long index)
         throw(CORBA::SystemException);

      void rewind()
         throw(CORBA::SystemException);

      CORBA::Boolean next()
         throw(CORBA::SystemException);

      DynamicAny::DynAny_ptr current_component()
         throw(DynamicAny::DynAny::TypeMismatch,CORBA::SystemException);

      // Streamable operations

      CORBA::TypeCode_ptr _type() const;

   protected:
      DynAnyBase(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

      DynAnyBase(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                 CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      void destroy_now()
         throw();

      void set_user_destroy(CORBA::Boolean value)
         throw(CORBA::SystemException);

      virtual void type_mismatch()
         throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException);

   protected:
      // The active ORB
      TIDorb::core::TIDORB_ref _orb;

      // The active DynAny factory
      DynamicAny::DynAnyFactory_var _factory;

      // The DynAny typecode. It maybe different from the base_type typecode,
      // if it is a alias typecode
      CORBA::TypeCode_var _dyn_type;

      // The "real" DynAny typecode base type
      CORBA::TypeCode_var _base_type;

      // If true the DynAny can be destroyed by the user. Otherwise, it only
      // will be destroyed by the container DynAny that created it.
      // Default value: true (user can destroy it)
      CORBA::Boolean _user_destroy;

      // If true, the DynAny is destroyed
      CORBA::Boolean _destroyed;

   friend class DynComposite;
   friend class DynUnionImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_ANY_BASE_H__
