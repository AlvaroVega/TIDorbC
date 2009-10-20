/////////////////////////////////////////////////////////////////////////
//
// File:        CDROutputStream.C
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

#include "TIDorb/core/cdr.h"
#include "TIDorb/core/typecode.h"
#include "TIDorb/core/util.h"
#include "TIDorb/util.h"

#include <string.h>





namespace TIDorb {
namespace core {
namespace cdr {




void CDROutputStream::orb(TIDorb::core::TIDORB* ptr)
{
  m_orb = ptr;
}


TIDorb::core::TIDORB* CDROutputStream::tidorb()
{
  return m_orb;
}

TIDorb::portable::ORB* CDROutputStream::orb()
{
  return m_orb;
}




TIDorb::portable::InputStream* CDROutputStream::create_input_stream()
{
  return new CDRInputStream(m_orb, m_bufferCDR);
}




CDRInputStream* CDROutputStream::input_stream_at_this_position()
{
  CDRInputStream* input = new CDRInputStream(*this);
  input->fix_starting();
  return input;
}
/*
void CDROutputStream::write_wchar(::CORBA::WChar v)
{
  // 2 BYTES UTF ASSUMED
  CORBA::UShort val = (CORBA::UShort) (v & 0xffff);
  write_ushort(val);
}
*/

/*

void CDROutputStream::write_short(::CORBA::Short v)
{
  alignment(CDR::SHORT_SIZE);
  *((::CORBA::Short*) m_next) = v;
  m_next+= CDR::SHORT_SIZE;
  if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
    m_chunk->set_available_end(m_next);
}
*/
/*
void CDROutputStream::write_longlong(::CORBA::LongLong v)
{
  alignment(CDR::LONGLONG_SIZE);
  *((::CORBA::LongLong*) m_next) = v;
  m_next+= CDR::LONGLONG_SIZE;
  if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
    m_chunk->set_available_end(m_next);
}
*/


/*
void CDROutputStream::write_longdouble(::CORBA::LongDouble v)
{
  alignment(CDR::LONGLONG_SIZE); //long double is aligned to 8

  *((::CORBA::LongDouble*) m_next) = v;
  m_next+= CDR::LONG_DOUBLE_SIZE;
  if ((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
    m_chunk->set_available_end(m_next);
}
*/
/*
void CDROutputStream::write_char(::CORBA::Char v)
{
  write_octet((CORBA::Octet) v);
}
*/

/*

void CDROutputStream::write_ushort(::CORBA::UShort v)
{
  write_short((CORBA::Short) v);
}

*/

/*
void CDROutputStream::write_ulong(::CORBA::ULong v)
{
  write_long((::CORBA::Long) v);
}
*/


/*
void CDROutputStream::write_ulonglong(::CORBA::ULongLong v)
{
  write_longlong((::CORBA::LongLong) v);
}




void CDROutputStream::write_float(::CORBA::Float v)
{
  write_long(*((::CORBA::Long*) &v));
}




void CDROutputStream::write_double(::CORBA::Double v)
{
  write_longlong(*((::CORBA::LongLong*) &v));
}




void CDROutputStream::write_string(const char* v)
{
  if (v == 0)
    throw CORBA::BAD_PARAM();

  size_t length = strlen(v);
  write_ulong((CORBA::ULong) length + 1);
  write_octet_array((CORBA::Octet*) v, 0, length);
  write_char('\0');
}


*/

void CDROutputStream::write_wstring(const CORBA::WChar* v)
{
  if (v == 0)
    throw CORBA::BAD_PARAM();

  size_t length = wcslen(v);

  if (m_version == TIDorb::core::comm::iiop::Version::VERSION_1_2)
    write_ulong((length + 1) * CDR::WCHAR_SIZE);
  else
    write_ulong(length + 1);

  // write value data (including the end wchar)
  write_wchar_array(v, 0, length + 1);
}




void CDROutputStream::write_fixed(const ::CORBA::Fixed& v,
                                  CORBA::UShort digits,
                                  CORBA::Short scale)
{
  int value_length = TIDorb::core::typecode::FixedTypeCode::value_length(digits);

  TIDorb::types::BigInt unscaled = v.fixed_value();

  CORBA::String_var literal = unscaled.toString();

  int literal_count = (v < CORBA::Fixed(0)) ? 1:0; //skip the signum char

  int literal_digits = strlen(literal) - literal_count;

  if (digits < literal_digits)
    throw CORBA::MARSHAL("Fixed Overflow", 0, CORBA::COMPLETED_NO);

  int value_pairs = (literal_digits - 1) / 2; // number of digit_digit octets
  // 1 is substracted from literals represents
  // the last digit int digit_signum pair

  int zero_value_pairs = (literal_digits - 1) % 2; // 0_digit octet is 0 or 1
  int zero_pairs = (value_length / 2) - zero_value_pairs - value_pairs - 1;
  // 0_0 octets
  // 1 is substracted from zero_pairs represents the zero_signum pair

  for (int i = 0; i < zero_pairs; i++)
    write_pair(0, 0);

  if (zero_value_pairs != 0)
    write_pair(0, TIDorb::util::Base16Codec::getValue(((char*)literal)[literal_count++]));

  for (int j = 0; j < zero_pairs; j++)
    write_pair(TIDorb::util::Base16Codec::getValue(((char*)literal)[literal_count++]),
               TIDorb::util::Base16Codec::getValue(((char*)literal)[literal_count++]));

  write_pair(TIDorb::util::Base16Codec::getValue(((char*)literal)[literal_count++]),
             (v < CORBA::Fixed(0)) ? 0xD:0xC);
}




void CDROutputStream::write_pair(unsigned int digit1, unsigned int digit2)
{
  write_octet((CORBA::Octet) ((digit1<<4)|(digit2)));
}




void CDROutputStream::write_TypeCode(const ::CORBA::TypeCode_ptr v)
{
  //jagd
  //if (CORBA::is_nil(v))
  if (!(v))
    throw CORBA::BAD_PARAM();

  //jagd
  //TIDorb::core::typecode::TypeCodeImpl* tc =     
  //  (TIDorb::core::typecode::TypeCodeImpl*)v->_impl();
  TIDorb::core::typecode::TypeCodeImpl* tc =     
    (TIDorb::core::typecode::TypeCodeImpl*)v;
  if(m_orb)
    tc->set_exhaustive_equal(m_orb->conf().exhaustive_equal);
  tc->marshal(*this);
}




void CDROutputStream::write_Object(const ::CORBA::Object_ptr v)
{
  //jagd
  //if (CORBA::is_nil(v)){
  if (!(v)){
    TIDorb::core::iop::IOR* null_ior = ::TIDorb::core::iop::IOR::null_ior();
    null_ior->write(*this);
    return;
  }

  CORBA::ValueBase* _value_base = dynamic_cast<CORBA::ValueBase*> (v);
  if (_value_base != NULL)
    throw CORBA::NO_IMPLEMENT();

  CORBA::LocalObject* _local_object = dynamic_cast<CORBA::LocalObject*> (v);
  if (_local_object != NULL)
    throw CORBA::MARSHAL("Impossible to marshal a local object", 4, CORBA::COMPLETED_NO);

  TIDorb::portable::Stub* _stub = dynamic_cast<TIDorb::portable::Stub*> (v);
  TIDorb::portable::ObjectDelegate* delegate = _stub->_get_delegate();

  //jagd
  //TIDorb::core::ObjectDelegateImpl* _obj_del_impl =
  //  dynamic_cast<TIDorb::core::ObjectDelegateImpl*> (delegate);
  TIDorb::core::ObjectDelegateImpl* _obj_del_impl =
    (TIDorb::core::ObjectDelegateImpl*) (delegate);

  if (_obj_del_impl != NULL) {
    TIDorb::core::iop::IOR* ref = _obj_del_impl->getReference();
    ref->write(*this);
  } else {
    // write IOR from another ORB object
    TIDorb::core::iop::IOR ref;
    TIDorb::portable::ORB* _orb = delegate->orb();
    //jagd 
    //TIDorb::core::TIDORB* _tidorb = dynamic_cast<TIDorb::core::TIDORB*> (_orb);
    TIDorb::core::TIDORB* _tidorb = (TIDorb::core::TIDORB*) (_orb);
    ref.fromString(_tidorb, _orb->object_to_string(v));
    ref.write(*this);
  }
}




void CDROutputStream::write_Context(const CORBA::Context_ptr ctx,
                                    const CORBA::ContextList_ptr contexts)
{
  TIDorb::core::ContextImpl::write(*this, ctx, contexts);
}




void CDROutputStream::write_Principal(const char* v)
{
  write_string(v);
}




void CDROutputStream::write_any(const ::CORBA::Any& v)
{
  CORBA::TypeCode_var tc = v.type();
  write_TypeCode(tc);
  v.delegate().write_value(*this);
}




// void CDROutputStream::write_octet_array(const ::CORBA::Octet v[],
//                                         ::CORBA::ULong offset,
//                                         ::CORBA::ULong length)
// {
//   if (v == 0)
//     throw CORBA::BAD_PARAM();

//   ::CORBA::ULong octets_can_write = 0;

//   // numero de octetos que quedan por escribir
//   ::CORBA::ULong remain_octets = length;

// //MLG
//   while (true)
//     {
//       octets_can_write = (::CORBA::ULong)
//         ((uintptr_t) m_chunk->get_buffer_end() - (uintptr_t) m_next);

//       if(octets_can_write > remain_octets)
//         octets_can_write = remain_octets;

//       memcpy((void *) m_next, (const void *) v, octets_can_write);
//       m_next+= octets_can_write;
//       v+= octets_can_write;
//       remain_octets -= octets_can_write;
// //EMLG
//       if(remain_octets > 0)
//         {
//           get_next_chunk();
//         }
//       else
//         {
//           if((uintptr_t) m_next > (uintptr_t)m_chunk->get_available_end())
//             m_chunk->set_available_end(m_next);
//           break;
//         }
//     }
// }




void CDROutputStream::write_boolean_array(const ::CORBA::Boolean v[],
                                          ::CORBA::ULong offset,
                                          ::CORBA::ULong length)
{
  write_octet_array((::CORBA::Octet*) v, offset, length);
}




void CDROutputStream::write_char_array(const ::CORBA::Char v[],
                                       ::CORBA::ULong offset,
                                       ::CORBA::ULong length)
{
  write_octet_array((::CORBA::Octet*) v, offset, length);
}




void CDROutputStream::write_wchar_array(const ::CORBA::WChar v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length)
{
  CORBA::ULong index = offset;
  for (CORBA::ULong i = 0; i < length; i++)
    write_wchar(v[index++]);
}




void CDROutputStream::write_short_array(const ::CORBA::Short v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::SHORT_SIZE);
}




void CDROutputStream::write_long_array(const ::CORBA::Long v[],
                                       ::CORBA::ULong offset,
                                       ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::LONG_SIZE);
}




void CDROutputStream::write_longlong_array(const ::CORBA::LongLong v[],
                                           ::CORBA::ULong offset,
                                           ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::LONGLONG_SIZE);
}




