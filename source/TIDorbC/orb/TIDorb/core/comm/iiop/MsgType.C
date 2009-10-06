//////////////////////////////////////////////////////////////////////////////////
//
// File:        MsgType.C
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

#include "TIDorb/core/comm/iiop/MsgType.h"



TIDorb::core::comm::iiop::MsgType TIDorb::core::comm::iiop::MsgType_from_int(CORBA::ULong type)
{
	if(type > Fragment)
	  throw CORBA::MARSHAL("Invalid MsgType value");
	
	return (MsgType) type;
};
	
const char* TIDorb::core::comm::iiop::toString(TIDorb::core::comm::iiop::MsgType type)
{
	switch (type) {
		case TIDorb::core::comm::iiop::Request:
			return "Request";
		case TIDorb::core::comm::iiop::Reply:
			return "Reply";
		case TIDorb::core::comm::iiop::CancelRequest:
			return "CancelRequest";
		case TIDorb::core::comm::iiop::LocateRequest:
			return "LocateRequest";
		case TIDorb::core::comm::iiop::LocateReply:
			return "LocateReply";
		case TIDorb::core::comm::iiop::CloseConnection:
			return "CloseConnection";
		case TIDorb::core::comm::iiop::MessageError:
			return "MessageError";
		case TIDorb::core::comm::iiop::Fragment:
			return "Fragment";
		default:
			return "Unknow";
	}
};




