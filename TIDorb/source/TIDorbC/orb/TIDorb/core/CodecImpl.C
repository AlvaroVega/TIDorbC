/////////////////////////////////////////////////////////////////////////
//
// File:        CodecImpl.C
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
////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"
#include "TIDorb/core/cdr.h"

TIDorb::core::CodecImpl::CodecImpl(TIDorb::core::TIDORB* orb, 
                                   const IOP::Encoding& enc)
  throw (TIDThr::SystemException) 
  : m_orb(orb), m_enc(enc)
{
  this->TIDThr::RefCounter::_add_ref();
}


CORBA::OctetSeq* TIDorb::core::CodecImpl::encode(const CORBA::Any& data)
  throw (IOP::Codec::InvalidTypeForEncoding)
{
  TIDorb::core::cdr::CDROutputStream out(m_orb);

  TIDorb::core::comm::iiop::Version version(m_enc.major_version, m_enc.minor_version);

  out.set_version(version);

  try {
    // write as an encapsulation
    out.write_boolean(out.get_byte_order());
    out.write_any(data);
  } catch (const CORBA::MARSHAL) {
    throw IOP::Codec::InvalidTypeForEncoding();
  }

  TIDorb::core::cdr::CDRInputStream input(m_orb, out.get_buffer());

  /* available() solo cuenta los bytes disponibles del chunk actual
  CORBA::ULong length = input.available(); */
  CORBA::ULong length = input.buffer_available();

  CORBA::Octet* value = new CORBA::Octet[length];

  input.read_octet_array(value, 0, length);

  return new CORBA::OctetSeq(length, length, value, true);
}


CORBA::Any* TIDorb::core::CodecImpl::decode(const ::CORBA::OctetSeq& data)
  throw (IOP::Codec::FormatMismatch)
{
  TIDorb::core::cdr::CDROutputStream out(m_orb);

  TIDorb::core::comm::iiop::Version version(m_enc.major_version, m_enc.minor_version);

  out.set_version(version);

  try {
    out.write_octet_array(data.get_buffer(), 0, data.length());
  } catch (const CORBA::MARSHAL) {
    throw IOP::Codec::FormatMismatch();
  }

  TIDorb::core::cdr::CDRInputStream input(m_orb, out.get_buffer());

  // read as an encapsulation

  CORBA::Boolean byte_order;
  input.read_boolean(byte_order);
  input.set_byte_order(byte_order);

  CORBA::Any* value = new CORBA::Any();

  try {
    input.read_any(*value);
  } catch (...) {
    delete value;
    throw IOP::Codec::FormatMismatch();
  }

  return value;
}


CORBA::OctetSeq* TIDorb::core::CodecImpl::encode_value(const CORBA::Any& data)
  throw (IOP::Codec::InvalidTypeForEncoding)
{
  TIDorb::core::cdr::CDROutputStream out(m_orb);

  TIDorb::core::comm::iiop::Version version(m_enc.major_version, m_enc.minor_version);

  out.set_version(version);

  try {
    out.write_boolean(out.get_byte_order());
    data.delegate().write_value(out);
  } catch (const CORBA::MARSHAL) {
    throw IOP::Codec::InvalidTypeForEncoding();
  }

  TIDorb::core::cdr::CDRInputStream input(m_orb, out.get_buffer());

  /* available() solo cuenta los bytes disponibles del chunk actual
  CORBA::ULong length = input.available(); */
  CORBA::ULong length = input.buffer_available();

  CORBA::Octet* value = new CORBA::Octet[length];

  input.read_octet_array(value, 0, length);

  return new CORBA::OctetSeq(length, length, value, true);
}


CORBA::Any* TIDorb::core::CodecImpl::decode_value(const ::CORBA::OctetSeq& data,
                                                  CORBA::TypeCode_ptr tc)
  throw (IOP::Codec::FormatMismatch, IOP::Codec::TypeMismatch)
{
  if(!(tc))
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);

  TIDorb::core::cdr::CDROutputStream out(m_orb);

  TIDorb::core::comm::iiop::Version version(m_enc.major_version, m_enc.minor_version);

  out.set_version(version);

  try {
    out.write_octet_array(data.get_buffer(), 0, data.length());
  } catch (const CORBA::MARSHAL) {
    throw IOP::Codec::InvalidTypeForEncoding();
  }

  TIDorb::core::cdr::CDRInputStream input(m_orb, out.get_buffer());

  CORBA::Boolean byte_order;
  input.read_boolean(byte_order);
  input.set_byte_order(byte_order);

  CORBA::Any* value = new CORBA::Any();

  try {
    value->delegate().read_value(input, tc);
  } catch (...) {
    delete value;
    throw IOP::Codec::FormatMismatch();
  }

  return value;
}
