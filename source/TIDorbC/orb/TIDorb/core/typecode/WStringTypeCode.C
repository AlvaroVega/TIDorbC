/////////////////////////////////////////////////////////////////////////
//
// File:        WStringTypeCode.C
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
#include <wchar.h>




TIDorb::core::typecode::WStringTypeCode::WStringTypeCode()
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_wstring), m_length(0)
{
}




TIDorb::core::typecode::WStringTypeCode::WStringTypeCode(CORBA::ULong length)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_wstring), m_length(length)
{
}




TIDorb::core::typecode::WStringTypeCode::~WStringTypeCode()  throw (TIDThr::SystemException)
{
}




CORBA::Boolean TIDorb::core::typecode::WStringTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
        if(!TypeCodeImpl::equal(tc))
                return false;
        try {
                return (m_length == tc->length());
        } catch (const CORBA::TypeCode::BadKind& e) {
                return false;
        }
}




CORBA::ULong TIDorb::core::typecode::WStringTypeCode::length() const
{
  return m_length;
}




bool TIDorb::core::typecode::WStringTypeCode::isSimple() const
{
  return false;
}




void TIDorb::core::typecode::WStringTypeCode::partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input)
{
         input.read_ulong(m_length);
}




void TIDorb::core::typecode::WStringTypeCode::marshal(TIDorb::core::cdr::CDROutputStream& output)
{
  output.write_long(m_kind);
  output.write_long(m_length);
}




void TIDorb::core::typecode::WStringTypeCode::remarshal_value
      (TIDorb::core::cdr::CDRInputStream& input,
       TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::WChar* str;
  input.read_wstring(str);
  output.write_wstring(str);

  CORBA::wstring_free(str);
}




bool
TIDorb::core::typecode::WStringTypeCode::skip_value(TIDorb::core::cdr::CDRInputStream& input) const
{
        input.skip_wstring();
        return true;
}




bool TIDorb::core::typecode::WStringTypeCode::values_equal
        (TIDorb::core::cdr::CDRInputStream& a_input,
         TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::WChar* str_a;
  CORBA::WChar* str_b;

  a_input.read_wstring(str_a);
  b_input.read_wstring(str_b);

  bool equal = !wcscmp(str_a, str_b);

  CORBA::wstring_free(str_a);
  CORBA::wstring_free(str_b);

  return equal;
}




void TIDorb::core::typecode::WStringTypeCode::dump (ostream& output) const
{
  output << "[TYPECODE]{wstring";

  if(m_length > 0)
    output << '<' << m_length << '>';

  output << '}';
}




bool TIDorb::core::typecode::WStringTypeCode::dump_value
        (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  CORBA::WChar* str;
  input.read_wstring(str);

  output << "[VALUE] {wstring: ";

  if(m_length > 0)
    output << '<' << m_length << '>';
  output << ": \"" << str << "\"}";

  CORBA::wstring_free(str);
  return true;
}
