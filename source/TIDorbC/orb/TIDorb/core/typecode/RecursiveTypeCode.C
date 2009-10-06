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
