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
