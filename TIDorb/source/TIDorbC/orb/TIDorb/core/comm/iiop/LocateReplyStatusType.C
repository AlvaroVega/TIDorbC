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
	
