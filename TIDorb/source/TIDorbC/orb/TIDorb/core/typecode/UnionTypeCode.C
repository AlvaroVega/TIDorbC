/////////////////////////////////////////////////////////////////////////
//
// File:        UnionTypeCode.C
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


UnionTypeCode::UnionTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_union), m_members(0), m_discriminator_type(0)
{
  m_default_used = -1;
}


UnionTypeCode::UnionTypeCode(const char* id,
                             const char* name,
                             CORBA::TypeCode_ptr discriminator_type,
                             CORBA::UnionMemberSeq* members)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_union, id, name), m_members(members)
{
  m_discriminator_type = CORBA::TypeCode::_duplicate(discriminator_type);

  // default member search

  CORBA::ULong length = members->length();

  m_default_used = -1; // default_used mecanism not explicited in CORBA 2.3

  CORBA::TypeCode_var label_type;
  CORBA::TypeCode_var tc_default =
    TIDorb::portable::TypeCodeFactory::get_basic_TypeCode(CORBA::tk_octet);

  for(CORBA::ULong i=0; i< length; i++ )
    {
      label_type = ((*members)[i]).label.type();
      if(tc_default->equal(label_type))
        {
          m_default_used = i;
          break;
        }
    }
}


UnionTypeCode::~UnionTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::release(m_discriminator_type);
  delete m_members;
}




CORBA::Boolean UnionTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if (!ComplexTypeCode::equal(tc))
    return false;

  if (!m_exhaustive_equal)
    return true;

  UnionTypeCode* other = dynamic_cast<UnionTypeCode*> (tc);

  if(!other)
    return false;

  CORBA::ULong length = m_members->length();

  if (length != other->m_members->length())
    return false;

  if (!m_discriminator_type->equal(other->m_discriminator_type))
    return false;

  CORBA::TypeCode_ptr my_member_type, others_member_type;
  CORBA::Any* my_member_label;
  CORBA::Any* others_member_label;

  for (CORBA::ULong index = 0; index < length; index++) {
    if (strcmp((const char*)(((*m_members)[index]).name),
               (const char*)(((*(other->m_members))[index]).name)))
      return false;

    my_member_label = &(((*m_members)[index]).label);
    others_member_label = &(((*(other->m_members))[index]).label);

    if (my_member_label->delegate() != others_member_label->delegate())
      return false;

    my_member_type = ((*m_members)[index]).type;
    others_member_type = ((*(other->m_members))[index]).type;

    if (!my_member_type->equal(others_member_type))
      return false;
  }
  // allright
  return true;

}

CORBA::Boolean UnionTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equivalent(tc))
    return false;

  if (!m_exhaustive_equal)
    return true;

  UnionTypeCode* other = dynamic_cast<UnionTypeCode*> (tc);

  if(!other)
    return false;

  CORBA::ULong length = m_members->length();

  if (length != other->m_members->length())
    return false;

  if (!m_discriminator_type->equivalent(other->m_discriminator_type))
    return false;

  CORBA::TypeCode_ptr my_member_type, others_member_type;
  CORBA::Any* my_member_label;
  CORBA::Any* others_member_label;

  for (CORBA::ULong index = 0; index < length; index++) {
    if (strcmp((const char*)(((*m_members)[index]).name),
               (const char*)(((*(other->m_members))[index]).name)))
      return false;

    my_member_label = &(((*m_members)[index]).label);
    others_member_label = &(((*(other->m_members))[index]).label);

    if (my_member_label->delegate() != others_member_label->delegate())
      return false;

    my_member_type = ((*m_members)[index]).type;
    others_member_type = ((*(other->m_members))[index]).type;

    if (!my_member_type->equivalent(others_member_type))
      return false;
  }
  // allright
  return true;
}


CORBA::ULong UnionTypeCode::member_count() const
{
  return m_members->length();
}




const char* UnionTypeCode::member_name(CORBA::ULong index) const
{
  if(index < m_members->length())
    return ((*m_members)[index]).name;
  else
    throw CORBA::TypeCode::Bounds();
}




CORBA::TypeCode_ptr UnionTypeCode::member_type(CORBA::ULong index) const
{
  if(index < m_members->length())
    return CORBA::TypeCode::_duplicate(((*m_members)[index]).type);
  else
    throw CORBA::TypeCode::Bounds();
}


CORBA::Any* UnionTypeCode::member_label(CORBA::ULong index) const
{
  if(index < m_members->length())
    return new CORBA::Any((((*m_members)[index]).label));
  else
    throw CORBA::TypeCode::Bounds();
}



CORBA::TypeCode_ptr UnionTypeCode::discriminator_type() const
{
  return CORBA::TypeCode::_duplicate(m_discriminator_type);
}


CORBA::Long UnionTypeCode::default_index() const
{
  return m_default_used;
}



void UnionTypeCode::write_params
(TIDorb::core::cdr::CDROutputStream& output)
{
  ComplexTypeCode::write_params(output);

  output.write_TypeCode(m_discriminator_type);
  output.write_long(default_index());

  CORBA::ULong length = m_members->length();

  output.write_ulong(length);

  for(CORBA::ULong i = 0; i < length; i++) {
  output.write_any(((*m_members)[i]).label);
  output.write_string(((*m_members)[i]).name);
  output.write_TypeCode(((*m_members)[i]).type);
  }

}




