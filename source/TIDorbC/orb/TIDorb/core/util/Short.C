/////////////////////////////////////////////////////////////////////////
//
// File:        Short.C
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
#include <errno.h>




const CORBA::Short TIDorb::core::util::Short::MIN_VALUE = INT16_MIN;
const CORBA::Short TIDorb::core::util::Short::MAX_VALUE = INT16_MAX;




CORBA::Short TIDorb::core::util::Short::parseShort(const string& str)
{
  char* ptr;
  long long value;

  errno = 0;
  value = ::strtoll(str.c_str(), &ptr, 10);
  if ((value < MIN_VALUE) || (value > MAX_VALUE)) {
    errno = ERANGE;
  }

  return (CORBA::Short) value;
}
