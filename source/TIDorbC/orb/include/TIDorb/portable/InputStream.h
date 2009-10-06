/*/////////////////////////////////////////////////////////////////////////
//
// File:        InputStream.h
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

#ifndef _TIDORB_portable_InputStream_H_
#define _TIDORB_portable_InputStream_H_ 

namespace TIDorb {
namespace portable {

class InputStream {

  public:
  
    virtual ~InputStream(){}
  
    virtual TIDorb::portable::ORB* orb() = 0;
    
    virtual void read_octet(::CORBA::Octet& v) = 0;
    virtual void read_boolean(::CORBA::Boolean& v) = 0;
    virtual void read_char(::CORBA::Char& v) = 0;
    virtual void read_wchar(::CORBA::WChar& v) = 0;
    virtual void read_short(::CORBA::Short& v) = 0;
    virtual void read_long(::CORBA::Long& v) = 0;
    virtual void read_longlong(::CORBA::LongLong& v) = 0;
    virtual void read_ushort(::CORBA::UShort& v) = 0;
    virtual void read_ulong(::CORBA::ULong& v) = 0;
    virtual void read_ulonglong(::CORBA::ULongLong& v) = 0;  
    virtual void read_float(::CORBA::Float& v) = 0;
    virtual void read_double(::CORBA::Double& v) = 0;
    virtual void read_longdouble(::CORBA::LongDouble& v) = 0;    
    virtual void read_fixed(::CORBA::Fixed& v, CORBA::UShort digits, CORBA::Short scale) = 0;
    virtual void read_string(char*& v) = 0;
    virtual void read_string(char*& v, CORBA::ULong bound) = 0;
    virtual void read_wstring(::CORBA::WChar*& v) = 0;
    virtual void read_any(::CORBA::Any& v) = 0;
    virtual void read_TypeCode(::CORBA::TypeCode_out v) = 0;
    virtual void read_Object(::CORBA::Object_out v) = 0;
    virtual void read_Value(::CORBA::ValueBase_out v) = 0;
    virtual void read_Abstract(::CORBA::AbstractBase_out v) = 0;
    virtual void read_Context(::CORBA::Context_out v) = 0;
    virtual void read_Principal(char*& v) = 0;
    
    virtual void read_octet_array(::CORBA::Octet v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_boolean_array(::CORBA::Boolean v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_char_array(::CORBA::Char v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_wchar_array(::CORBA::WChar v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_short_array(::CORBA::Short v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_long_array(::CORBA::Long v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_longlong_array(::CORBA::LongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_ushort_array(::CORBA::UShort v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_ulong_array(::CORBA::ULong v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_ulonglong_array(::CORBA::ULongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;  
    virtual void read_float_array(::CORBA::Float v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_double_array(::CORBA::Double v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_longdouble_array(::CORBA::LongDouble v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;    
    virtual void read_fixed_array(::CORBA::Fixed v[], ::CORBA::ULong offset, ::CORBA::ULong length, 
                                                      ::CORBA::UShort digits, ::CORBA::Short scale) = 0;
    virtual void read_string_array(::CORBA::Char* v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_wstring_array(::CORBA::WChar* v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;
    virtual void read_any_array(::CORBA::Any v[], ::CORBA::ULong offset, ::CORBA::ULong length) = 0;           

// ValueBase marshalling: looks up a ValueFactory and call create_for_unmarshal
    static CORBA::ValueBase* create_value(const CORBA::ORB_ptr orb, 
                                          const char* rep_id);

};
  
}
}

#endif
