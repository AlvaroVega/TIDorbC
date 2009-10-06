/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/CDROutputStream.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/cdr.h"

#ifndef _TIDORB_CORE_CDR_CDROUTPUTSTREAM_H_
#define _TIDORB_CORE_CDR_CDROUTPUTSTREAM_H_

#include "CORBA.h"



namespace TIDorb {
namespace core {
namespace cdr {

class CDROutputStream : public TIDorb::portable::OutputStream,
                        public TIDorb::core::cdr::IteratorCDR
                        // TODO: public virtual CORBA::DataOutputStream
 {
    
  public:

    CDROutputStream(){};
 

    CDROutputStream(TIDorb::core::TIDORB* orb): IteratorCDR(orb){}
    CDROutputStream(TIDorb::core::TIDORB* orb, BufferCDR* buffer)
    : IteratorCDR(orb, buffer) {}
    
    ~CDROutputStream() {}

    void orb(TIDorb::core::TIDORB* ptr);
    TIDorb::core::TIDORB* tidorb();
    TIDorb::portable::ORB* orb();
    
    TIDorb::portable::InputStream* create_input_stream();    
    CDRInputStream* input_stream_at_this_position();
     
    void write_octet(::CORBA::Octet v);
      
    void write_boolean(::CORBA::Boolean v);

    void write_char(::CORBA::Char v);
    void write_wchar(::CORBA::WChar v);      
    void write_short(::CORBA::Short v);
    void write_long(::CORBA::Long v);
    void write_longlong(::CORBA::LongLong v);
    void write_ushort(::CORBA::UShort v);
    void write_ulong(::CORBA::ULong v);
    void write_ulonglong(::CORBA::ULongLong v); 
    void write_float(::CORBA::Float v);
    void write_double(::CORBA::Double v);
    void write_longdouble(::CORBA::LongDouble v);
    void write_string(const char* v);
    void write_wstring(const ::CORBA::WChar* v);    
    void write_fixed(const ::CORBA::Fixed& v, CORBA::UShort digits, CORBA::Short scale);
    void write_any(const ::CORBA::Any& v);
    void write_TypeCode(const ::CORBA::TypeCode_ptr v);
    void write_Object(const ::CORBA::Object_ptr v);
    void write_Context(const CORBA::Context_ptr ctx, 
                       const CORBA::ContextList_ptr contexts);
    void write_Principal(const char* p);
    void write_Value(const CORBA::ValueBase_ptr value);
    void write_Abstract(const CORBA::AbstractBase_ptr abstract);
    
    void write_referenceable_string(const char* p);  
   
    void write_octet_array(const ::CORBA::Octet v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_boolean_array(const ::CORBA::Boolean v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_char_array(const ::CORBA::Char v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_wchar_array(const ::CORBA::WChar v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_short_array(const ::CORBA::Short v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_long_array(const ::CORBA::Long v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_longlong_array(const ::CORBA::LongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_ushort_array(const ::CORBA::UShort v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_ulong_array(const ::CORBA::ULong v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_ulonglong_array(const ::CORBA::ULongLong v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_float_array(const ::CORBA::Float v[], ::CORBA::ULong offset, ::CORBA::ULong length);    
    void write_double_array(const ::CORBA::Double v[], ::CORBA::ULong offset, ::CORBA::ULong length);    
    void write_longdouble_array(const ::CORBA::LongDouble v[], ::CORBA::ULong offset, ::CORBA::ULong length);        
    void write_string_array(const ::CORBA::Char* v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_wstring_array(const ::CORBA::WChar* v[], ::CORBA::ULong offset, ::CORBA::ULong length);
    void write_fixed_array(const ::CORBA::Fixed v[], ::CORBA::ULong offset, ::CORBA::ULong length,  
                           CORBA::UShort digits, CORBA::Short scale);    
    void write_any_array(const ::CORBA::Any v[], ::CORBA::ULong offset, ::CORBA::ULong length);
   
   /* Encapsulation marshalling */

    /*
      create_encapsulation: prepares an encapsulation to be marshaled in this stream
    */
    CDROutputStream* create_encapsulation();   
    void write_indirection(CDR::AbsolutePosition previous_position);
    void write_buffer(BufferCDR& buffer);    
   
    /** 
    * Set the stream pointer to the next aligned position of given data type and
    * sets it as the last available position
    * @param type_size the type size in octets
    */
    void fix_next_aligned_position(CORBA::ULong type_size);
    
  protected:
  
    bool writeValueTags(const CORBA::ValueBase_ptr value);
    
    void write_pair(unsigned int digit1, unsigned int digit2);
    
    void write_data_array(const ::CORBA::Octet v[], ::CORBA::ULong offset, 
                          ::CORBA::ULong length, ::CORBA::ULong type_size);
                          
    void get_next_chunk();
    

};

  inline void CDROutputStream::write_octet(::CORBA::Octet v)
  {
    alignment(CDR::OCTET_SIZE);
    *(m_next++) = v;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
  }    	

  inline void CDROutputStream::write_boolean(::CORBA::Boolean v)
  {
    write_octet((CORBA::Octet) v);
  }

  inline void CDROutputStream::write_long(::CORBA::Long v)
  {
    alignment(CDR::LONG_SIZE);
    *((::CORBA::Long*) m_next) = v;
    m_next+= CDR::LONG_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
  }

  inline void CDROutputStream::write_short(::CORBA::Short v)
  {
    alignment(CDR::SHORT_SIZE);
    *((::CORBA::Short*) m_next) = v;
    m_next+= CDR::SHORT_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
  }

  inline void CDROutputStream::write_wchar(::CORBA::WChar v)
  {
    // 2 BYTES UTF ASSUMED
    //CORBA::UShort val = (CORBA::UShort) (v & 0xffff);
    alignment(CDR::SHORT_SIZE);
    *((::CORBA::UShort*) m_next) = v;
    m_next+= CDR::SHORT_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
    
    //write_ushort(val);
  }

  inline void CDROutputStream::write_longlong(::CORBA::LongLong v)
  {
    alignment(CDR::LONGLONG_SIZE);
    *((::CORBA::LongLong*) m_next) = v;
    m_next+= CDR::LONGLONG_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
  }
  
  inline void CDROutputStream::write_longdouble(::CORBA::LongDouble v)
  {
    alignment(CDR::LONGLONG_SIZE); //long double is aligned to 8
  
    *((::CORBA::LongDouble*) m_next) = v;
    m_next+= CDR::LONG_DOUBLE_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
  }

 
  inline void CDROutputStream::write_char(::CORBA::Char v)
  {
    alignment(CDR::OCTET_SIZE);
    *(m_next++) = v;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);

    //write_octet((CORBA::Octet) v);
  }
  
  inline void CDROutputStream::write_ushort(::CORBA::UShort v)
  {
    alignment(CDR::SHORT_SIZE);
    *((::CORBA::UShort*) m_next) = v;
    m_next+= CDR::SHORT_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);

    //write_short((CORBA::Short) v);
  }
 
  inline void CDROutputStream::write_ulong(::CORBA::ULong v)
  {
    alignment(CDR::LONG_SIZE);
    *((::CORBA::ULong*) m_next) = v;
    m_next+= CDR::LONG_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);
 
    //write_long((::CORBA::Long) v);
  }

  inline void CDROutputStream::write_ulonglong(::CORBA::ULongLong v)
  {
    alignment(CDR::LONGLONG_SIZE);
    *((::CORBA::ULongLong*) m_next) = v;
    m_next+= CDR::LONGLONG_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);

    //write_longlong((::CORBA::LongLong) v);
  }




  inline void CDROutputStream::write_float(::CORBA::Float v)
  {
    alignment(CDR::LONG_SIZE);
    *((::CORBA::Float*) m_next) = v;
    m_next+= CDR::LONG_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);

    //write_long(*((::CORBA::Long*) &v));
  }




  inline void CDROutputStream::write_double(::CORBA::Double v)
  {
    alignment(CDR::LONGLONG_SIZE);
    *((::CORBA::Double*) m_next) = v;
    m_next+= CDR::LONGLONG_SIZE;
    if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
      m_chunk->set_available_end(m_next);

    //write_longlong(*((::CORBA::LongLong*) &v));
  }
  
      


  inline void CDROutputStream::write_string(const char* v)
  {
    if (v == 0)
      throw CORBA::BAD_PARAM();
  
    size_t length = strlen(v);
    write_ulong((CORBA::ULong) length + 1);
    write_octet_array((CORBA::Octet*) v, 0, length);
    write_char('\0');
  }

  inline void CDROutputStream::write_octet_array(const ::CORBA::Octet v[],
                                                 ::CORBA::ULong offset,
                                                 ::CORBA::ULong length)
    {
      if (v == 0)
        throw CORBA::BAD_PARAM();

      ::CORBA::ULong octets_can_write = 0;

      // numero de octetos que quedan por escribir
      ::CORBA::ULong remain_octets = length;

      //MLG
      while (true)
        {
          octets_can_write = (::CORBA::ULong)
            ((uintptr_t) m_chunk->get_buffer_end() - (uintptr_t) m_next);

          if(octets_can_write > remain_octets)
            octets_can_write = remain_octets;

          memcpy((void *) m_next, (const void *) v, octets_can_write);
          m_next+= octets_can_write;
          v+= octets_can_write;
          remain_octets -= octets_can_write;
          //EMLG
          if(remain_octets > 0)
            {
              get_next_chunk();
            }
          else
            {
              if((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
                m_chunk->set_available_end(m_next);
              break;
            }
        }
    }
 
} // namespace cdr
} // namespace core
} // namespace TIDorb

#endif  