void CDROutputStream::write_ushort_array(const ::CORBA::UShort v[],
                                         ::CORBA::ULong offset,
                                         ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::USHORT_SIZE);
}




void CDROutputStream::write_ulong_array(const ::CORBA::ULong v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::ULONG_SIZE);
}




void CDROutputStream::write_ulonglong_array(const ::CORBA::ULongLong v[],
                                            ::CORBA::ULong offset,
                                            ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::ULONGLONG_SIZE);
}




void CDROutputStream::write_float_array(const ::CORBA::Float v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::FLOAT_SIZE);
}




void CDROutputStream::write_double_array(const ::CORBA::Double v[],
                                         ::CORBA::ULong offset,
                                         ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::DOUBLE_SIZE);
}




void CDROutputStream::write_longdouble_array(const ::CORBA::LongDouble v[],
                                             ::CORBA::ULong offset,
                                             ::CORBA::ULong length)
{
  write_data_array((const ::CORBA::Octet*) v, offset, length, CDR::LONG_DOUBLE_SIZE);
}




void CDROutputStream::write_string_array(const ::CORBA::Char* v[],
                                         ::CORBA::ULong offset,
                                         ::CORBA::ULong length)
{
  CORBA::ULong end = offset + length;

  for (CORBA::ULong i = offset; i < end; i++)
    write_string(v[i]);
}




