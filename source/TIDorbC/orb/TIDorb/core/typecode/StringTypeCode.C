/////////////////////////////////////////////////////////////////////////
//
// File:        StringTypeCode.C
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
#include <string.h>



TIDorb::core::typecode::StringTypeCode::StringTypeCode()
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_string), m_length(0)
{
}




TIDorb::core::typecode::StringTypeCode::StringTypeCode(CORBA::ULong length)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_string), m_length(length)
{
}




TIDorb::core::typecode::StringTypeCode::~StringTypeCode()
  throw (TIDThr::SystemException)
{
}




CORBA::Boolean TIDorb::core::typecode::StringTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
        if(!TypeCodeImpl::equal(tc))
                return false;
        try {
                return (m_length == tc->length());
        } catch (const CORBA::TypeCode::BadKind& e) {
                return false;
        }
}




CORBA::ULong TIDorb::core::typecode::StringTypeCode::length() const
{
  return m_length;
}




bool TIDorb::core::typecode::StringTypeCode::isSimple() const
{
  return false;
}




void TIDorb::core::typecode::StringTypeCode::partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input)
{
         input.read_ulong(m_length);
}




void TIDorb::core::typecode::StringTypeCode::marshal(TIDorb::core::cdr::CDROutputStream& output)
{
  output.write_long(m_kind);
  output.write_long(m_length);
}




void TIDorb::core::typecode::StringTypeCode::remarshal_value
      (TIDorb::core::cdr::CDRInputStream& input,
       TIDorb::core::cdr::CDROutputStream& output) const
{
  char * str;
  input.read_string(str);
  output.write_string(str);

  CORBA::string_free(str);
}




bool
TIDorb::core::typecode::StringTypeCode::skip_value(TIDorb::core::cdr::CDRInputStream& input) const
{
        input.skip_string();
        return true;
}




bool TIDorb::core::typecode::StringTypeCode::values_equal
        (TIDorb::core::cdr::CDRInputStream& a_input,
         TIDorb::core::cdr::CDRInputStream& b_input) const
{
  char * str_a;
  char * str_b;
  a_input.read_string(str_a);
  b_input.read_string(str_b);

  bool equal = !strcmp(str_a, str_b);

  CORBA::string_free(str_a);
  CORBA::string_free(str_b);

  return equal;
}




void TIDorb::core::typecode::StringTypeCode::dump (ostream& output) const
{
  output << "[TYPECODE]{string";

  if(m_length > 0)
    output << '<' << m_length << '>';

  output << '}';
}




bool TIDorb::core::typecode::StringTypeCode::dump_value
        (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  char* str;
  input.read_string(str);

  output << "[VALUE] {string";

  if(m_length > 0)
    output << '<' << m_length << '>';
  output << ": \"" << str << "\"}";

  CORBA::string_free(str);
  return true;
}
