/////////////////////////////////////////////////////////////////////////
//
// File:        Float.C
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
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <values.h>
#include <float.h>
#include <errno.h>




const CORBA::Float TIDorb::core::util::Float::MIN_VALUE = FLT_MIN;
const CORBA::Float TIDorb::core::util::Float::MAX_VALUE = FLT_MAX;




CORBA::Float TIDorb::core::util::Float::parseFloat(const string& str)
{
  char* ptr;
  double value;

  errno = 0;
  value = ::strtod(str.c_str(), &ptr);
  if ((value < MIN_VALUE) || (value > MAX_VALUE)) {
    errno = ERANGE;
  }

  return (CORBA::Float) value;
}
