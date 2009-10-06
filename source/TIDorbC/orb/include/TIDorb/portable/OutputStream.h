/*/////////////////////////////////////////////////////////////////////////
//
// File:        OutputStream.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/portable.h"

#ifndef _TIDORB_portable_OutputStream_H_
#define _TIDORB_portable_OutputStream_H_

namespace TIDorb {
namespace portable {

class OutputStream {

  public:
   
    virtual ~OutputStream(){}
    
    virtual InputStream* create_input_stream() = 0;

    virtual TIDorb::portable::ORB* orb() = 0;
     
    virtual void write_octet(::CORBA::Octet v) = 0;
    virtual void write_boolean(::CORBA::Boolean v) = 0;
    virtual void write_char(::CORBA::Char v) = 0;
    virtual void write_wchar(::CORBA::WChar v) = 0;
    virtual void write_short(::CORBA::Short v) = 0;
    virtual void write_long(::CORBA::Long v) = 0;
    virtual void write_longlong(::CORBA::LongLong v) = 0;
    virtual void write_ushort(::CORBA::UShort v) = 0;
    virtual void write_ulong(::CORBA::ULong v) = 0;
    virtual void write_ulonglong(::CORBA::ULongLong v) = 0;  
    virtual void write_float(::CORBA::Float v) = 0;
    virtual void write_double(::CORBA::Double v) = 0;
    virtual void write_longdouble(::CORBA::LongDouble v) = 0;
    virtual void write_string(const char* v) = 0;
    virtual void write_wstring(const ::CORBA::WChar* v) = 0;
    virtual void write_fixed(const ::CORBA::Fixed& v, CORBA::UShort digits, CORBA::Short scale) = 0;    
    virtual void write_any(const ::CORBA::Any& v) = 0;
    virtual void write_TypeCode(const ::CORBA::TypeCode_ptr v) = 0;
    virtual void write_Object(const ::CORBA::Object_ptr v) = 0;
    virtual void write_Value(const ::CORBA::ValueBase_ptr v) = 0;
    virtual void write_Abstract(const ::CORBA::AbstractBase_ptr v) = 0;
    virtual void write_Context(const CORBA::Context_ptr ctx, 
                               const CORBA::ContextList_ptr contexts) = 0;
    virtual void write_Principal(const char* v) = 0;
    
    virtual void write_octet_array(const ::CORBA::Octet v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_boolean_array(const ::CORBA::Boolean  v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_char_array(const ::CORBA::Char v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_wchar_array(const ::CORBA::WChar v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_short_array(const ::CORBA::Short v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_long_array(const ::CORBA::Long v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_longlong_array(const ::CORBA::LongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_ushort_array(const ::CORBA::UShort v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_ulong_array(const ::CORBA::ULong v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_ulonglong_array(const ::CORBA::ULongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;  
    virtual void write_float_array(const ::CORBA::Float v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_double_array(const ::CORBA::Double v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_longdouble_array(const ::CORBA::LongDouble v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_string_array(const char* v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_wstring_array(const ::CORBA::WChar* v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void write_fixed_array(const ::CORBA::Fixed v[], ::CORBA::ULong offset, ::CORBA::ULong length, 
                                   CORBA::UShort digits, CORBA::Short scale) = 0;    
    virtual void write_any_array(const ::CORBA::Any v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    
  protected:
    OutputStream(){}
            
};
  
}
}

#endif
