/////////////////////////////////////////////////////////////////////////
//
// File:        EnumTypeCode.C
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




TIDorb::core::typecode::EnumTypeCode::EnumTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_enum), m_members(0)
{
}




//PRA
//TIDorb::core::typecode::EnumTypeCode::EnumTypeCode(const char* id,
//                                                   const char* name,
//                                                   const CORBA::EnumMemberSeq* members)
TIDorb::core::typecode::EnumTypeCode::EnumTypeCode(const char* id,
                                                   const char* name,
                                                   CORBA::EnumMemberSeq* members)
//EPRA
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_enum, id, name)
{
//PRA
//m_members = new CORBA::EnumMemberSeq(*members);
  m_members = members;
//EPRA
}




TIDorb::core::typecode::EnumTypeCode::~EnumTypeCode()
  throw (TIDThr::SystemException)
{
  delete m_members;
}




CORBA::Boolean TIDorb::core::typecode::EnumTypeCode::equal(CORBA::TypeCode_ptr tc) const
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
        }
        // alright
        return true;
  } catch (const CORBA::TypeCode::BadKind& bk) {
        return false;
  } catch (const CORBA::TypeCode::Bounds& bn) {
        return false;
  }
}

CORBA::Boolean TIDorb::core::typecode::EnumTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
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
        }
        // alright
        return true;
  } catch (const CORBA::TypeCode::BadKind& bk) {
        return false;
  } catch (const CORBA::TypeCode::Bounds& bn) {
        return false;
  }
}


CORBA::ULong TIDorb::core::typecode::EnumTypeCode::member_count() const
{
  return m_members->length();
}




const char* TIDorb::core::typecode::EnumTypeCode::member_name(CORBA::ULong index) const
{
  if(index < m_members->length())
    return (*m_members)[index];
  else
    throw CORBA::TypeCode::Bounds();
}




void TIDorb::core::typecode::EnumTypeCode::write_params
      (TIDorb::core::cdr::CDROutputStream& output)
{
  ComplexTypeCode::write_params(output);

  // write sequence

  CORBA::ULong length = m_members->length();

  output.write_ulong(length);

  for(CORBA::ULong i = 0; i < length; i++)
    output.write_string((*m_members)[i]);

}




void TIDorb::core::typecode::EnumTypeCode::read_params
       (TIDorb::core::cdr::CDRInputStream& input)
{
  ComplexTypeCode::read_params(input);

  // CORBA::__StructMemberSeqHelper::read(input, *m_members);

  CORBA::ULong length;
  input.read_ulong(length);

  m_members = new CORBA::EnumMemberSeq();

  m_members->length(length);

//FRAN
//  TypeCodeImpl* tc =0;
//EFRAN
  for (CORBA::ULong i = 0; i < length; i++) {
//FRAN
    if ((*m_members)[i]) {
      CORBA::string_free((*m_members)[i]);
    }
//EFRAN
    input.read_string((*m_members)[i]);
  }
}




bool TIDorb::core::typecode::EnumTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
//MLG
  input.skip_ulong();
  return true;
//EMLG

/*
  CORBA::ULong length = m_members->length();
  input.skip_string_array(length);
  return true;
*/
}




void TIDorb::core::typecode::EnumTypeCode::remarshal_value
       (TIDorb::core::cdr::CDRInputStream& input,
        TIDorb::core::cdr::CDROutputStream& output) const
{
//MLG
        unsigned int value = 0;

        input.read_ulong(value);

        if (value >= m_members->length())
           throw CORBA::MARSHAL();


        output.write_ulong(value);
//EMLG
/*
  CORBA::ULong length = m_members->length();
  char* str;
  for(CORBA::ULong i = 0; i < length; i++) {
    input.read_string(str);
    output.write_string(str);
    CORBA::string_free(str);
  }
*/
}



bool TIDorb::core::typecode::EnumTypeCode::values_equal
       (TIDorb::core::cdr::CDRInputStream& a_input,
        TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::ULong a_value, b_value;

  a_input.read_ulong(a_value);
  b_input.read_ulong(b_value);

/*
  CORBA::ULong length = m_members->length();
  if (a_value >= length)
    throw CORBA::MARSHAL("Enum discriminator value out of range.");
*/

  return (a_value == b_value);
}  



/* PRA

bool TIDorb::core::typecode::EnumTypeCode::values_equal
       (TIDorb::core::cdr::CDRInputStream& a_input,
        TIDorb::core::cdr::CDRInputStream& b_input) const
{
  CORBA::ULong length = m_members->length();

  char* str_a;
  char* str_b;

  for(CORBA::ULong i = 0; i < length; i++) {
    a_input.read_string(str_a);
    b_input.read_string(str_b);

    //PRA
    //if(strcmp(str_a, str_b))
    //  return false;
    bool equal = !strcmp(str_a, str_b);
    //EPRA

    CORBA::string_free(str_a);
    CORBA::string_free(str_b);

    //PRA
    if (!equal)
      return false; 
    //EPRA
  }

  return true;
}
*/



void TIDorb::core::typecode::EnumTypeCode::dump (ostream& output) const
{
  CORBA::ULong length= m_members->length();

  output << "[TYPECODE]{enum  ";
  ComplexTypeCode::dump_params(output);
  output << " {";

  for(CORBA::ULong i = 0; i < length; i++) {
    output << (const char *) ((*m_members)[i]) << ": ";
  }

  output << "} }";
}




bool TIDorb::core::typecode::EnumTypeCode::dump_value
       (TIDorb::core::cdr::CDRInputStream& input,
        ostream& output) const
{
  CORBA::ULong length = m_members->length();

  CORBA::ULong value;

  input.read_ulong(value);

  if (value >= length)
    throw CORBA::MARSHAL();

  output << "[VALUE]{enum " << m_name << " : " ;

  output << (const char *) ((*m_members)[value]) << " }";

  return true;
}
