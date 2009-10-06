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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
