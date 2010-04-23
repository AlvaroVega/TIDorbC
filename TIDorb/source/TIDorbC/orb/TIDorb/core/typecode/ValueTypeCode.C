/////////////////////////////////////////////////////////////////////////
//
// File:        ValueTypeCode.C
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




namespace TIDorb {
namespace core {
namespace typecode {




ValueTypeCode::ValueTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_value),
    m_value_modifier(CORBA::VM_NONE),
    m_concrete_base(0),
    m_members(0) //will be initialized in read_params()
{
}




ValueTypeCode::ValueTypeCode(const char* id,
                             const char* name,
                             CORBA::ValueModifier value_modifier,
                             CORBA::TypeCode_ptr concrete_base,
                             CORBA::ValueMemberSeq* members)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_value, id, name),
    m_value_modifier(value_modifier),
    m_members(members)
{
  m_concrete_base = CORBA::TypeCode::_duplicate(concrete_base); // may be null
}




ValueTypeCode::~ValueTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::release(m_concrete_base);
  delete m_members;
}




CORBA::Boolean ValueTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if (!ComplexTypeCode::equal(tc))
    return false;

  if (!m_exhaustive_equal)
    return true;

  ValueTypeCode* value_tc = dynamic_cast<ValueTypeCode*>(tc);
  if (value_tc == NULL)
    return false;

  if (m_value_modifier != value_tc->m_value_modifier)
    return false;

  if (m_concrete_base == NULL)
    return (value_tc->m_concrete_base == NULL);

  if (!m_concrete_base->equal(value_tc->m_concrete_base))
    return false;

  CORBA::ULong length = m_members->length();
  if (length != tc->member_count())
    return false;

  for (CORBA::ULong index = 0; index < length; index++) {
    CORBA::ValueMember& member    = (*m_members)[index];
    CORBA::ValueMember& tc_member = (*(value_tc->m_members))[index];

    if (strcmp((const char*) member.name, (const char*) tc_member.name))
      return false;
    if (member.access != tc_member.access)
      return false;
    if (! member.type->equal(tc_member.type))
      return false;
  }

  // allright
  return true;
}




CORBA::Boolean ValueTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equivalent(tc))
    return false;

  if (!m_exhaustive_equal)
    return true;

  ValueTypeCode* value_tc = dynamic_cast<ValueTypeCode*>(tc);
  if (value_tc == NULL)
    return false;

  if (m_value_modifier != value_tc->m_value_modifier)
    return false;

  if (m_concrete_base == NULL)
    return (value_tc->m_concrete_base == NULL);

  if (!m_concrete_base->equal(value_tc->m_concrete_base))
    return false;

  CORBA::ULong length = m_members->length();
  if (length != tc->member_count())
    return false;

  for (CORBA::ULong index = 0; index < length; index++) {
    CORBA::ValueMember& member    = (*m_members)[index];
    CORBA::ValueMember& tc_member = (*(value_tc->m_members))[index];

    if (strcmp((const char*) member.name, (const char*) tc_member.name))
      return false;
    if (member.access != tc_member.access)
      return false;
    if (! member.type->equal(tc_member.type))
      return false;
  }

  // allright
  return true;
}




CORBA::ValueModifier ValueTypeCode::type_modifier() const
{
  return m_value_modifier;
}




CORBA::TypeCode_ptr ValueTypeCode::concrete_base_type() const
{
  return CORBA::TypeCode::_duplicate(m_concrete_base);
}




CORBA::ULong ValueTypeCode::member_count() const
{
  return m_members->length();
}




const char* ValueTypeCode::member_name(CORBA::ULong index) const
{
  if (index < m_members->length())
    return ((*m_members)[index]).name;
  else
    throw CORBA::TypeCode::Bounds();
}




CORBA::TypeCode_ptr ValueTypeCode::member_type(CORBA::ULong index) const
{
  if (index < m_members->length())
    return CORBA::TypeCode::_duplicate(((*m_members)[index]).type);
  else
    throw CORBA::TypeCode::Bounds();
}




CORBA::Visibility ValueTypeCode::member_visibility(CORBA::ULong index) const
{
  if (index < m_members->length())
    return ((*m_members)[index]).access;
  else
    throw CORBA::TypeCode::Bounds();
}




void ValueTypeCode::write_params
  (TIDorb::core::cdr::CDROutputStream& output)
{
  ComplexTypeCode::write_params(output);

  output.write_short(m_value_modifier);

  CORBA::TypeCode_ptr tc = (m_concrete_base) ? m_concrete_base : CORBA::_tc_null;
  output.write_TypeCode(tc);

  CORBA::ULong length = m_members->length();
  output.write_ulong(length);

  for (CORBA::ULong i = 0; i < length; i++) {
    CORBA::ValueMember& member = (*m_members)[i];

    output.write_string(member.name);
    output.write_TypeCode(member.type);
    output.write_short(member.access);

  }
}




