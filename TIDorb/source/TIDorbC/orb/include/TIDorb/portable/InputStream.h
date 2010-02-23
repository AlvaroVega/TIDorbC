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
