/////////////////////////////////////////////////////////////////////////
//
// File:        CORBAException.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/util.h"

#include <sstream>

const char* TIDorb::core::util::CORBAException::what() const throw()
{
  if(mesg.size()  == 0)
  {
    ostringstream out;
    out << _ex;
    ((CORBAException*)this)->mesg = out.str().data();
  }

  return mesg.data();
}




