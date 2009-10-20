/////////////////////////////////////////////////////////////////////////
//
// File:        StructTypeCode.C
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




TIDorb::core::typecode::StructTypeCode::StructTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_struct), m_members(0)
{
}




TIDorb::core::typecode::StructTypeCode::StructTypeCode(const char* id,
                                                       const char* name,
                                                       CORBA::StructMemberSeq* members)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_struct, id, name), m_members(members)
{
}




TIDorb::core::typecode::StructTypeCode::~StructTypeCode()
  throw (TIDThr::SystemException)
{
//MLG
  if (m_members)
        delete m_members;
//EMLG
}




CORBA::Boolean TIDorb::core::typecode::StructTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if (!ComplexTypeCode::equal(tc))
        return false;

  if (!m_exhaustive_equal)
        return true;
  try {

        CORBA::ULong length = m_members->length();
        if (length != tc->member_count())
                return false;


        for (CORBA::ULong i = 0; i < length; i++) {
                if (strcmp(member_name(i), tc->member_name(i)))
                        return false;
                if (! member_type(i)->equal(tc->member_type(i)))
                        return false;
        }
        // allright
        return true;
  } catch (const CORBA::TypeCode::BadKind& bk) {
        return false;
  } catch (const CORBA::TypeCode::Bounds& bn) {
        return false;
  }
}

CORBA::Boolean TIDorb::core::typecode::StructTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equivalent(tc))
        return false;

  if (!m_exhaustive_equal)
        return true;
  try {

        CORBA::ULong length = m_members->length();
        if (length != tc->member_count())
                return false;


        for (CORBA::ULong i = 0; i < length; i++) {
                if (strcmp(member_name(i), tc->member_name(i)))
                        return false;
                if (! member_type(i)->equivalent(tc->member_type(i)))
                        return false;
        }
        // allright
        return true;
  } catch (const CORBA::TypeCode::BadKind& bk) {
        return false;
  } catch (const CORBA::TypeCode::Bounds& bn) {
        return false;
  }
}



CORBA::ULong TIDorb::core::typecode::StructTypeCode::member_count() const
{
  return m_members->length();
}




const char* TIDorb::core::typecode::StructTypeCode::member_name(CORBA::ULong index) const
{
  if(index < m_members->length())
    return ((*m_members)[index]).name;
  else
    throw CORBA::TypeCode::Bounds();
}




CORBA::TypeCode_ptr TIDorb::core::typecode::StructTypeCode::member_type(CORBA::ULong index) const
{
  if(index < m_members->length())
    return CORBA::TypeCode::_duplicate(((*m_members)[index]).type);
  else
    throw CORBA::TypeCode::Bounds();
}




void TIDorb::core::typecode::StructTypeCode::write_params
      (TIDorb::core::cdr::CDROutputStream& output)
{
  ComplexTypeCode::write_params(output);

  CORBA::ULong length = m_members->length();

  output.write_ulong(length);

  for(CORBA::ULong i = 0; i < length; i++) {
    output.write_string(((*m_members)[i]).name);
    output.write_TypeCode(((*m_members)[i]).type);
  }
}




void TIDorb::core::typecode::StructTypeCode::read_params
       (TIDorb::core::cdr::CDRInputStream& input)
{
  ComplexTypeCode::read_params(input);
/*
//MLG
  m_members = new CORBA::StructMemberSeq();
//EMLG

  CORBA::_StructMemberSeqHelper::read(input, *m_members);
*/

  CORBA::ULong length;
  input.read_ulong(length);

//FRAN
  if(m_members){
    delete m_members;
  }
//EFRAN
 
  m_members = new CORBA::StructMemberSeq();

  m_members->length(length);

  for(CORBA::ULong i = 0; i < length; i++) {
//FRAN
  if(((*m_members)[i]).name){
    CORBA::string_free(((*m_members)[i]).name);
  }
//EFRAN
    input.read_string(((*m_members)[i]).name);
    input.read_TypeCode(((*m_members)[i]).type);
  }

}


void TIDorb::core::typecode::StructTypeCode::remarshal_value
       (TIDorb::core::cdr::CDRInputStream& input,
        TIDorb::core::cdr::CDROutputStream& output) const
{
  CORBA::ULong length = m_members->length();
  TypeCodeImpl* tc =0;
  for(CORBA::ULong i = 0; i < length; i++) {
    //tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    //jagd     
    //tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type)->_impl();
    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type);
    tc->remarshal_value(input, output);
  }
}




bool TIDorb::core::typecode::StructTypeCode::values_equal
       (TIDorb::core::cdr::CDRInputStream& a_input,
        TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::ULong length = m_members->length();
  TypeCodeImpl* tc =0;
  for(CORBA::ULong i = 0; i < length; i++) {
    //tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    //jagd
    //tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type)->_impl();
    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type);
    if(!tc->values_equal(a_input, b_input))
      return false;
  }

  return true;
}




void TIDorb::core::typecode::StructTypeCode::dump (ostream& output) const
{
  CORBA::ULong length= m_members->length();

  output << "[TYPECODE]{struct ("  << length << " members) ";
  ComplexTypeCode::dump_params(output);
  output << " {";

  TypeCodeImpl* tc =0;

  for(CORBA::ULong i = 0; i < length; i++) {
    //tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    //jagd
    //tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type)->_impl();
    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type);
    output << ((*m_members)[i]).name << ": ";
    tc->dump(output);
  }

  output << '}';
}




bool TIDorb::core::typecode::StructTypeCode::dump_value
       (TIDorb::core::cdr::CDRInputStream& input,
        ostream& output) const
{
  CORBA::ULong length = m_members->length();

  output << "[VALUE]{struct " << m_name << " : ";

  TypeCodeImpl* tc =0;

  for(CORBA::ULong i = 0; i < length; i++) {
    //tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    //jagd
    //tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type)->_impl();
    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type);

    output << '(' << i << ") " << ((*m_members)[i]).name << ": ";

    if(!tc->dump_value(input, output))
      return false;

    output << " | ";
  }

  output << " END_STRUCT-}";

  return true;
}
