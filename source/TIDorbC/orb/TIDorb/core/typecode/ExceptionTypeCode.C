/////////////////////////////////////////////////////////////////////////
//
// File:        ExceptionTypeCode.C
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




TIDorb::core::typecode::ExceptionTypeCode::ExceptionTypeCode ()
  throw (TIDThr::SystemException)
  : StructTypeCode()
{
  m_kind = CORBA::tk_except;
}




TIDorb::core::typecode::ExceptionTypeCode::ExceptionTypeCode
  (const char* id, const char* name, CORBA::StructMemberSeq* members)
  throw (TIDThr::SystemException)
  : StructTypeCode(id, name, members)
{
  m_kind = CORBA::tk_except;
}




bool TIDorb::core::typecode::ExceptionTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  // skip the id
  input.skip_string();
  // skip the members
  return StructTypeCode::skip_value(input);
}




void TIDorb::core::typecode::ExceptionTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  char* str;
  // exception id
  input.read_string(str);
  output.write_string(str);
  CORBA::string_free(str);
  StructTypeCode::remarshal_value(input,output);
}




bool TIDorb::core::typecode::ExceptionTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const
{
  bool result = false;

  char* str_a;
  char* str_b;

  a_input.read_string(str_a);
  b_input.read_string(str_b);

  if(!strcmp(str_a,str_b))
    result = StructTypeCode::values_equal(a_input,b_input);
  else
    result = false;

  CORBA::string_free(str_a);
  CORBA::string_free(str_b);

  return result;
}




void TIDorb::core::typecode::ExceptionTypeCode::dump (ostream& output) const
{
  CORBA::ULong length= m_members->length();

  output << "[TYPECODE]{exception ("  << length << " members) ";
  ComplexTypeCode::dump_params(output);
  output << " {";

  TypeCodeImpl* tc =0;

  for(CORBA::ULong i = 0; i < length; i++) {
    //tc = dynamic_cast<TypeCodeImpl *>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
    //jagd 
    //tc = (TypeCodeImpl*)((CORBA::TypeCode*)(*m_members)[i].type)->_impl();
    tc = (TypeCodeImpl*)((CORBA::TypeCode*)(*m_members)[i].type);
    output << ((*m_members)[i]).name << ": ";
    tc->dump(output);
  }

  output << '}';
}




bool TIDorb::core::typecode::ExceptionTypeCode::dump_value
       (TIDorb::core::cdr::CDRInputStream& input,
        ostream& output) const
{
  CORBA::ULong length = m_members->length();
  //PRA
  //char* id;
  CORBA::String_var id;
  //EPRA

  input.read_string(id.out());

  output << "[VALUE]{struct " << m_name << " : ";
        output << "(exception id) " << id << " - ";

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

  output << " END_EXCEPTION-}";

  return true;
}
