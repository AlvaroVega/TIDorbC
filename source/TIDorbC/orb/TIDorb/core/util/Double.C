/////////////////////////////////////////////////////////////////////////
//
// File:        Double.C
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




const CORBA::Double TIDorb::core::util::Double::MIN_VALUE = DBL_MIN;
const CORBA::Double TIDorb::core::util::Double::MAX_VALUE = DBL_MAX;




CORBA::Double TIDorb::core::util::Double::parseDouble(const string& str)
{
  char* ptr;
  double value;

  errno = 0;
  value = ::strtod(str.c_str(), &ptr);
  if ((value < MIN_VALUE) || (value > MAX_VALUE)) {
    errno = ERANGE;
  }

  return (CORBA::Double) value;
}
