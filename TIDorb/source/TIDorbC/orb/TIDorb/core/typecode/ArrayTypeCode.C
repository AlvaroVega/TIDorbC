/////////////////////////////////////////////////////////////////////////
//
// File:        ArrayTypeCode.C
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

#include "TIDorb/core/typecode.h"




TIDorb::core::typecode::ArrayTypeCode::ArrayTypeCode()
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_array), m_length(0)
{
  m_element_type = CORBA::TypeCode::_duplicate(CORBA::_tc_null);
  //m_element_type = TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(CORBA::tk_null);
}




TIDorb::core::typecode::ArrayTypeCode::ArrayTypeCode(CORBA::TypeCode_ptr element_type,
                                                     CORBA::ULong length)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_array), m_length(length)
{
  m_element_type = CORBA::TypeCode::_duplicate(element_type);
}




TIDorb::core::typecode::ArrayTypeCode::~ArrayTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::release(m_element_type);
}




CORBA::Boolean TIDorb::core::typecode::ArrayTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equal(tc))
    return false;

  if (!m_exhaustive_equal)
    return true;

  try {
    return ((m_length == tc->length()) &&
            m_element_type->equal(tc->content_type()));

  } catch (const CORBA::TypeCode::BadKind& bk) {
        return false;
  }
}




CORBA::Boolean TIDorb::core::typecode::ArrayTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equivalent(tc))
    return false;

  if (!m_exhaustive_equal)
    return true;

  try {
    return ((m_length == tc->length()) &&
            m_element_type->equal(tc->content_type()));

  } catch (const CORBA::TypeCode::BadKind& bk) {
        return false;
  }
}




CORBA::ULong TIDorb::core::typecode::ArrayTypeCode::length() const
{
  return m_length;
}




CORBA::TypeCode_ptr TIDorb::core::typecode::ArrayTypeCode::content_type() const
{
  return CORBA::TypeCode::_duplicate((CORBA::TypeCode_ptr) m_element_type);
}




bool TIDorb::core::typecode::ArrayTypeCode::isSimple() const
{
  return false;
}




void TIDorb::core::typecode::ArrayTypeCode::marshal(TIDorb::core::cdr::CDROutputStream& output)
{
  output.write_long(m_kind);

  TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
  encapsulation->write_TypeCode(m_element_type);
  encapsulation->write_ulong(m_length);
  output.write_buffer(*(encapsulation->get_buffer()));
  delete encapsulation;
}




void TIDorb::core::typecode::ArrayTypeCode::partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input)
{
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();

  if(m_element_type){
    CORBA::release(m_element_type);
  }

  encapsulation->read_TypeCode(m_element_type);
  encapsulation->read_ulong(m_length);

  delete encapsulation;
}




bool TIDorb::core::typecode::ArrayTypeCode::skip_value(TIDorb::core::cdr::CDRInputStream& input) const
{
  return TypeCodeMarshaler::skip_value_array(m_element_type, input, m_length);
}




void TIDorb::core::typecode::ArrayTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  TypeCodeImpl* tc_impl = 
    (TypeCodeImpl*)m_element_type;

  for (CORBA::ULong i = 0; i < m_length; i++)
    tc_impl->remarshal_value(input, output);
}




bool TIDorb::core::typecode::ArrayTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const
{
  TypeCodeImpl* tc_impl =
    (TypeCodeImpl*)m_element_type;

  for (CORBA::ULong i = 0; i < m_length; i++) {
    if (!tc_impl->values_equal(a_input, b_input))
      return false;
  }
  return true;
}




void TIDorb::core::typecode::ArrayTypeCode::dump (ostream& output) const
{
  TypeCodeImpl* tc_impl = 
    (TypeCodeImpl*)m_element_type;
  
  output << "[TYPECODE]{";
  tc_impl->dump(output);
  output << '[' <<  m_length  << "]}";
}




bool TIDorb::core::typecode::ArrayTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input,
   ostream& output) const
{
  TypeCodeImpl* tc_impl =
    (TypeCodeImpl*)m_element_type;  

  output << "[VALUE]{";

  for (CORBA::ULong i = 0; i < m_length ; i++) {
    output << '(' << i  << ") ";
    if (!tc_impl->dump_value(input, output))
      return false;

    output << " | ";
  }

  output << " END_ARRAY-}";
  return true;
}