void CDROutputStream::write_wstring_array(const ::CORBA::WChar* v[],
                                          ::CORBA::ULong offset,
                                          ::CORBA::ULong length)
{
  CORBA::ULong end = offset + length;

  for (CORBA::ULong i = offset; i < end; i++)
    write_wstring(v[i]);
}




void CDROutputStream::write_fixed_array(const ::CORBA::Fixed v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length,
                                        ::CORBA::UShort digits,
                                        ::CORBA::Short scale)
{
  CORBA::ULong end = offset + length;

  for (CORBA::ULong i = offset; i < end; i++)
    write_fixed(v[i], digits, scale);
}




void CDROutputStream::write_any_array(const ::CORBA::Any v[],
                                      ::CORBA::ULong offset,
                                      ::CORBA::ULong length)
{
  CORBA::ULong end = offset + length;

  for (CORBA::ULong i = offset; i < end; i++)
    write_any(v[i]);
}




void CDROutputStream::write_data_array(const ::CORBA::Octet v[],
                                       ::CORBA::ULong offset,
                                       ::CORBA::ULong length,
                                       ::CORBA::ULong type_size)
{
  if (v == 0)
    throw CORBA::BAD_PARAM();

  ::CORBA::ULong octets_can_write = 0;
  ::CORBA::ULong values_can_write = 0;

  // numero de octetos que quedan por escribir
  ::CORBA::ULong remain_values = length;

  alignment(type_size);

  while (true)
    {

      octets_can_write = (::CORBA::ULong)
        ((uintptr_t) m_chunk->get_buffer_end() - (uintptr_t) m_next);

      values_can_write = octets_can_write / type_size;

      if(values_can_write > length)
        values_can_write = length;

      octets_can_write = values_can_write * type_size;

      memcpy((void *) m_next, (const void *) v, octets_can_write);

      m_next+= octets_can_write;
      remain_values -= values_can_write;

      if(remain_values > 0)
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




bool CDROutputStream::writeValueTags(const CORBA::ValueBase_ptr value)
{
    // write null
    if (!value) {
      ValueTypeInfo::write_null(*this);
      return true;
    }

    CDR::AbsolutePosition previous_position = get_context()->lookup(value);

    // write indirection of the value that was marshalled
    if (previous_position != 0) {
      write_indirection(previous_position);
      return true;
    }

    // write the value
    return false;
}




void CDROutputStream::write_Value(const CORBA::ValueBase_ptr value)
{
    if (writeValueTags(value)) {
      return;
    }

    // Initialize the marshalling info about the value

    ValueTypeInfo value_info;

    CORBA::TypeCode_var type = value->_type();
    // TODO: set truncatable values: value->_truncatable_ids();

    const char* id = type->id();
    value_info.set_repository_ids(&id, 1);
    value_info.set_value(value);

    alignment(CDR::ULONG_SIZE);

    CDR::AbsolutePosition position = get_absolute_position();

    value_info.write(*this);
    
    get_context()->put(position, value_info);


    // TODO: check CustomMarshal in CDROutputStream::write_Value()
/*  CORBA::CustomMarshal_ptr custom =
      dynamic_cast<CORBA::CustomMarshal_ptr> (value);

    if(custom) { // is custom
        custom->marshal(*this);
    } else */ if(value) { // is value base
        value->_write(*this);
    } else { // is null
        ValueTypeInfo::write_null(*this);
    }
}




void CDROutputStream::write_Abstract(const CORBA::AbstractBase_ptr value)
{
    if (!value) {
        throw CORBA::BAD_PARAM("Null value reference");
    }

    CORBA::Object_var ref = value->_to_object();

    //jagd
    //if (!CORBA::is_nil(ref)) {
    if ((ref)) {
        // is an object ref
        write_boolean(true);
        write_Object(ref);
        return;
    }

    CORBA::ValueBase_var val = value->_to_value();

    write_boolean(false);
    write_Value(val);
}




void CDROutputStream::write_referenceable_string
    (const char* value)
{
        CDR::AbsolutePosition previous_position = get_context()->lookup(value);

        if (previous_position != 0) {
            write_indirection(previous_position);
            return;
        }

        // aligns to get the id position

        alignment(CDR::LONG_SIZE);

        // calculate the typecode position

        CDR::AbsolutePosition id_position = get_absolute_position();

        get_context()->put(id_position, value);

        write_string(value);
}




void CDROutputStream::write_indirection
  (CDR::AbsolutePosition previous_position)
{
  write_long(0xFFFFFFFF);

  // prevents that indirection value will be marshaled in the next chunk

  alignment(CDR::LONG_SIZE);

  CDR::AbsolutePosition indirection_position;

  indirection_position = get_absolute_position();

  CORBA::Long offset =
    (CORBA::Long)((CORBA::LongLong) previous_position - (CORBA::LongLong) indirection_position);

  write_long(offset);

  return;
}

CDROutputStream* CDROutputStream::create_encapsulation()
{
  // calculate the encapsulation position

  alignment(CDR::ULONG_SIZE);
  CDR::AbsolutePosition encapsulation_position = get_absolute_position() + CDR::ULONG_SIZE;

  // create the encapsulation stream
  BufferCDR* buffer;
  if(m_orb)
   buffer = new BufferCDR(m_orb->conf().block_size, 
                          get_context(), 
                          encapsulation_position);
 else 
   buffer = new BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE,                              
                          get_context(), 
                          encapsulation_position); 
   
  CDROutputStream* output = new CDROutputStream(m_orb, buffer);

  // write encapsulation byte order
  output->write_boolean(output->get_byte_order());

  return output;
}

void CDROutputStream::write_buffer(BufferCDR& buffer)
{
  write_ulong(buffer.get_available_bytes());
  CORBA::ULong num_chunks = buffer.get_num_available_chunks();
  ChunkCDR* chunk;

  for (CORBA::ULong i = 0; i < num_chunks; i++) {
      chunk = buffer.get_chunk(i);
      write_octet_array((CORBA::Octet *) chunk->get_buffer(), 0,  chunk->get_available_bytes());
  }
}




void CDROutputStream::get_next_chunk()
{
  m_chunk->set_available_end(m_chunk->get_buffer_end());

  if (m_current_chunk_num == (m_bufferCDR->get_num_chunks() -1))
    m_bufferCDR->grow();

  next_chunk();
}




/**
 * Set the stream pointer to the next aligned position of given data type and
 * sets it as the last available position
 * @param type_size the type size in octets
 */
void CDROutputStream::fix_next_aligned_position(CORBA::ULong type_size)
{
  go_next_aligned_position(type_size);

  if (m_next > m_chunk->get_available_end())
    m_chunk->set_available_end(m_next);
}




} //namespace cdr
} //namespace core
} //namespace TIDorb
