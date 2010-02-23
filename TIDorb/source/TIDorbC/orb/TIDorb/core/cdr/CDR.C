/////////////////////////////////////////////////////////////////////////
//
// File:        CDR.C
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

#include "TIDorb/core/cdr/CDR.h"
#include <inttypes.h>


const CORBA::ULong TIDorb::core::cdr::CDR::MAX_ALIGNMENT = 16;

const CORBA::ULong TIDorb::core::cdr::CDR::OCTET_SIZE = 1;
const CORBA::ULong TIDorb::core::cdr::CDR::BOOLEAN_SIZE = 1;
const CORBA::ULong TIDorb::core::cdr::CDR::CHAR_SIZE = 1;

const CORBA::ULong TIDorb::core::cdr::CDR::WCHAR_SIZE = 2;
const CORBA::ULong TIDorb::core::cdr::CDR::SHORT_SIZE = 2;
const CORBA::ULong TIDorb::core::cdr::CDR::USHORT_SIZE = 2;

const CORBA::ULong TIDorb::core::cdr::CDR::LONG_SIZE = 4;
const CORBA::ULong TIDorb::core::cdr::CDR::ULONG_SIZE = 4;
const CORBA::ULong TIDorb::core::cdr::CDR::FLOAT_SIZE = 4;

const CORBA::ULong TIDorb::core::cdr::CDR::LONGLONG_SIZE = 8;
const CORBA::ULong TIDorb::core::cdr::CDR::ULONGLONG_SIZE = 8;
const CORBA::ULong TIDorb::core::cdr::CDR::DOUBLE_SIZE = 8;
const CORBA::ULong TIDorb::core::cdr::CDR::LONG_DOUBLE_SIZE = 16;

const CORBA::Boolean TIDorb::core::cdr::CDR::BIG_ENDIAN = false;
const CORBA::Boolean TIDorb::core::cdr::CDR::LITTLE_ENDIAN = true;

const CORBA::Boolean TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER = 
  TIDorb::core::cdr::CDR::local_byte_order();

CORBA::Boolean TIDorb::core::cdr::CDR::local_byte_order()
{
  int val = 1;
  char* ptr = (char *) &val;
  return ((ptr[0])? LITTLE_ENDIAN : BIG_ENDIAN);
}

TIDorb::core::cdr::CDR::Addr
 TIDorb::core::cdr::CDR::align(TIDorb::core::cdr::CDR::Addr buffer)
{
  uintptr_t temp= (uintptr_t)buffer;
  temp+= MAX_ALIGNMENT - 1;
  temp&= ~((uintptr_t) MAX_ALIGNMENT - 1);
  return (Addr)temp;
}
