/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractInterfaceTypeCode.h
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




TIDorb::core::typecode::AbstractInterfaceTypeCode::AbstractInterfaceTypeCode()
 throw (TIDThr::SystemException)
 : ComplexTypeCode(CORBA::tk_abstract_interface)
{
}




TIDorb::core::typecode::AbstractInterfaceTypeCode::AbstractInterfaceTypeCode
  (const char* id, const char* name)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_abstract_interface, id, name)
{
}




TIDorb::core::typecode::AbstractInterfaceTypeCode::~AbstractInterfaceTypeCode()
  throw (TIDThr::SystemException)
{
}




bool TIDorb::core::typecode::AbstractInterfaceTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
  throw (CORBA::SystemException)
{
  CORBA::AbstractBase_var abstract;
  input.read_Abstract(abstract.out());
  return true;
}




void TIDorb::core::typecode::AbstractInterfaceTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
  throw (CORBA::SystemException)
{
  CORBA::AbstractBase_var value;
  input.read_Abstract(value.out());
  output.write_Abstract((const CORBA::AbstractBase_ptr) value);
}




bool TIDorb::core::typecode::AbstractInterfaceTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const throw (CORBA::SystemException)
{
  throw CORBA::NO_IMPLEMENT();
}




void TIDorb::core::typecode::AbstractInterfaceTypeCode::dump (ostream& output) const
  throw (CORBA::SystemException)
{
  output << "[TYPECODE]{abstract interface:";
  ComplexTypeCode::dump_params(output);
  output << '}';
}




bool TIDorb::core::typecode::AbstractInterfaceTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input,
   ostream& output) const
  throw (CORBA::SystemException)
{
  throw CORBA::NO_IMPLEMENT();
}


