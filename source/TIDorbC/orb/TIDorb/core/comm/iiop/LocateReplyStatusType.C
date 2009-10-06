//////////////////////////////////////////////////////////////////////////////////
//
// File:        LocateReplyStatusType.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/LocateReplyStatusType.h"



	
TIDorb::core::comm::iiop::LocateReplyStatusType 
TIDorb::core::comm::iiop::LocateReplyStatusType_from_int(CORBA::ULong type)
{
  if(type > LOC_NEEDS_ADDRESSING_MODE)
    throw CORBA::MARSHAL("Invalid LocateReplyStatusType");
	
  return (TIDorb::core::comm::iiop::LocateReplyStatusType) type;
};
	
const char* TIDorb::core::comm::iiop::toString(LocateReplyStatusType value)
{
  switch (value) {
  case TIDorb::core::comm::iiop::UNKNOWN_OBJECT:
    return "LocateReplyStatusType: UNKNOWN_OBJECT";
  case TIDorb::core::comm::iiop::OBJECT_HERE:
    return "LocateReplyStatusType: OBJECT_HERE";
  case TIDorb::core::comm::iiop::OBJECT_FORWARD:
    return "LocateReplyStatusType: OBJECT_FORWARD";
  case TIDorb::core::comm::iiop::OBJECT_FORWARD_PERM:
    return "LocateReplyStatusType: OBJECT_FORWARD_PERM";
  case TIDorb::core::comm::iiop::LOC_SYSTEM_EXCEPTION:
    return "LocateReplyStatusType: LOC_SYSTEM_EXCEPTION";
  case TIDorb::core::comm::iiop::LOC_NEEDS_ADDRESSING_MODE:
    return "LocateReplyStatusType: LOC_NEEDS_ADDRESSING_MODE";
  default:
    return "LocateReplyStatusType: UNKNOWED";
  }
};
	
