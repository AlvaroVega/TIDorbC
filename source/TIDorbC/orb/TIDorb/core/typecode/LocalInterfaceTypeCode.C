/////////////////////////////////////////////////////////////////////////
//
// File:        LocalInterfaceTypeCode.C
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




TIDorb::core::typecode::LocalInterfaceTypeCode::LocalInterfaceTypeCode()
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_local_interface)
{
}




TIDorb::core::typecode::LocalInterfaceTypeCode::LocalInterfaceTypeCode
  (const char* id, const char* name)
  throw (TIDThr::SystemException)
  : ComplexTypeCode(CORBA::tk_local_interface, id, name)
{
}




TIDorb::core::typecode::LocalInterfaceTypeCode::~LocalInterfaceTypeCode()
  throw (TIDThr::SystemException)
{
}




bool TIDorb::core::typecode::LocalInterfaceTypeCode::skip_value
  (TIDorb::core::cdr::CDRInputStream& input) const
{
  throw CORBA::NO_IMPLEMENT();
}




void TIDorb::core::typecode::LocalInterfaceTypeCode::remarshal_value
  (TIDorb::core::cdr::CDRInputStream& input,
   TIDorb::core::cdr::CDROutputStream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}




bool TIDorb::core::typecode::LocalInterfaceTypeCode::values_equal
  (TIDorb::core::cdr::CDRInputStream& a_input,
   TIDorb::core::cdr::CDRInputStream& b_input) const
{
  throw CORBA::NO_IMPLEMENT();
}




void TIDorb::core::typecode::LocalInterfaceTypeCode::dump (ostream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}




bool TIDorb::core::typecode::LocalInterfaceTypeCode::dump_value
  (TIDorb::core::cdr::CDRInputStream& input, ostream& output) const
{
  throw CORBA::NO_IMPLEMENT();
}
