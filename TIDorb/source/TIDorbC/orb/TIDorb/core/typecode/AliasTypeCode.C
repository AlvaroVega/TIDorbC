/////////////////////////////////////////////////////////////////////////
//
// File:        AliasTypeCode.C
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




TIDorb::core::typecode::AliasTypeCode::AliasTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_alias)
{
  m_type = CORBA::TypeCode::_nil();
}




TIDorb::core::typecode::AliasTypeCode::AliasTypeCode(const char* id,
                                                     const char* name,
                                                     CORBA::TypeCode_ptr type)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_alias, id, name)
{
  m_type = CORBA::TypeCode::_duplicate(type);
}




TIDorb::core::typecode::AliasTypeCode::~AliasTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::release(m_type);
}




CORBA::Boolean TIDorb::core::typecode::AliasTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if(!ComplexTypeCode::equal(tc))
    return false;

  try {
    return m_type->equal(tc->content_type());
  } catch (const CORBA::TypeCode::BadKind& e) {
    return false;
  }
}




CORBA::Boolean TIDorb::core::typecode::AliasTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  return m_type->equivalent(tc);
}




CORBA::TypeCode_ptr TIDorb::core::typecode::AliasTypeCode::content_type() const
{
  return CORBA::TypeCode::_duplicate(m_type);
}




void TIDorb::core::typecode::AliasTypeCode::write_params
  (TIDorb::core::cdr::CDROutputStream& output)
{
  ComplexTypeCode::write_params(output);
  output.write_TypeCode(m_type);
}




void TIDorb::core::typecode::AliasTypeCode::read_params
  (TIDorb::core::cdr::CDRInputStream& input)
{
  ComplexTypeCode::read_params(input);

  if (m_type){
    CORBA::release(m_type);
  }

  input.read_TypeCode(m_type);
}

void TIDorb::core::typecode::AliasTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  TypeCodeImpl* tc = (TypeCodeImpl*)m_type;
  tc->remarshal_value(input, output);
}

bool TIDorb::core::typecode::AliasTypeCode::values_equal
      (TIDorb::core::cdr::CDRInputStream& a_input,
       TIDorb::core::cdr::CDRInputStream& b_input) const
{
  TypeCodeImpl* tc = (TypeCodeImpl*)m_type;
  return tc->values_equal(a_input, b_input);
}




void TIDorb::core::typecode::AliasTypeCode::dump (ostream& output) const
{
  TypeCodeImpl* tc = (TypeCodeImpl*)m_type;

  output << "[TYPECODE]{typedef: ";
  dump_params(output);
  output << " , type= ";
  tc->dump(output);
  output << '}';
}




bool TIDorb::core::typecode::AliasTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input,
   ostream& output) const
{
  TypeCodeImpl* tc = (TypeCodeImpl*)m_type;

  output << "[VALUE]{typedef " << m_name << ":";
  if(!tc->dump_value(input,output))
    return false;
  output << '}';
  return true;

}
