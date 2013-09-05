/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractLocalRequest.C
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

#include "TIDorb/core/poa/AbstractLocalRequest.h"


TIDorb::core::poa::AbstractLocalRequest::AbstractLocalRequest
  (TIDorb::core::poa::POAKey* poakey,
   TIDorb::core::poa::POAImpl* root_poa)
  : QueuedRequest(poakey, root_poa)
{
}

TIDorb::core::poa::AbstractLocalRequest::~AbstractLocalRequest()
  throw(TIDThr::SystemException)
{
}


void TIDorb::core::poa::AbstractLocalRequest::destroy()
{
  // Instead of "delete this", elimination of LocalRequests is controlled by
  // reference counting
  _remove_ref();
}


// void* TIDorb::core::poa::AbstractLocalRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::AbstractLocalRequest::_typeid()
// {
// 	//return CORBA::string_dup("AbstractLocalRequest");
// 	return "AbstractLocalRequest";
// }
