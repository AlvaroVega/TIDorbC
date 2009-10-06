/////////////////////////////////////////////////////////////////////////
//
// File:        ValueBaseImpl.C
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

#include "TIDorb/core.h"



void CORBA::ValueBase::_read(TIDorb::portable::InputStream& input)
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}


void CORBA::ValueBase::_write(TIDorb::portable::OutputStream& out) const
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}
