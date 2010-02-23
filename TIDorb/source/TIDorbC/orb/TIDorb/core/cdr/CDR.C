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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
