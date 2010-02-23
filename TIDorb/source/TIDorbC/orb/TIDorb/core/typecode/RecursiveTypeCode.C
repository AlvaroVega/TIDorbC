/////////////////////////////////////////////////////////////////////////
//
// File:        RecursiveTypeCode.C
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
#include <string.h>



TIDorb::core::typecode::RecursiveTypeCode::RecursiveTypeCode()
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_null)
{
  m_id = CORBA::string_dup("");
}




TIDorb::core::typecode::RecursiveTypeCode::RecursiveTypeCode(const char* id)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(CORBA::tk_null)
{
  m_id = CORBA::string_dup(id);
}




TIDorb::core::typecode::RecursiveTypeCode::~RecursiveTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::string_free(m_id);
}




CORBA::Boolean 
TIDorb::core::typecode::RecursiveTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  TIDorb::core::typecode::RecursiveTypeCode* rtc = 
    dynamic_cast<TIDorb::core::typecode::RecursiveTypeCode*> (tc);

  if (rtc)
    return (strcmp(rtc->m_id, m_id) == 0);
  else
    return false;

}




const char* TIDorb::core::typecode::RecursiveTypeCode::id() const
{
  return m_id;
}




bool TIDorb::core::typecode::RecursiveTypeCode::isSimple() const
{
  return false;
}




void TIDorb::core::typecode::RecursiveTypeCode::partial_unmarshal(
                                          TIDorb::core::cdr::CDRInputStream& input)
{
  throw CORBA::NO_IMPLEMENT();
}

void TIDorb::core::typecode::RecursiveTypeCode::dump (ostream& output) const
{
  output << "[TYPECODE] { recursive ";

  output << '<' << m_id << '>';

  output << '}';
}




bool TIDorb::core::typecode::RecursiveTypeCode::dump_value
        (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  output << "[VALUE] { indirection to ";
  output << '<' << m_id << '>';
  output << '}';
  return true;
}