void ValueTypeCode::read_params
  (TIDorb::core::cdr::CDRInputStream& input)
{
  ComplexTypeCode::read_params(input);

  input.read_short(m_value_modifier);

  CORBA::release(m_concrete_base);
  input.read_TypeCode(m_concrete_base);
  if (m_concrete_base->equal(CORBA::_tc_null)) {
    CORBA::release(m_concrete_base);
    m_concrete_base = 0;
  }
  
  CORBA::ULong length;
  input.read_ulong(length);

  if (m_members) {
    delete m_members;
  }
 
  m_members = new CORBA::ValueMemberSeq();

  m_members->length(length);
  for (CORBA::ULong i = 0; i < length; i++) {
    CORBA::ValueMember& member = (*m_members)[i];

    CORBA::_IdentifierHelper::read(input, member.name);
    CORBA::_TypeCodeHelper::read(input, member.type);
    CORBA::_VisibilityHelper::read(input, member.access);
  }
}




bool ValueTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  // TODO: cambiar el prototipo de CDRInputStream::skip_Value para admitir const
  CORBA::TypeCode_ptr tc = (CORBA::TypeCode_ptr) this;
  input.skip_Value(tc);
  return true; 
}




bool ValueTypeCode::partial_skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  CORBA::ULong length = member_count();

  TypeCodeImpl* tc = 0;
  for (CORBA::ULong i = 0; i < length; i++) {
    tc = NULL;
    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    if (!tc || !tc->skip_value(input))
      return false;
  }

  return true;
}




void ValueTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::ValueBase_var value;
  value = input.read_Value((const CORBA::TypeCode_ptr) this);
  output.write_Value(value.in());
}




void ValueTypeCode::partial_remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::ULong length = member_count();

  TypeCodeImpl* tc =0;
  for (CORBA::ULong i = 0; i < length; i++) {
    tc = NULL;
    tc =(TypeCodeImpl*)((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    if (tc)
	    tc->remarshal_value(input, output);
  }
}




bool ValueTypeCode::values_equal
 (TIDorb::core::cdr::CDRInputStream& a_input,
  TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::ULong length = member_count();

  TypeCodeImpl* tc =0;
  for (CORBA::ULong i = 0; i < length; i++) {
    tc = NULL;
    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    if (!tc || !tc->values_equal(a_input, b_input))
      return false;
  }

  return true;
}




void ValueTypeCode::dump(ostream& output) const
{
  TypeCodeImpl* tc;
  CORBA::ULong  length = member_count();

  output << "[TYPECODE]{value ("  << length << " members) ";
  ComplexTypeCode::dump_params(output);

  output << ", modifier= ";
  switch(m_value_modifier) {
    case CORBA::VM_NONE:
      output << "VM_NONE";
      break;
    case CORBA::VM_CUSTOM:
      output << "VM_CUSTOM";
      break;
    case CORBA::VM_ABSTRACT:
      output << "VM_ABSTRACT";
      break;
    case CORBA::VM_TRUNCATABLE:
      output << "VM_TRUNCATABLE";
      break;
  }

  output << ", concrete base= ";
  tc = NULL;
  tc = (TypeCodeImpl*)(m_concrete_base);
  if (tc)
    tc->dump(output);
  else
    output << "nil";

  output << " {";
  for (CORBA::ULong i = 0; i < length; i++) {
    CORBA::ValueMember& member = (*m_members)[i];
    tc = NULL;
    tc = (TypeCodeImpl*) ((CORBA::TypeCode_ptr) member.type);
    output << member.name << ": ";
    if (tc)
       tc->dump(output);
  }

  output << '}';
}




bool ValueTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input,
   ostream& output) const
{
  TypeCodeImpl* tc;

  CORBA::ULong length = member_count();
  output << "[VALUE]{value " << m_name << " : ";

  for (CORBA::ULong i = 0; i < length; i++) {
    CORBA::ValueMember& member = (*m_members)[i];
    tc = NULL;
    tc = (TypeCodeImpl*) ((CORBA::TypeCode_ptr) member.type);
    output << '(' << i << ") " << member.name << ": ";
    if (!tc || !tc->dump_value(input, output))
      return false;

    output << " | ";
  }

  output << " END_VALUE-}";

  return true;
}




} //namespace typecode
} //namespace core
} //namespace TIDorb
