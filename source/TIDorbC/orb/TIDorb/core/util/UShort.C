/////////////////////////////////////////////////////////////////////////
//
// File:        UShort.C
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




const CORBA::UShort TIDorb::core::util::UShort::MAX_VALUE = UINT16_MAX;




CORBA::UShort TIDorb::core::util::UShort::parseUShort(const string& str)
{
  char* ptr;
  unsigned long long value;

  errno = 0;
  value = ::strtoull(str.c_str(), &ptr, 10);
  if (value > MAX_VALUE) {
    errno = ERANGE;
  }

  return (CORBA::UShort) value;
}