void UnionTypeCode::read_params
(TIDorb::core::cdr::CDRInputStream& input)
{
  ComplexTypeCode::read_params(input);

        if(m_discriminator_type){
          CORBA::release(m_discriminator_type);
        }

        input.read_TypeCode(m_discriminator_type);
        input.read_long(m_default_used);

        CORBA::ULong length;
        input.read_ulong(length);

        if(m_members){
          delete m_members;
        }

        m_members = new CORBA::UnionMemberSeq();
        m_members->length(length);

        for (CORBA::ULong i = 0; i < length; i++) {
            if (((*m_members)[i]).name)
               CORBA::string_free(((*m_members)[i]).name);
            input.read_any(((*m_members)[i]).label);
            input.read_string(((*m_members)[i]).name);
            input.read_TypeCode(((*m_members)[i]).type);
        }
}




bool UnionTypeCode::skip_value (TIDorb::core::cdr::CDRInputStream& input) const
{
  CORBA::Any discriminator;

  // jagd
  //discriminator.delegate().read_value(input, m_discriminator_type);
  discriminator.delegate().read_value(input, m_discriminator_type, true);

  CORBA::Long index = search_member_index(discriminator);

  //PRA
  if (index == -1) {
    return true;	// default constructed union
  }
  //EPRA

  if ((index < 0) || (index >= m_members->length())) {
    throw CORBA::MARSHAL();
  }
  //jagd
  //TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type)->_impl();
  TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type);

  tc->skip_value(input);

  return true;
}




void UnionTypeCode::remarshal_value
    (TIDorb::core::cdr::CDRInputStream& input,
     TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::Any discriminator;

  // jagd
  //discriminator.delegate().read_value(input, m_discriminator_type);
  discriminator.delegate().read_value(input, m_discriminator_type, true);
  discriminator.delegate().write_value(output);

  CORBA::Long index = search_member_index(discriminator);

  //PRA
  if (index == -1) {
    return;		// default constructed union
  }
  //EPRA

  if ((index < 0) || (index >= m_members->length())) {
    throw CORBA::MARSHAL();
  }

  //TypeCodeImpl* tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[index]).type);
  //jagd 
  //TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type)->_impl();
  TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type);

  tc->remarshal_value(input, output);
}




bool UnionTypeCode::values_equal 
    (TIDorb::core::cdr::CDRInputStream& a_input,
     TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::Any discriminator_a, discriminator_b;

  // jagd
  //discriminator_a.delegate().read_value(a_input, m_discriminator_type);
  //discriminator_b.delegate().read_value(b_input, m_discriminator_type);
  discriminator_a.delegate().read_value(a_input, m_discriminator_type, true);
  discriminator_b.delegate().read_value(b_input, m_discriminator_type, true);

  if(discriminator_a != discriminator_b)
    return false;

  CORBA::Long index = search_member_index(discriminator_a);
  //PRA
  if (index == -1) {
    return true;	// default constructed union
  }
  //EPRA

  if ((index < 0) || (index >= m_members->length())) {
    throw CORBA::MARSHAL();
  }

  //TypeCodeImpl* tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[index]).type);
  //jagd 
  //TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type)->_impl();
  TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type);

  return tc->values_equal(a_input, b_input);
}




void UnionTypeCode::dump (ostream& output) const
{

  CORBA::ULong length = m_members->length();

  output << "[TYPECODE]{union (" <<      length << " members) ";

  ComplexTypeCode::dump_params(output);

  output <<  " {";

  TypeCodeImpl* my_member_type;
  CORBA::Any* my_member_label;

  for (CORBA::ULong i = 0; i < length; i++) {
    output << " case ";
    my_member_label =&(((*m_members)[i]).label);
    my_member_label->delegate().dump(output);

    output<< ": " << member_name(i)  << " -> ";

    //my_member_type = dynamic_cast<TypeCodeImpl*>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    //jagd 
    //my_member_type = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type)->_impl();
    my_member_type = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type);

    my_member_type->dump(output);

    output << " | ";
  }

  output << '}';
}


bool UnionTypeCode::dump_value (TIDorb::core::cdr::CDRInputStream& input,
                                ostream& output) const
{
  CORBA::Any discriminator;
  // jagd
  //discriminator.delegate().read_value(input, m_discriminator_type);
  discriminator.delegate().read_value(input, m_discriminator_type, true);

  output << "[VALUE]{union " << m_name << " discriminator: ";

  discriminator.delegate().dump(output);

  CORBA::Long index = search_member_index(discriminator);
  //PRA
  if (index == -1) {
    return true;	// default constructed union
  }
  //EPRA

  if ((index < 0) || (index >= m_members->length())) {
    throw CORBA::MARSHAL();
  }

  //TypeCodeImpl* tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[index]).type);
  //jagd 
  //TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type)->_impl();
  TypeCodeImpl* tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[index].type);

  if(index == m_default_used)
    output << " /default/ ";

  output << ((*m_members)[index]).name << " -> " ;

  if (!tc->dump_value(input, output))
    return false;

  output << '}';

  return true;
}




CORBA::Long UnionTypeCode::search_member_index
    (const CORBA::Any& discriminator) const
{
  CORBA::ULong length = m_members->length();

  CORBA::Any* disc;

  for (CORBA::ULong i = 0; i < length; i++) {
    disc = &(((*m_members)[i]).label);
    if( discriminator.delegate() == disc->delegate())
      return i;
  }

  return m_default_used;

}

}//namespace TIDorb::core::typecode
}
}
