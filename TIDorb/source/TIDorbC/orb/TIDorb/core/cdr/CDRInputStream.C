/////////////////////////////////////////////////////////////////////////
//
// File:        CDRInputStream.C
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
#include "TIDorb/core/typecode/TypeCodeCache.h"
#include "TIDorb/types/BigInt.h"
#include "TIDorb/util.h"
#include "TIDorb/core/util/GenericValue.h"

#include <string.h>


using TIDorb::core::typecode::TypeCodeImpl;
using TIDorb::core::util::GenericValue;




namespace TIDorb {
namespace core {
namespace cdr {





CDRInputStream::CDRInputStream(TIDorb::core::TIDORB* orb,
                               BufferCDR* buffer)
  : IteratorCDR(orb, buffer)
{
    m_byte_order = buffer->get_byte_order();
    point_ok=true;
    any_ref=NULL;
}




CDRInputStream::CDRInputStream(const CDRInputStream& input)
  : IteratorCDR(input)
{
    m_byte_order = m_bufferCDR->get_byte_order();
    point_ok= input.point_ok;
}




CDRInputStream::CDRInputStream(const CDROutputStream& output)
  : IteratorCDR(output)
{
    m_byte_order = m_bufferCDR->get_byte_order();
}




void CDRInputStream::orb(TIDorb::core::TIDORB* ptr)
{
  m_orb = ptr;
}




TIDorb::portable::ORB* CDRInputStream::orb()
{
  return m_orb;
}

TIDorb::core::TIDORB* CDRInputStream::tidorb()
{
  return m_orb;
}

CDRInputStream* CDRInputStream::copy()
{
  return new CDRInputStream(*this);
}

CDRInputStream* CDRInputStream::deep_copy()
{
  CDRInputStream* input = new CDRInputStream(*this);
  // And BufferCDR duplicate
  input->m_bufferCDR = this->m_bufferCDR->duplicate();
  input->m_chunk = input->m_bufferCDR->get_chunk(input->m_current_chunk_num);
  input->m_next = input->m_chunk->get_buffer() + 
    (this->m_next - this->m_chunk->get_buffer());
  input->m_starting = PointerCDR(input->m_bufferCDR, input->m_current_chunk_num, 
                                 input->m_next);
  // input->m_bufferCDR_ptr_owned = true;
  return input;
}

void CDRInputStream::read_char(::CORBA::Char& v)
{
  read_octet((::CORBA::Octet&) v);
}

void CDRInputStream::read_wchar(::CORBA::WChar& v)
{
    CORBA::UShort value;
    read_ushort(value);
    v = value;
}

void CDRInputStream::read_fixed(::CORBA::Fixed& v,
                                CORBA::UShort digits,
                                CORBA::Short scale)
{
  CORBA::ULong value_length =
    TIDorb::core::typecode::FixedTypeCode::value_length(digits);

  //char* buffer = new char[value_length];
  char* buffer = (char*) malloc(value_length+1);

  CORBA::ULong pairs = value_length / 2;

  CORBA::Octet octet;
  CORBA::ULong index = 0;

  for (CORBA::ULong i = 1 ; i < pairs; i++) {
    read_octet(octet);
    // getDigit(octet) de Base16Codec devuelve el caracter correspondiente al
    // digito que viene de la parte del octeto que se le pasa.
    buffer[index++] = TIDorb::util::Base16Codec::getDigit(0xf & (octet>>4));
    // El buffer contendra la cadena de digitos que representa el numero entero.
    buffer[index++] = TIDorb::util::Base16Codec::getDigit(0xf & octet);
  }

  // last pair
  read_octet(octet);
  buffer[index++]=TIDorb::util::Base16Codec::getDigit(0xf & (octet>>4));

  // Make sure that string buffer ends in '\0'
  buffer[value_length] = '\0';

  bool positive = false;

  switch (0xf & octet) {
    case 0xD:
      positive = false;
      break;
    case 0xC:
      positive = true;
      break;
    default:
      throw CORBA::MARSHAL("Fixed_read: Invalid signum value",
                           0, CORBA::COMPLETED_NO);
  }

  TIDorb::types::BigInt value_int(buffer); 
  if (positive)
    v = CORBA::Fixed(value_int,scale);
  else
    v = CORBA::Fixed(-value_int,scale);
  free(buffer);
}



/*
void CDRInputStream::read_short(::CORBA::Short& v)
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
*/

/*
void CDRInputStream::read_longlong(::CORBA::LongLong& v)
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

*/

/*
void CDRInputStream::read_ushort(::CORBA::UShort& v)
{
  read_short((::CORBA::Short&) v);
}
*/


/*
void CDRInputStream::read_ulong(::CORBA::ULong& v)
{
  read_long((::CORBA::Long&) v);
}
*/



void CDRInputStream::read_ulonglong(::CORBA::ULongLong& v)
{
  read_longlong((::CORBA::LongLong&) v);
}



/*
void CDRInputStream::read_float(::CORBA::Float& v)
{
  CORBA::Long value;
  read_long(value);
  v = *((CORBA::Float*) &value);
}
*/


/*
void CDRInputStream::read_double(::CORBA::Double& v)
{
  read_longlong((::CORBA::LongLong&) v);
}
*/



void CDRInputStream::read_longdouble(::CORBA::LongDouble& v)
{
  alignment(CDR::LONGLONG_SIZE); // long double is aligned to 8
  if (m_byte_swap) {
    register unsigned char* sp = (unsigned char*) &v;
    sp[15] = *m_next++;
    sp[14] = *m_next++;
    sp[13] = *m_next++;
    sp[12] = *m_next++;
    sp[11] = *m_next++;
    sp[10] = *m_next++;
    sp[9]  = *m_next++;
    sp[8]  = *m_next++;
    sp[7]  = *m_next++;
    sp[6]  = *m_next++;
    sp[5]  = *m_next++;
    sp[4]  = *m_next++;
    sp[3]  = *m_next++;
    sp[2]  = *m_next++;
    sp[1]  = *m_next++;
    sp[0]  = *m_next++;
  } else {
    v = *((::CORBA::LongDouble*) m_next);
    m_next += CDR::LONG_DOUBLE_SIZE;
  }
}




// void CDRInputStream::read_string(char*& v)
// {
//   CORBA::ULong length;
//   read_ulong(length);
  
//   if(length == 0)
//   	throw CORBA::MARSHAL("Invalid string length");
  	
//   v = CORBA::string_alloc(length-1);
//   //read_char_array(v, 0, length);
//   read_octet_array(v, 0, length);
// }


// void CDRInputStream::read_string(char*& v, CORBA::ULong bound)
// {
//   CORBA::ULong length;
//   read_ulong(length);

//   if (length == 0)
//     throw CORBA::MARSHAL("Invalid string length");

//   if (length > bound + 1) 
//     throw CORBA::BAD_PARAM("String length exceeds bound");

//   v = CORBA::string_alloc(length);
//   //read_char_array(v, 0, length);
//   read_octet_array(v, 0, length);
// }


void CDRInputStream::read_wstring(::CORBA::WChar*& v)
{
    // read the string length, in wstring there is a null '\0' character at
    // the end

    CORBA::ULong length;

    read_ulong(length);

    // the length is the number of bytes in the wide-string
    CORBA::ULong real_length = 0;

    if (m_version == TIDorb::core::comm::iiop::Version::VERSION_1_2) {
        real_length = (length / CDR::WCHAR_SIZE);
    } else {
        real_length = length;
    }

    if (real_length < 0)
        throw  CORBA::MARSHAL(); // invalid string  length: " + length
    if (real_length == 0) {
        skip_wchar();
        v = new CORBA::WChar[1];
        v[0] = '\0';
    } else {
        v = new CORBA::WChar[real_length];
        read_wchar_array(v, 0, real_length);
    }
}




void CDRInputStream::read_any(::CORBA::Any& v)
{
  CORBA::TypeCode_ptr tc;
  read_TypeCode(tc);
  //jagd 
  //(dynamic_cast<TIDorb::core::AnyImpl&>(v.delegate())).orb(m_orb);
  ((TIDorb::core::AnyImpl*)(&(v.delegate())))->orb(m_orb);
  //v.delegate().read_value(*this, tc);
  v.delegate().read_value(*this, tc, true); 
  CORBA::release(tc);
}




void CDRInputStream::read_Object(::CORBA::Object_out v)
{
  TIDorb::core::iop::IOR* ior = new TIDorb::core::iop::IOR();

  ior->read(*this);
  // ior.read(this);

  if ((ior->member_count() == 0) || (!m_orb)) {
    v = NULL;
    delete ior;
  } else {
    v = ::TIDorb::core::ObjectImpl::fromIOR(m_orb, ior);
  }
}




void CDRInputStream::read_Principal(char*& v)
{
  read_string(v);
}




void CDRInputStream::read_Context(::CORBA::Context_out v)
{
  v = TIDorb::core::ContextImpl::read(*this);
}




void CDRInputStream::skip_Context()
{
  CORBA::ULong size;
  read_ulong(size);
  skip_string_array(size);
}




void CDRInputStream::skip_string()
{
  CORBA::ULong length;
  read_ulong(length);
  skip_char_array(length);
}




void CDRInputStream::skip_wstring()
{
  CORBA::ULong length;
  read_ulong(length);
  skip_octet_array(length);
}




void CDRInputStream::skip_any()
{
  CORBA::TypeCode_ptr tc;
  read_TypeCode(tc);

  TypeCodeImpl* type = NULL;
  //jagd
  //type = (TypeCodeImpl*) (tc)->_impl();
  type = (TypeCodeImpl*) (tc);

  type->skip_value(*this);

  CORBA::release(tc);
}




void CDRInputStream::skip_TypeCode()
{
  CORBA::TypeCode_ptr tc;
  read_TypeCode(tc);
  CORBA::release(tc);
}




void CDRInputStream::skip_Object()
{
  throw CORBA::NO_IMPLEMENT();
}




void CDRInputStream::skip_type(::CORBA::ULong size)
{
  alignment(size);
  m_next += size;
}




void CDRInputStream::read_octet_array(CORBA::Octet v[],
                                      CORBA::ULong offset,
                                      CORBA::ULong length)
{
  // numero de octetos pertenecientes a datos que se pueden leer enteros
  CORBA::ULong octets_can_read = 0;
  CORBA::ULong chunk_available = 0;

  // numero de datos que quedan por saltar
  CORBA::ULong remain_data = length;

  while (remain_data > 0) {
//MLG
    alignment(CDR::OCTET_SIZE);
    chunk_available = available();
    octets_can_read =
        (remain_data < chunk_available)? remain_data : chunk_available;

    memcpy(v, m_next, octets_can_read);
    m_next += octets_can_read;
    v += octets_can_read;
    remain_data -= octets_can_read;
//EMLG
    if(remain_data > 0)
    {
     get_next_chunk();
    }
  }
}




void CDRInputStream::read_boolean_array(::CORBA::Boolean v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length)
{
  read_octet_array((::CORBA::Octet*) v, offset, length);
}




void CDRInputStream::read_char_array(::CORBA::Char v[],
                                     ::CORBA::ULong offset,
                                     ::CORBA::ULong length)
{
  read_octet_array((::CORBA::Octet*) v, offset, length);
}




void CDRInputStream::read_short_array(CORBA::Short v[],
                                      CORBA::ULong offset,
                                      CORBA::ULong length)
{
  // numero de octetos pertenecientes a datos que se pueden leer enteros
  CORBA::ULong octets_can_read = 0;
  CORBA::ULong chunk_available = 0;

  CORBA::Short* v_ptr = v + offset;
  register unsigned char* sp = (unsigned char*) v_ptr;

  // numero de datos que quedan por saltar
  CORBA::ULong remain_data = length;
  // numero de valores que quedan por saltar
  CORBA::ULong values_can_read = 0;

  while (remain_data > 0) {

    alignment(CDR::SHORT_SIZE);
    octets_can_read = remain_data * CDR::SHORT_SIZE;
    chunk_available = available();
    octets_can_read =
        (octets_can_read < chunk_available)? octets_can_read : chunk_available;
    values_can_read = octets_can_read / CDR::SHORT_SIZE;

    if (m_byte_swap) {
      for (CORBA::ULong i = 0; i < values_can_read; i++) {
        sp[1] = *m_next++;
        sp[0] = *m_next++;
      }
    } else {
      memcpy(sp, m_next, octets_can_read);
      m_next += octets_can_read;
    }

    remain_data -= values_can_read;
    sp += octets_can_read;

    if (remain_data > 0) {
      get_next_chunk();
    }
  }
}




void CDRInputStream::read_long_array(CORBA::Long v[],
                                     CORBA::ULong offset,
                                     CORBA::ULong length)
{
  // numero de octetos pertenecientes a datos que se pueden leer enteros
  CORBA::ULong octets_can_read = 0;
  CORBA::ULong chunk_available = 0;

  CORBA::Long* v_ptr = v + offset;
  register unsigned char* sp = (unsigned char*) v_ptr;

  // numero de datos que quedan por saltar
  CORBA::ULong remain_data = length;
  // numero de valores que quedan por saltar
  CORBA::ULong values_can_read = 0;

  while (remain_data > 0) {

    alignment(CDR::LONG_SIZE);
    octets_can_read = remain_data * CDR::LONG_SIZE;
    chunk_available = available();
    octets_can_read =
      (octets_can_read < chunk_available)? octets_can_read : chunk_available;
    values_can_read = octets_can_read / CDR::LONG_SIZE;

    if (m_byte_swap) {
       for(CORBA::ULong i = 0; i < values_can_read; i++) {
        sp[3] = *m_next++;
        sp[2] = *m_next++;
        sp[1] = *m_next++;
        sp[0] = *m_next++;
      }
    } else {
      memcpy(sp, m_next, octets_can_read);
      m_next += octets_can_read;
    }

    remain_data -= values_can_read;
    sp += octets_can_read;

    if (remain_data > 0) {
      get_next_chunk();
    }
  }
}




void CDRInputStream::read_longlong_array(CORBA::LongLong v[],
                                         CORBA::ULong offset,
                                         CORBA::ULong length)
{
  // numero de octetos pertenecientes a datos que se pueden leer enteros
  CORBA::ULong octets_can_read = 0;
  CORBA::ULong chunk_available = 0;

  CORBA::LongLong* v_ptr = v + offset;
  register unsigned char  *sp = (unsigned char *) v_ptr;

  // numero de datos que quedan por saltar
  CORBA::ULong remain_data = length;
  // numero de valores que quedan por saltar
  CORBA::ULong values_can_read = 0;

  while (remain_data > 0) {

    alignment(CDR::LONGLONG_SIZE);
    octets_can_read = remain_data * CDR::LONGLONG_SIZE;
    chunk_available = available();
    octets_can_read =
      (octets_can_read < chunk_available)? octets_can_read : chunk_available;
    values_can_read = octets_can_read / CDR::LONGLONG_SIZE;

    if (m_byte_swap) {
      for (CORBA::ULong i = 0; i < values_can_read; i++) {
        sp[7] = *m_next++;
        sp[6] = *m_next++;
        sp[5] = *m_next++;
        sp[4] = *m_next++;
        sp[3] = *m_next++;
        sp[2] = *m_next++;
        sp[1] = *m_next++;
        sp[0] = *m_next++;
      }
    } else {
      memcpy(sp, m_next, octets_can_read);
      m_next += octets_can_read;
    }

    remain_data -= values_can_read;
    sp += octets_can_read;

    if (remain_data > 0) {
      get_next_chunk();
    }
  }
}




void CDRInputStream::read_ushort_array(::CORBA::UShort v[],
                                       ::CORBA::ULong offset,
                                       ::CORBA::ULong length)
{
  read_short_array((::CORBA::Short*) v, offset, length);
}




void CDRInputStream::read_ulong_array(::CORBA::ULong v[],
                                      ::CORBA::ULong offset,
                                      ::CORBA::ULong length)
{
  read_long_array((::CORBA::Long*) v, offset, length);
}




void CDRInputStream::read_ulonglong_array(::CORBA::ULongLong v[],
                                          ::CORBA::ULong offset,
                                          ::CORBA::ULong length)
{
  read_longlong_array((::CORBA::LongLong*) v, offset, length);
}




void CDRInputStream::read_float_array(::CORBA::Float v[],
                                      ::CORBA::ULong offset,
                                      ::CORBA::ULong length)
{
  read_long_array((::CORBA::Long*) v, offset, length);
}




void CDRInputStream::read_double_array(::CORBA::Double v[],
                                       ::CORBA::ULong offset,
                                       ::CORBA::ULong length)
{
  read_longlong_array((::CORBA::LongLong*) v, offset, length);
}




void CDRInputStream::read_longdouble_array(CORBA::LongDouble v[],
                                           CORBA::ULong offset,
                                           CORBA::ULong length)
{
  // numero de octetos pertenecientes a datos que se pueden leer enteros
  CORBA::ULong octets_can_read = 0;
  CORBA::ULong chunk_available = 0;

  CORBA::LongDouble* v_ptr = v + offset;
  register unsigned char* sp = (unsigned char*) v_ptr;

  // numero de datos que quedan por saltar
  CORBA::ULong remain_data = length;
  // numero de valores que quedan por saltar
  CORBA::ULong values_can_read = 0;

  while (remain_data > 0) {

    alignment(CDR::LONGLONG_SIZE); // long double is aligned to 8
    octets_can_read = remain_data * CDR::LONG_DOUBLE_SIZE;
    chunk_available = available();
    octets_can_read =
      (octets_can_read < chunk_available)? octets_can_read : chunk_available;
    values_can_read = octets_can_read / CDR::LONG_DOUBLE_SIZE;

    if (m_byte_swap) {
      for (CORBA::ULong i = 0; i < values_can_read; i++) {
        sp[15] = *m_next++;
        sp[14] = *m_next++;
        sp[13] = *m_next++;
        sp[12] = *m_next++;
        sp[11] = *m_next++;
        sp[10] = *m_next++;
        sp[9]  = *m_next++;
        sp[8]  = *m_next++;
        sp[7]  = *m_next++;
        sp[6]  = *m_next++;
        sp[5]  = *m_next++;
        sp[4]  = *m_next++;
        sp[3]  = *m_next++;
        sp[2]  = *m_next++;
        sp[1]  = *m_next++;
        sp[0]  = *m_next++;
      }
    } else {
      memcpy(sp, m_next, octets_can_read);
      m_next += octets_can_read;
    }

    remain_data -= values_can_read;
    sp += octets_can_read;

    if (remain_data > 0) {
      get_next_chunk();
    }
  }
}




void CDRInputStream::read_wchar_array(CORBA::WChar v[],
                                      CORBA::ULong offset,
                                      CORBA::ULong length)
{
  CORBA::ULong index = offset;
  for (CORBA::ULong i = 0; i < length; i++)
    read_wchar(v[index++]);
}




void CDRInputStream::read_fixed_array(CORBA::Fixed v[],
                                      CORBA::ULong offset,
                                      CORBA::ULong length,
                                      CORBA::UShort digits,
                                      CORBA::Short scale)
{
  CORBA::ULong index = offset;
  for (CORBA::ULong i = 0; i < length; i++)
    read_fixed(v[index++], digits, scale);
}




void CDRInputStream::read_string_array(char* v[],
                                       ::CORBA::ULong offset,
                                       ::CORBA::ULong length)
{
  CORBA::ULong index = offset;
  for (CORBA::ULong i = 0; i < length; i++)
    read_string(v[index++]);
}




void CDRInputStream::read_wstring_array(::CORBA::WChar* v[],
                                        ::CORBA::ULong offset,
                                        ::CORBA::ULong length)
{
  CORBA::ULong index = offset;
  for (CORBA::ULong i = 0; i < length; i++)
    read_wstring(v[index++]);
}




void CDRInputStream::read_any_array(::CORBA::Any v[],
                                    ::CORBA::ULong offset,
                                    ::CORBA::ULong length)
{
  CORBA::ULong index = offset;
  for (CORBA::ULong i = 0; i < length; i++)
    read_any(v[index++]);
}




void CDRInputStream::skip_type_array(CORBA::ULong length,
                                     CORBA::ULong type_length)
{
  // numero de octetos pertenecientes a datos que se pueden leer enteros
  CORBA::ULong octets_can_skip = 0;
  CORBA::ULong chunk_available = 0;

  // numero de datos que quedan por saltar
  CORBA::ULong remain_data = length;

  while (remain_data > 0) {

    alignment(type_length);
    octets_can_skip = remain_data * type_length;
    chunk_available = available();
    octets_can_skip =
      (octets_can_skip < chunk_available)? octets_can_skip : chunk_available;
    remain_data -= octets_can_skip / type_length;
    m_next += octets_can_skip;

    if (remain_data > 0) {
      get_next_chunk();
    }
  }
}




void CDRInputStream::skip_fixed_array(CORBA::ULong length,
                                      ::CORBA::UShort digits,
                                      ::CORBA::UShort scale)
{
  throw CORBA::NO_IMPLEMENT();
}




void CDRInputStream::skip_string_array(CORBA::ULong length)
{
  for (CORBA::ULong i = 0; i < length; i++)
    skip_string();
}




void CDRInputStream::skip_wstring_array(CORBA::ULong length)
{
  for (CORBA::ULong i = 0; i < length; i++)
    skip_wstring();
}




void CDRInputStream::skip_any_array(CORBA::ULong length)
{
  for (CORBA::ULong i = 0; i < length; i++)
    skip_any();
}




void CDRInputStream::read_TypeCode(CORBA::TypeCode_out tc)
{

  // aligns to get the typeCode position

  alignment(CDR::LONG_SIZE);

  // gets the typecode position

  CDR::AbsolutePosition typecode_position = get_absolute_position();

  // read the tc_kind value
  CORBA::Long kind;

  read_long(kind);

  if (kind == -1) // indirection
  {
    tc = CORBA::TypeCode::_duplicate(read_indirected_TypeCode());
    
    return;
  }

  TypeCodeImpl* type = 0;

  // Test Simple or complex TypeCode
  // jagd cambiamos el orden de obtener get_empty_TypeCode
  if ((kind!=CORBA::tk_struct)&&
      (kind!=CORBA::tk_objref)&&
      (kind!=CORBA::tk_union)&&
      (kind!=CORBA::tk_enum)&&
      (kind!=CORBA::tk_string)&&
      (kind!=CORBA::tk_sequence)&&
      (kind!=CORBA::tk_array)&&
      (kind!=CORBA::tk_alias)&&
      (kind!=CORBA::tk_except)&&
      (kind!=CORBA::tk_wstring)&&
      (kind!=CORBA::tk_fixed)&&
      (kind!=CORBA::tk_value)&&
      (kind!=CORBA::tk_value_box)&&
      (kind!=CORBA::tk_native)&&
      (kind!=CORBA::tk_abstract_interface))
  {
/*
  type =
  TIDorb::core::typecode::TypeCodeFactory::get_empty_TypeCode((CORBA::TCKind) kind);

  // Test if the TCKind is a basic TypeCode
  if (!type)
  {*/
    tc = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode((CORBA::TCKind) kind);
//MLT
    if(!tc)
//EMLG
      throw CORBA::MARSHAL(); //("Error reading typecode");

    return;
  }

//MLG
  CORBA::Boolean is_complex = false;

  switch (kind)
  {
        case CORBA::tk_struct:
        case CORBA::tk_objref:
        case CORBA::tk_union:
        case CORBA::tk_enum:
        case CORBA::tk_alias:
        case CORBA::tk_except:
        case CORBA::tk_value:
        case CORBA::tk_value_box:
        case CORBA::tk_native:
        case CORBA::tk_abstract_interface:
                is_complex = true;
                    break;
        default:
            is_complex = false;
  }


  if (is_complex) {
        CORBA::RepositoryId_var rep_id;

	// Read in advance the repository ID from the encapsulation
        CDRInputStream copy_stream(*this);        
        CORBA::Boolean byte_order;
        
        //skip encapsulation length
        copy_stream.skip_ulong();
        
        copy_stream.read_boolean(byte_order);
        copy_stream.set_byte_order(byte_order);
        copy_stream.read_string(rep_id.out());        

        TIDorb::core::typecode::TypeCodeCache* cache = NULL;
        if (m_orb)
          cache = m_orb->getTypeCodeCache();
        TypeCodeImpl* cached_type = NULL;

        if (cache) {
          CORBA::TypeCode_ptr paux = NULL;
          paux = cache->find(rep_id);
          if (paux) {
            //jagd 
            //cached_type = (TypeCodeImpl*) paux->_impl();
            cached_type = (TypeCodeImpl*) paux;
          }
        }

        if (cached_type) {
          skip_encapsulation();
          /* if (type){
             CORBA::release(type);
             }*/
          type = cached_type;
        } else {
            type =
              TIDorb::core::typecode::TypeCodeFactory::get_empty_TypeCode((CORBA::TCKind) kind);
            if (m_orb)
              type->set_exhaustive_equal(m_orb->conf().exhaustive_equal);
            type->partial_unmarshal(*this);

            if (cache) {
              cache->put(CORBA::TypeCode::_duplicate(type));
            }
        }
        // it is a complex typecode: insert the typecode in the table and unmarshal
        get_context()->put(typecode_position, type);
  } else {
        type =
          TIDorb::core::typecode::TypeCodeFactory::get_empty_TypeCode((CORBA::TCKind) kind);
        if (m_orb)
          type->set_exhaustive_equal(m_orb->conf().exhaustive_equal);
        type->partial_unmarshal(*this);
  }

  // ?queremos podemos guardar el typecode aplanado para otras veces?
//FRAN
  CORBA::release(tc);
//EFRAN
  tc = type;
//EMLG
}




void CDRInputStream::read_Value(CORBA::ValueBase_out value)
{
    value = read_Value();
}




//
//PRA
//

//CORBA::ValueBase_ptr CDRInputStream::read_Value()
//{
//    alignment(CDR::LONG_SIZE);
//
//    CDR::AbsolutePosition value_position = get_absolute_position();
//
//    ValueTypeInfo_ref value_info;
//
//    value_info = ValueTypeInfo::read(*this);
//
//    if(value_info.is_null()) {
//        throw CORBA::MARSHAL();
//    }
//    if (value_info->is_null()) {
//        return NULL;
//    }
//
//    if (value_info->isIndirection()) {
//        value_info = get_context()->lookup_value(value_position);        
//        return value_info->get_value();
//    }
//
//    const char* id = value_info->get_id();
//
//    if (!id) {
//        throw CORBA::MARSHAL();
//    }
//
//    if (value_info->isFragmented()) {
//        throw  CORBA::NO_IMPLEMENT();
//        //"Fragmented value reading not implemented"
//    }
//
//    CORBA::ValueBase* value = NULL;
//    if (m_orb)
//       value = create_value(m_orb, (const CORBA::RepositoryId) id);
//
//    // create_value is a static operation from TIDorb::portable::InputStream
//
//    if(!value) {
//        throw CORBA::MARSHAL();
//    }
//
//    CORBA::TypeCode_var type = value->_type();
//
//    const char* value_rep_id = type->id();
//
//    if ((!value_info->is_truncable(value_rep_id))) {
//        CORBA::remove_ref(value);
//        throw CORBA::MARSHAL(); // "Cannot truncate value
//    }

//    // TODO: check CustomMarshal in CDRInputStream::read_Value()
///*  CORBA::CustomMarshal* custom =
//        dynamic_cast<CORBA::CustomMarshal*> (value);
//
//    if(custom) {
//        custom->unmarshal(this);
//    } else */  {
//        value->_read(*this);
//    }
//
//    value_info->set_value(value);
//
//    get_context()->put(value_position, value_info);
//
//    return value;
//}


CORBA::ValueBase_ptr CDRInputStream::read_Value()
{
    alignment(CDR::LONG_SIZE);

    CDR::AbsolutePosition value_position = get_absolute_position();

    ValueTypeInfo value_info;

    value_info.read(*this);
    
    if (value_info.is_null()) {
        return NULL;
    }

    if (value_info.isIndirection()) {
        value_info = get_context()->lookup_value(value_position);
        return value_info.get_value();
    }

    const char* id = value_info.get_id();

    if (!id) {
        throw CORBA::MARSHAL();
    }

    if (value_info.isFragmented()) {
        throw CORBA::NO_IMPLEMENT();
        //"Fragmented value reading not implemented"
    }

    CORBA::ValueBase* value = NULL;
    if (m_orb)
      value = create_value(m_orb, (const CORBA::RepositoryId) id);

    // create_value is a static operation from TIDorb::portable::InputStream

    if(!value) {
        throw CORBA::MARSHAL();
    }

    CORBA::TypeCode_var type = value->_type();

    const char* value_rep_id = type->id();

    if ((!value_info.is_truncable(value_rep_id))) {        
        CORBA::remove_ref(value);
        throw CORBA::MARSHAL(); // "Cannot truncate value
    }

    // TODO: check CustomMarshal in CDRInputStream::read_Value()
/*  CORBA::CustomMarshal* custom =
        dynamic_cast<CORBA::CustomMarshal*> (value);

    if(custom) {
        custom->unmarshal(this);
    } else */  {
        value->_read(*this);
    }

    value_info.set_value(value);

    get_context()->put(value_position, value_info);

    return value;
}

//
//EPRA
//



CORBA::ValueBase_ptr CDRInputStream::read_Value(const CORBA::TypeCode_ptr type)
{
    alignment(CDR::LONG_SIZE);

    CDR::AbsolutePosition value_position = get_absolute_position();

    ValueTypeInfo value_info;

    value_info.read(*this);

    if (value_info.is_null()) {
        return NULL;
    }

    if (value_info.isIndirection()) {
        value_info = get_context()->lookup_value(value_position);
        return value_info.get_value();
    }

    const char* id = value_info.get_id();

    if (!id) {
        throw CORBA::MARSHAL();
    }

    if (value_info.isFragmented()) {
        throw CORBA::NO_IMPLEMENT();
        //"Fragmented value reading not implemented"
    }

    CORBA::ValueBase* value = NULL;
    try {    	
      if (m_orb)
        value = create_value(m_orb, (const CORBA::RepositoryId) id);
    } catch (const CORBA::MARSHAL& m) {
    }

    // create_value is a static operation from TIDorb::portable::InputStream

    if(!value) {
        value = new GenericValue(CORBA::TypeCode::_duplicate(type));
    }

    const char* value_rep_id = type->id();

    if (!value_info.is_truncable(value_rep_id)) {
        CORBA::remove_ref(value);
        throw CORBA::MARSHAL(); // "Cannot truncate value
    }

    value->_read(*this);

    value_info.set_value(value);

    get_context()->put(value_position, value_info);

    return value;
}




//PRA

void CDRInputStream::skip_Value(CORBA::TypeCode_ptr type)
{
        CORBA::ValueBase_var value = read_Value(type);
}


// void CDRInputStream::skip_Value(CORBA::TypeCode_ptr type)
// {
//     alignment(CDR::LONG_SIZE);

//     ValueTypeInfo value_info;

//     value_info.read(*this);
    
//     if (value_info.is_null()) {
//         return;
//     }

//     if (value_info.isIndirection()) {
//         return;
//     }

//     const char* id = value_info.get_id();

//     if (!id) {
//         throw CORBA::MARSHAL();
//     }

//     if (value_info.isFragmented()) {
//         throw CORBA::NO_IMPLEMENT();
//         //"Fragmented value reading not implemented"
//     }

//     CORBA::ValueBase* value = NULL;
//     try {    	
//        value = create_value(m_orb, (const CORBA::RepositoryId) id);
//     } catch (const CORBA::MARSHAL& m) {
//     }

//     // create_value is a static operation from TIDorb::portable::InputStream

//     const char* value_rep_id = type->id();
    
//     if ((!value_info.is_truncable(value_rep_id))) {        
//         CORBA::remove_ref(value);
//         throw CORBA::MARSHAL(); // "Cannot truncate value
//     }
    
//     if(!value) {
//         TIDorb::core::typecode::ValueTypeCode* value_type = dynamic_cast<TIDorb::core::typecode::ValueTypeCode*>(type);
//         value_type->partial_skip_value(*this);
//         return;
//     }   

//     value->_read(*this);
//     CORBA::remove_ref(value);
//     return;
// }
//EPRA




void CDRInputStream::skip_Abstract()
{
        CORBA::AbstractBase_var value = read_Abstract();
}




void CDRInputStream::read_Abstract(CORBA::AbstractBase_out value)
{
        value = read_Abstract();
}




CORBA::AbstractBase* CDRInputStream::read_Abstract()
{
    throw CORBA::NO_IMPLEMENT();
}




void CDRInputStream::skip(CORBA::ULong n)
{
  CORBA::ULong chunk_left = available();

  if (n <= chunk_left)  {
    m_next += n;
  } else {
    get_next_chunk();
    skip(n - chunk_left);
  }
}




CDR::AbsolutePosition
 CDRInputStream::read_indirection()
{
  // Prevent fragment jumps

  alignment(CDR::LONG_SIZE);

  // calculate the indirection

  CDR::AbsolutePosition current_position = get_absolute_position();

  // read indirection offset

  CORBA::Long offset;

  read_long(offset);

  // SELF-INDIRECTINIG not allowed

  if(offset >= - CDR::LONG_SIZE)
    throw  CORBA::MARSHAL(); //"Invalid offset");

  CDR::AbsolutePosition referenced_position;

  referenced_position = current_position + offset;

  if(referenced_position < 0)
    throw  CORBA::MARSHAL(); //"Invalid indirection: offset out of bounds."


  // align the position to read the typecode in a aligned position of long
  // this position alwais is aligned to long independently of encapsulation.

//  referenced_position.align(CDR.LONG_SIZE);

  return referenced_position;
}




CDRInputStream* CDRInputStream::read_encapsulation()
{
  CORBA::ULong length;

  read_ulong(length);

  alignment(CDR::OCTET_SIZE);

  CORBA::ULong absolute_position = get_absolute_position();

  BufferCDR* buffer = new BufferCDR(length, get_context(), absolute_position);

  read_octet_array((CORBA::Octet *) buffer->get_chunk(0)->get_buffer(), 0, length);

  buffer->set_available_bytes(length);
  
  CDRInputStream* input = new CDRInputStream(m_orb, buffer);

  CORBA::Boolean byte_order;  
  input->read_boolean(byte_order);
  input->set_byte_order(byte_order);
  buffer->set_byte_order(byte_order);

  return input;
}




void CDRInputStream::skip_encapsulation()
{
  CORBA::ULong length;
  read_ulong(length);
  skip_octet_array(length);
}




CORBA::TypeCode_ptr CDRInputStream::read_indirected_TypeCode()
{
  CDR::AbsolutePosition referenced_position = read_indirection();

  // search in the indirection node
  CORBA::TypeCode_ptr tc = 
    get_context()->lookup_typecode(referenced_position);

    return tc;
}

void CDRInputStream::read_indirected_string(char*& value)
{    
    alignment(CDR::LONG_SIZE);

    CDR::AbsolutePosition id_position = get_absolute_position();

    CORBA::Long tag;
    
    read_long(tag);
    
    if (tag == INDIRECTION_TAG) {
        CDR::AbsolutePosition str_position = read_indirection();
        
        const char* id = get_context()->lookup_string(str_position);

        if (id) {
            value = CORBA::string_dup(id);
            return;
        } else {
            throw CORBA::MARSHAL("Invalid String indirection");
        }
    } else if(tag > 0) {
        value = CORBA::string_alloc((CORBA::ULong) tag);
        //read_char_array(value, 0, (CORBA::ULong) tag);
        read_octet_array((::CORBA::Octet*) value, 0, (CORBA::ULong) tag);        
        get_context()->put(id_position, value);
    } else {
    	throw CORBA::MARSHAL("Invalid string size");
    }
}



void CDRInputStream::get_next_chunk()
{
  if(is_message())  {
   // tratar mensajes
  }

  next_chunk();
}




} // namespace cdr
} // namespace core
} // namespace TIDorb




namespace TIDorb {
namespace portable {


CORBA::ValueBase*
InputStream::create_value(CORBA::ORB_ptr orb, const char* id)
{
    CORBA::ValueFactory_var factory =
        orb->lookup_value_factory((const CORBA::RepositoryId) id);

    if (!((CORBA::ValueFactory)factory)) {
        throw  CORBA::MARSHAL(); //"Cannot get Value Factory"
    }

    return factory->create_for_unmarshal();
}


} //namespace portable
} //namespace TIDorb
