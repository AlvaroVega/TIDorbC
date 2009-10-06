//////////////////////////////////////////////////////////////////////////////////
//
// File:        ReplyStatusType.C
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

#include "TIDorb/core/comm/iiop/ReplyStatusType.h"
	
	
TIDorb::core::comm::iiop::ReplyStatusType TIDorb::core::comm::iiop::ReplyStatusType_from_int(CORBA::ULong type)
{
  if(type > TIDorb::core::comm::iiop::NEEDS_ADDRESSING_MODE)
		 throw CORBA::MARSHAL("Invalid ReplyStatusType");
		
		return (TIDorb::core::comm::iiop::ReplyStatusType) type;
};
	
const char* TIDorb::core::comm::iiop::toString(ReplyStatusType value)
{
	switch (value) {
		case TIDorb::core::comm::iiop::NO_EXCEPTION:
			return "ReplyStatusType: NO_EXCEPTION";
		case TIDorb::core::comm::iiop::USER_EXCEPTION:
			return "ReplyStatusType:USER_EXCEPTION";
		case TIDorb::core::comm::iiop::SYSTEM_EXCEPTION:
			return "ReplyStatusType:SYSTEM_EXCEPTION";
		case TIDorb::core::comm::iiop::LOCATION_FORWARD:
			return "ReplyStatusType:LOCATION_FORWARD";
		case TIDorb::core::comm::iiop::LOCATION_FORWARD_PERM:
			return "ReplyStatusType:LOCATION_FORWARD_PERM";
		case TIDorb::core::comm::iiop::NEEDS_ADDRESSING_MODE:
			return "ReplyStatusType:NEEDS_ADDRESSING_MODE";
		default: 
			return "ReplyStatusType:Unknowed";
	}
};

