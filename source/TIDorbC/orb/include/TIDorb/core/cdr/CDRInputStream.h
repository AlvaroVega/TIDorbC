/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/CDRInputStream.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/cdr.h"

#ifndef _TIDORB_core_cdr_CDRInputStream_H_
#define _TIDORB_core_cdr_CDRInputStream_H_ 

#include "CORBA.h"




namespace TIDorb {
namespace core {
     class AnyImpl;
}
}





namespace TIDorb {
namespace core {
namespace cdr {
  
class CDRInputStream: public TIDorb::portable::InputStream,
                      public IteratorCDR
                      // TODO: public virtual CORBA::DataInputStream
{

  public:
    CDRInputStream(){};
 
    static const CORBA::Long INDIRECTION_TAG = 0xffffffff;
  
    CDRInputStream(TIDorb::core::TIDORB* orb, BufferCDR* buffer);
    CDRInputStream(const CDRInputStream& input);
    CDRInputStream(const CDROutputStream& output);
    
    CDRInputStream* copy();
    
    CDRInputStream* deep_copy();

    ~CDRInputStream(){}
  
    void orb(TIDorb::core::TIDORB* ptr);
    TIDorb::core::TIDORB* tidorb();
    
    TIDorb::portable::ORB* orb();
    
    void read_octet(::CORBA::Octet& v); 

    void read_boolean(::CORBA::Boolean& v);

    void read_char(::CORBA::Char& v);
    void read_wchar(::CORBA::WChar& v);
    void read_short(::CORBA::Short& v);
    void read_long(::CORBA::Long& v);
    void read_longlong(::CORBA::LongLong& v);
    void read_ushort(::CORBA::UShort& v);
    void read_ulong(::CORBA::ULong& v);
    void read_ulonglong(::CORBA::ULongLong& v);
    void read_float(::CORBA::Float& v);
    void read_double(::CORBA::Double& v);
    void read_longdouble(::CORBA::LongDouble& v);    
    void read_fixed(::CORBA::Fixed& v,  CORBA::UShort digits, CORBA::Short scale);
    void read_string(char*& v);
    void read_string(char*& v, CORBA::ULong bound);
    // reads a string that can be inderected like repository ids
    void read_indirected_string(char*& v);
    void read_wstring(::CORBA::WChar*& v);
    void read_any(::CORBA::Any& v);
    void read_TypeCode(::CORBA::TypeCode_out v);
    void read_Object(::CORBA::Object_out v);
    void read_Principal(char*& v);
    void read_Context(::CORBA::Context_out v);
    void read_Value(::CORBA::ValueBase_out v);    
    ::CORBA::ValueBase_ptr read_Value();
    // Used to allocate dynamically values    
    ::CORBA::ValueBase_ptr read_Value(const CORBA::TypeCode_ptr type);
    void read_Abstract(::CORBA::AbstractBase_out v);
    ::CORBA::AbstractBase_ptr read_Abstract();
    
    void skip(CORBA::ULong length);

    void skip_octet() { skip_type(CDR::OCTET_SIZE); }    
    void skip_boolean() { skip_type(CDR::BOOLEAN_SIZE); }
    void skip_char() { skip_type(CDR::CHAR_SIZE); }
    void skip_wchar() { skip_type(CDR::WCHAR_SIZE); }
    void skip_short() { skip_type(CDR::SHORT_SIZE); }
    void skip_long() { skip_type(CDR::LONG_SIZE); }
    void skip_longlong() { skip_type(CDR::LONGLONG_SIZE); }
    void skip_ushort() { skip_type(CDR::USHORT_SIZE); }
    void skip_ulong() {skip_type(CDR::ULONG_SIZE);}
    void skip_ulonglong() { skip_type(CDR::ULONGLONG_SIZE); }
    void skip_float() {skip_type(CDR::FLOAT_SIZE); }
    void skip_double() {skip_type(CDR::DOUBLE_SIZE); }
    void skip_longdouble() {skip_type(CDR::LONG_DOUBLE_SIZE); }
    void skip_fixed(::CORBA::UShort digits, ::CORBA::Short scale);
    void skip_string();
    void skip_wstring();
    void skip_any();
    void skip_TypeCode();
    void skip_Object();
    void skip_Context();    
    void skip_Value(CORBA::TypeCode_ptr type);
    void skip_Abstract();
    void skip_Principal() {skip_string();}
   
    void read_octet_array(::CORBA::Octet v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_boolean_array(::CORBA::Boolean v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_char_array(::CORBA::Char v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_wchar_array(::CORBA::WChar v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_short_array(::CORBA::Short v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_long_array(::CORBA::Long v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_longlong_array(::CORBA::LongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_ushort_array(::CORBA::UShort v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_ulong_array(::CORBA::ULong v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_ulonglong_array(::CORBA::ULongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_float_array(::CORBA::Float v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_double_array(::CORBA::Double v[], ::CORBA::ULong offset, ::CORBA::ULong length);    
    void read_longdouble_array(::CORBA::LongDouble v[], ::CORBA::ULong offset, ::CORBA::ULong length);        
    void read_fixed_array(::CORBA::Fixed v[], ::CORBA::ULong offset, ::CORBA::ULong length, 
                          ::CORBA::UShort digits, ::CORBA::Short scale);
    void read_string_array(char* v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_wstring_array(::CORBA::WChar* v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void read_any_array(::CORBA::Any v[], ::CORBA::ULong offset, ::CORBA::ULong length);       

    void skip_octet_array(CORBA::ULong length) {skip(length);}
    void skip_boolean_array(CORBA::ULong length) {skip(length);}
    void skip_char_array(CORBA::ULong length) {skip(length);}
    void skip_wchar_array(CORBA::ULong length){skip_type_array(length, CDR::WCHAR_SIZE);}
    void skip_short_array(CORBA::ULong length){skip_type_array(length, CDR::SHORT_SIZE);}
    void skip_long_array(CORBA::ULong length){skip_type_array(length, CDR::LONG_SIZE);}
    void skip_longlong_array(CORBA::ULong length){skip_type_array(length, CDR::LONGLONG_SIZE);}
    void skip_ushort_array(CORBA::ULong length){skip_type_array(length, CDR::USHORT_SIZE);}
    void skip_ulong_array(CORBA::ULong length){skip_type_array(length, CDR::ULONG_SIZE);}
    void skip_ulonglong_array(CORBA::ULong length){skip_type_array(length, CDR::ULONGLONG_SIZE);}
    void skip_float_array(CORBA::ULong length){skip_type_array(length, CDR::FLOAT_SIZE);}
    void skip_double_array(CORBA::ULong length){skip_type_array(length, CDR::DOUBLE_SIZE);}
    void skip_longdouble_array(CORBA::ULong length){skip_type_array(length, CDR::LONG_DOUBLE_SIZE);}
    void skip_fixed_array(CORBA::ULong length, ::CORBA::UShort digits, ::CORBA::UShort scale);
    void skip_string_array(CORBA::ULong length);
    void skip_wstring_array(CORBA::ULong length);
    void skip_any_array(CORBA::ULong length);
    
    CDR::AbsolutePosition read_indirection();
    CDRInputStream* read_encapsulation();
    void skip_encapsulation();    

    TIDorb::core::AnyImpl * any_ref;
    bool point_ok;
    TIDorb::core::AnyImpl * get_any_ref(){ return any_ref;}
    void put_any_ref(TIDorb::core::AnyImpl * a_any_ref) {any_ref=a_any_ref;}
    
  protected:
  
    void skip_type(CORBA::ULong size);
    void skip_type_array(CORBA::ULong length, CORBA::ULong type_length);
    
    CORBA::TypeCode_ptr read_indirected_TypeCode();
  
    void get_next_chunk();
};
 
  inline void CDRInputStream::read_octet(::CORBA::Octet& v)
  {
    alignment(CDR::OCTET_SIZE);
    v = *(m_next++);
  }
  
  inline void CDRInputStream::read_boolean(::CORBA::Boolean& v)
  {
    read_octet((::CORBA::Octet&) v);
  }

  inline void CDRInputStream::read_long(::CORBA::Long& v)
  {
    alignment(CDR::LONG_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) &v;
      sp[3] = *m_next++;
      sp[2] = *m_next++;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      v = *((::CORBA::Long*) m_next);
      m_next += CDR::LONG_SIZE;
    }
  }
  
  inline void CDRInputStream::read_ulong(::CORBA::ULong& v)
  {
    //read_long((::CORBA::Long&) v);
    ::CORBA::Long& val= (::CORBA::Long&)v;
     alignment(CDR::LONG_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) &val;
      sp[3] = *m_next++;
      sp[2] = *m_next++;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      val = *((::CORBA::Long*) m_next);
      m_next += CDR::LONG_SIZE;
    }
   
  }

  inline void CDRInputStream::read_short(::CORBA::Short& v)
  {
    alignment(CDR::SHORT_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) &v;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      v = *((::CORBA::Short*) m_next);
      m_next += CDR::SHORT_SIZE;
    }
  }

  inline void CDRInputStream::read_ushort(::CORBA::UShort& v)
  {
    //read_short((::CORBA::Short&) v);
    ::CORBA::Short& val=(::CORBA::Short&) v;
    alignment(CDR::SHORT_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) &val;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      val = *((::CORBA::Short*) m_next);
      m_next += CDR::SHORT_SIZE;
    }

  }

  
  inline void CDRInputStream::read_float(::CORBA::Float& v)
  {
    CORBA::Long * val= (::CORBA::Long *)&v;
    //read_long(value);
    alignment(CDR::LONG_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) val;
      sp[3] = *m_next++;
      sp[2] = *m_next++;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      *val = *((::CORBA::Long*) m_next);
      m_next += CDR::LONG_SIZE;
    }

    //v = *((CORBA::Float*) &val);
  }
  
  
  inline void CDRInputStream::read_double(::CORBA::Double& v)
  {
    ::CORBA::LongLong& val= (::CORBA::LongLong&) v;
    //read_longlong((::CORBA::LongLong&) v);
    alignment(CDR::LONGLONG_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) &val;
      sp[7] = *m_next++;
      sp[6] = *m_next++;
      sp[5] = *m_next++;
      sp[4] = *m_next++;
      sp[3] = *m_next++;
      sp[2] = *m_next++;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      val = *((::CORBA::LongLong*) m_next);
      m_next += CDR::LONGLONG_SIZE;
    }
 
  }

  inline void CDRInputStream::read_longlong(::CORBA::LongLong& v)
  {
    alignment(CDR::LONGLONG_SIZE);
    if (m_byte_swap) {
      register unsigned char* sp = (unsigned char*) &v;
      sp[7] = *m_next++;
      sp[6] = *m_next++;
      sp[5] = *m_next++;
      sp[4] = *m_next++;
      sp[3] = *m_next++;
      sp[2] = *m_next++;
      sp[1] = *m_next++;
      sp[0] = *m_next++;
    } else {
      v = *((::CORBA::LongLong*) m_next);
      m_next += CDR::LONGLONG_SIZE;
    }
  }


  inline void CDRInputStream::read_string(char*& v)
    {
      CORBA::ULong length;
      read_ulong(length);
      
      if(length == 0)
  	throw CORBA::MARSHAL("Invalid string length");
      
      v = CORBA::string_alloc(length-1);
      //read_char_array(v, 0, length);
      read_octet_array((::CORBA::Octet*)v, 0, length);
    }


  inline void CDRInputStream::read_string(char*& v, CORBA::ULong bound)
    {
      CORBA::ULong length;
      read_ulong(length);
      
      if (length == 0)
        throw CORBA::MARSHAL("Invalid string length");
      
      if (length > bound + 1) 
        throw CORBA::BAD_PARAM("String length exceeds bound");
      
      v = CORBA::string_alloc(length);
      //read_char_array(v, 0, length);
      read_octet_array((::CORBA::Octet*) v, 0, length);
    }
  


 
} // namespace cdr
} // namespace core
} // namespace TIDorb

#endif
