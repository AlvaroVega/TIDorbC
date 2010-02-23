/////////////////////////////////////////////////////////////////////////
//
// File:        SequenceTypeCode.C
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




TIDorb::core::typecode::SequenceTypeCode::SequenceTypeCode()
  throw (TIDThr::SystemException)
  : ArrayTypeCode()
{
  m_kind = CORBA::tk_sequence;
}




TIDorb::core::typecode::SequenceTypeCode::SequenceTypeCode(CORBA::TypeCode_ptr element_type,
                                                           CORBA::ULong length)
  throw (TIDThr::SystemException)
  : ArrayTypeCode(element_type, length)
{
  m_kind = CORBA::tk_sequence;
}




TIDorb::core::typecode::SequenceTypeCode::~SequenceTypeCode()
  throw (TIDThr::SystemException)
{
}




bool TIDorb::core::typecode::SequenceTypeCode::skip_value(TIDorb::core::cdr::CDRInputStream& input) const
{
  CORBA::ULong length;
  input.read_ulong(length);

  if((m_length > 0) && (length > m_length))
    throw CORBA::MARSHAL();

  return TypeCodeMarshaler::skip_value_array(m_element_type, input, length);
}




void TIDorb::core::typecode::SequenceTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::ULong length;

  input.read_ulong(length);

  if((m_length > 0) && (length > m_length))
    throw CORBA::MARSHAL();

  output.write_long(length);

  //TypeCodeImpl* tc_impl = dynamic_cast<TypeCodeImpl*>(m_element_type);
  //jagd
  //TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type->_impl();
  TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type;

        for (CORBA::ULong i = 0; i < length; i++)
                tc_impl->remarshal_value(input, output);
}




bool TIDorb::core::typecode::SequenceTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::ULong length_a, length_b;
  a_input.read_ulong(length_a);
  b_input.read_ulong(length_b);

  if(length_a != length_b)
    return false;

  if(length_a > m_length)
    throw CORBA::MARSHAL();

  //TypeCodeImpl* tc_impl = dynamic_cast<TypeCodeImpl*>(m_element_type);
  //jagd
  //TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type->_impl();
  TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type;

        for (CORBA::ULong i = 0; i < length_a; i++) {
                if(!tc_impl->values_equal(a_input, b_input))
                  return false;
        }
        return true;
}




void TIDorb::core::typecode::SequenceTypeCode::dump (ostream& output) const
{
  //TypeCodeImpl* tc_impl = dynamic_cast<TypeCodeImpl*>(m_element_type);
  //jagd 
  //TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type->_impl();
  TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type;

  output << "[TYPECODE]{ sequence <";

  if(m_length)
    output <<  m_length << ", ";

  tc_impl->dump(output);
  output <<  ">}";
}




bool TIDorb::core::typecode::SequenceTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  CORBA::ULong length;

  input.read_ulong(length);

  if(length > m_length)
    throw CORBA::MARSHAL();

  //TypeCodeImpl* tc_impl = dynamic_cast<TypeCodeImpl*>(m_element_type);
  //jagd 
  //TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type->_impl();
  TypeCodeImpl* tc_impl = (TypeCodeImpl*)m_element_type;

  output << "[" << length << " VALUES]{";

  for (CORBA::ULong i = 0; i <length ; i++)
  {
    output << '(' << i  << ") ";

    if(!tc_impl->dump_value(input, output))
          return false;

    output << " | ";
  }

  output << " END_ARRAY-}";
  return true;
}
