/////////////////////////////////////////////////////////////////////////
//
// File:        DynSequenceImpl.h
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


#ifndef __TIDORB_DYN_SEQUENCE_IMPL_H__
#define __TIDORB_DYN_SEQUENCE_IMPL_H__


namespace TIDorb {
namespace dynAny {


class DynSequenceImpl : public DynamicAny::DynSequence,
                        public CORBA::LocalObject,
                        public TIDThr::RefCounter,
                        public DynComposite
{
   public:
      DynamicAny::DynAny_ptr copy()
         throw(CORBA::SystemException);

      void from_any(const CORBA::Any& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // DynSequence operations
      CORBA::ULong get_length()
         throw(CORBA::SystemException);

      void set_length(CORBA::ULong len)
         throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      DynamicAny::AnySeq* get_elements()
         throw(CORBA::SystemException);

      void set_elements(const DynamicAny::AnySeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      DynamicAny::DynAnySeq* get_elements_as_dyn_any()
         throw(CORBA::SystemException);

      void set_elements_as_dyn_any(const DynamicAny::DynAnySeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // CORBA 2.5
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
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_long_seq(const CORBA::LongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_ulong_seq(const CORBA::ULongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_float_seq(const CORBA::FloatSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_double_seq(const CORBA::DoubleSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_longlong_seq(const CORBA::LongLongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_ulonglong_seq(const CORBA::ULongLongSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_longdouble_seq(const CORBA::LongDoubleSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      void insert_wchar_seq(const CORBA::WCharSeq& value)
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::BooleanSeq* get_boolean_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::OctetSeq* get_octet_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::CharSeq* get_char_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ShortSeq* get_short_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::UShortSeq* get_ushort_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::LongSeq* get_long_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ULongSeq* get_ulong_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::FloatSeq* get_float_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::DoubleSeq* get_double_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::LongLongSeq* get_longlong_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::ULongLongSeq* get_ulonglong_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::LongDoubleSeq* get_longdouble_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      CORBA::WCharSeq* get_wchar_seq()
         throw(DynamicAny::DynAny::TypeMismatch,
               DynamicAny::DynAny::InvalidValue, CORBA::SystemException);

      // Streamable operations
      void _read(TIDorb::portable::InputStream& in);
      void _write(TIDorb::portable::OutputStream& out) const;

      // LocalObject operations
      CORBA::Boolean _is_a(const char* repositoryIdentifier)
         throw(CORBA::SystemException);
   public:
      // Destructor
      virtual ~DynSequenceImpl()
        throw (TIDThr::SystemException);

   protected:
      // Empty Constructor for generate copies
      DynSequenceImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb)
         throw(CORBA::SystemException);

      // Constructor. Gets a TypeCode to create a new value.
      // Warning: It assumes that the TypeCode is tk_sequence.
      DynSequenceImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                      CORBA::TypeCode_ptr type, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      DynSequenceImpl(DynamicAny::DynAnyFactory_ptr factory, TIDorb::core::TIDORB* orb,
                      const CORBA::Any& any, CORBA::TypeCode_ptr real_type)
         throw(CORBA::SystemException);

      void extract_length()
         throw(CORBA::SystemException);

      CORBA::TypeCode_ptr get_component_type(CORBA::ULong position)
         throw(CORBA::SystemException);

   private:
      CORBA::Boolean _bounded;
      CORBA::Long    _bound;

   friend class DynAnyFactoryImpl;
};


} //namespace dynAny
} //namespace TIDorb


#endif //__TIDORB_DYN_SEQUENCE_IMPL_H__
