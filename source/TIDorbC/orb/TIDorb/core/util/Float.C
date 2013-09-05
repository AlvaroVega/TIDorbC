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
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
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
