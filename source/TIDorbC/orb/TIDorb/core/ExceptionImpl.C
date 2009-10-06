/////////////////////////////////////////////////////////////////////////
//
// File:        ExceptionImpl.C
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

#include "CORBA.h"


void operator<<=(::CORBA::Any& any, const ::CORBA::Exception& e)
{
    throw CORBA::NO_IMPLEMENT();
}

void operator<<=(::CORBA::Any& any, ::CORBA::Exception* e)
{
    throw CORBA::NO_IMPLEMENT();
}

ostream& operator<<(ostream& os, const ::CORBA::Exception* se)
{
  os << "CORBA::Exception " << se->_name();
  return os;
}

ostream& operator<<(ostream& os, const ::CORBA::Exception& se)
{
  os << "CORBA::Exception " << se._name();
  return os;
}
