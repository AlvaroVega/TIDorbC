/*/////////////////////////////////////////////////////////////////////////
//
// File:        Any.h
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

#ifndef _TIDORB_portable_Any_H_
#define _TIDORB_portable_Any_H_

#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDorb {
namespace portable {

class Any {

  public:

    virtual ~Any() {}
    
    virtual Any& operator=(const Any& value) = 0;

    virtual bool operator==(const Any& value) const = 0;
    virtual bool operator!=(const Any& value) const = 0;
       
    virtual void insert_boolean(CORBA::Boolean value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_char(CORBA::Char value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_wchar(CORBA::WChar value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_octet(CORBA::Octet value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_string(char* value) = 0; //non-copying
    virtual void insert_string(const char* value) = 0;
    virtual void insert_wstring(CORBA::WChar* value) = 0; //non-copying
    virtual void insert_wstring(const CORBA::WChar* value) = 0;
    virtual void insert_short (CORBA::Short value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_ushort (CORBA::UShort value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_long (CORBA::Long value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_ulong (CORBA::ULong value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_float (CORBA::Float value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_double (CORBA::Double value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_longlong (CORBA::LongLong value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_ulonglong (CORBA::ULongLong value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_longdouble (CORBA::LongDouble value, CORBA::TypeCode_ptr type=0) = 0;
    virtual void insert_any (const CORBA::Any& value) = 0; // copying
    virtual void insert_any (CORBA::Any* value) = 0; // non-copying
    virtual void insert_fixed(const CORBA::Fixed& value, CORBA::TypeCode_ptr type) = 0;  
    virtual void insert_fixed(const CORBA::Fixed& value, CORBA::UShort digits, CORBA::Short scale) = 0;   
    virtual void insert_TypeCode(CORBA::TypeCode_ptr value) = 0;
    virtual void insert_Streamable(Streamable* value) = 0; // non-copy take ownership
    virtual void insert_Streamable(const Streamable* value) = 0; // non-copy take reference
    virtual void insert_Object(CORBA::Object_ptr value) = 0;
    virtual void insert_Object(CORBA::Object_ptr value, const CORBA::TypeCode_ptr type) = 0;
    virtual void insert_Value(CORBA::ValueBase* value) = 0;
    virtual void insert_Value(CORBA::ValueBase* value, CORBA::TypeCode_ptr type) = 0;
     

    virtual CORBA::Boolean extract_boolean(CORBA::Boolean& value) const = 0;
    virtual CORBA::Boolean extract_char(CORBA::Char& value) const = 0;
    virtual CORBA::Boolean extract_wchar(CORBA::WChar& value) const = 0;
    virtual CORBA::Boolean extract_octet(CORBA::Octet& value) const = 0;
    virtual CORBA::Boolean extract_string(const CORBA::Char*& value) const = 0;
    virtual CORBA::Boolean extract_wstring(const CORBA::WChar*& value) const = 0;
    virtual CORBA::Boolean extract_short (CORBA::Short& value) const = 0;
    virtual CORBA::Boolean extract_ushort (CORBA::UShort& value) const = 0;
    virtual CORBA::Boolean extract_long (CORBA::Long& value) const = 0;
    virtual CORBA::Boolean extract_ulong (CORBA::ULong& value) const = 0;
    virtual CORBA::Boolean extract_float (CORBA::Float& value) const = 0;
    virtual CORBA::Boolean extract_double (CORBA::Double& value) const = 0;
    virtual CORBA::Boolean extract_longlong (CORBA::LongLong& value) const = 0;
    virtual CORBA::Boolean extract_ulonglong (CORBA::ULongLong& value) const = 0;
    virtual CORBA::Boolean extract_longdouble (CORBA::LongDouble& value) const = 0;
    virtual CORBA::Boolean extract_fixed(CORBA::Fixed& value, CORBA::TypeCode_ptr type) const = 0;
    virtual CORBA::Boolean extract_any (const CORBA::Any*& value) const = 0;
    virtual CORBA::Boolean extract_any (CORBA::Any*& value) = 0 ;
    virtual CORBA::Boolean extract_Object(CORBA::Object_out value) const = 0;
    virtual CORBA::Boolean extract_fixed(CORBA::Fixed& value, 
                                         CORBA::UShort digits, CORBA::Short scale) const = 0;   
    virtual CORBA::Boolean extract_TypeCode(CORBA::TypeCode_out value) const = 0;
    virtual CORBA::Boolean extract_Streamable(const Streamable*& value) const = 0;
    virtual CORBA::Boolean set_Streamable(Streamable* value) const = 0;
    //consumes the value, user must not delete value
    // return false if there is another Streamable setted

    virtual CORBA::Boolean extract_Value(CORBA::ValueBase*& value) const = 0;
 
  
    virtual void read_value(InputStream& stream, CORBA::TypeCode_ptr type,bool
skip_mode=false) = 0;
    virtual void write_value(OutputStream& stream) const = 0;
    
    virtual CORBA::TypeCode_ptr type() const = 0;
    virtual void type(CORBA::TypeCode_ptr type_code) = 0;
    virtual void set_equivalent_type(CORBA::TypeCode_ptr type_code) = 0;
    virtual void set_type(CORBA::TypeCode_ptr type_code) = 0;
    
    virtual InputStream*  create_input_stream() const = 0;
    virtual OutputStream* create_output_stream() = 0;
    
    virtual bool dump(ostream& os) const = 0;

  protected:
    Any(){}
 
};
  
}
}


#endif
