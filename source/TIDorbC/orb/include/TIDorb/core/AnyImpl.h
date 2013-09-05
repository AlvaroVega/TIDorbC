//////////////////////////////////////////////////////////////////////////////////
//
// File:        AnyImpl.h
// Description: CORBA::Any implementation.
//
// Rel:         01.00
// Created:     September, 2002
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_core_Any_H_
#define _TIDORB_core_Any_H_

#include "TIDorb/core/cdr.h"
#include "TIDorb/core/typecode.h"

#include "TIDThr.h"

#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif



namespace TIDorb {
namespace core {

class AnyImpl : public TIDorb::portable::Any
{

  public:


    AnyImpl();
    ~AnyImpl();
    
    TIDORB* orb() const;
    void orb(TIDORB* ptr);
    
    //previously declared as AnyImpl& operator=(const TIDorb::portable::Any& value);
    // changed due to gcc 2.95 does not admits covariant return types
    
    TIDorb::portable::Any& operator=(const TIDorb::portable::Any& value);
    bool operator==(const TIDorb::portable::Any& a) const;
    bool operator!=(const TIDorb::portable::Any& a) const;
       
    void insert_boolean(CORBA::Boolean value, CORBA::TypeCode_ptr type=0);
    void insert_char(CORBA::Char value, CORBA::TypeCode_ptr type=0);
    void insert_wchar(CORBA::WChar value, CORBA::TypeCode_ptr type=0);
    void insert_octet(CORBA::Octet value, CORBA::TypeCode_ptr type=0);
    void insert_string(char* value); //non-copying
    void insert_string(const char* value);
    void insert_wstring(CORBA::WChar* value); //non-copying
    void insert_wstring(const CORBA::WChar* value);
    void insert_short (CORBA::Short value, CORBA::TypeCode_ptr type=0);
    void insert_ushort (CORBA::UShort value, CORBA::TypeCode_ptr type=0);
    void insert_long (CORBA::Long value, CORBA::TypeCode_ptr type=0);
    void insert_ulong (CORBA::ULong value, CORBA::TypeCode_ptr type=0);
    void insert_float (CORBA::Float value, CORBA::TypeCode_ptr type=0);
    void insert_double (CORBA::Double value, CORBA::TypeCode_ptr type=0);
    void insert_longlong (CORBA::LongLong value, CORBA::TypeCode_ptr type=0);
    void insert_ulonglong (CORBA::ULongLong value, CORBA::TypeCode_ptr type=0);
    void insert_longdouble (CORBA::LongDouble value, CORBA::TypeCode_ptr type=0);
    void insert_any (const CORBA::Any& value); // copying
    void insert_any (CORBA::Any* value); // non-copying
    void insert_fixed(const CORBA::Fixed& value, CORBA::TypeCode_ptr type);  
    void insert_fixed(const CORBA::Fixed& value, CORBA::UShort digits, CORBA::Short scale);  
    void insert_TypeCode(CORBA::TypeCode_ptr value);
    void insert_Streamable(TIDorb::portable::Streamable* value); // non-copy take ownership
    void insert_Streamable(const TIDorb::portable::Streamable* value); // non-copy take reference    
    void insert_Object(CORBA::Object_ptr value);
    void insert_Object(CORBA::Object_ptr value, const CORBA::TypeCode_ptr type);
    void insert_Value(CORBA::ValueBase* value);
    void insert_Value(CORBA::ValueBase* value, CORBA::TypeCode_ptr type);
    
    CORBA::Boolean extract_boolean(CORBA::Boolean& value) const;
    CORBA::Boolean extract_char(CORBA::Char& value) const;
    CORBA::Boolean extract_wchar(CORBA::WChar& value) const;
    CORBA::Boolean extract_octet(CORBA::Octet& value) const;
    CORBA::Boolean extract_string(const CORBA::Char*& value) const;
    CORBA::Boolean extract_wstring(const CORBA::WChar*& value) const;
    CORBA::Boolean extract_short (CORBA::Short& value) const;
    CORBA::Boolean extract_ushort (CORBA::UShort& value) const;
    CORBA::Boolean extract_long (CORBA::Long& value) const;
    CORBA::Boolean extract_ulong (CORBA::ULong& value) const;
    CORBA::Boolean extract_float (CORBA::Float& value) const;
    CORBA::Boolean extract_double (CORBA::Double& value) const;
    CORBA::Boolean extract_longlong (CORBA::LongLong& value) const;
    CORBA::Boolean extract_ulonglong (CORBA::ULongLong& value) const;
    CORBA::Boolean extract_longdouble (CORBA::LongDouble& value) const;
    CORBA::Boolean extract_any (const CORBA::Any*& value) const;
    CORBA::Boolean extract_any (CORBA::Any*& value) ;
    CORBA::Boolean extract_Object(CORBA::Object_out value) const;
    CORBA::Boolean extract_fixed(CORBA::Fixed& value, CORBA::TypeCode_ptr type) const; 
    CORBA::Boolean extract_fixed(CORBA::Fixed& value, CORBA::UShort digits, CORBA::Short scale) const;    
    CORBA::Boolean extract_TypeCode(CORBA::TypeCode_out value) const;
    CORBA::Boolean extract_Streamable(const TIDorb::portable::Streamable*& value) const;
    CORBA::Boolean set_Streamable(TIDorb::portable::Streamable* value) const;
    CORBA::Boolean extract_Value(CORBA::ValueBase*& value) const;
 
    // void read_value(TIDorb::portable::InputStream& stream, CORBA::TypeCode_ptr tc);
    void read_value(TIDorb::portable::InputStream& stream, CORBA::TypeCode_ptr tc,bool skip_mode=false);
    void write_value(TIDorb::portable::OutputStream& os) const;
    
    CORBA::TypeCode_ptr type() const;
    void type(CORBA::TypeCode_ptr type_code);
    void set_equivalent_type(CORBA::TypeCode_ptr type_code);
    void set_type(CORBA::TypeCode_ptr type_code);

    TIDorb::portable::InputStream*  create_input_stream() const;
    TIDorb::portable::OutputStream* create_output_stream();
 
    bool dump(ostream& os) const;
 
    bool has_Streamable() const;
    void assign(const AnyImpl& to, bool wrap = false);
    void assign_value(const AnyImpl& a, bool wrap = false);

    enum Estado {OK,DEPENDIENTE,PRIMERO};
    void skip_value(TIDorb::core::cdr::CDRInputStream* &input);
    Estado mi_cdr;
    AnyImpl * my_any_ref;
    TIDorb::core::typecode::TypeCodeImpl * my_typecode;
   
  protected:
  
    void reset_value();
    void create_marshaled_value() const;
    void init_value(TIDorb::portable::Streamable* holder);
    void wrap_value(const AnyImpl* a);

    static CORBA::TypeCode_ptr get_effective_type(const CORBA::TypeCode_ptr tc);
    
  private:
  
    //TIDorb::core::TIDORB_ref m_orb;
    TIDorb::core::TIDORB* m_orb;

    TIDorb::core::typecode::TypeCodeImpl* m_type;	

// content_type in case of basic type aliases
    CORBA::TypeCode_ptr m_effective_type;
// end

    TIDorb::core::cdr::CDRInputStream* m_marshaled_value;

    TIDorb::portable::Streamable* m_value;

    const AnyImpl* m_wrapped_any;
};

}
}

#endif
