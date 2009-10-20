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




