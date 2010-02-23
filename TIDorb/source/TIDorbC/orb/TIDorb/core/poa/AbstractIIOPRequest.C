/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractIIOPRequest.C
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

#include "TIDorb/core/poa/AbstractIIOPRequest.h"


TIDorb::core::poa::AbstractIIOPRequest::AbstractIIOPRequest(TIDorb::core::poa::POAKey* poakey,
                                                            TIDorb::core::poa::POAImpl* root_poa,
                                                            TIDorb::core::comm::Connection* conn)
  : TIDorb::core::poa::QueuedRequest(poakey, root_poa),
    connection(conn)
{
}

TIDorb::core::poa::AbstractIIOPRequest::~AbstractIIOPRequest()
{
}

TIDorb::core::comm::Connection* TIDorb::core::poa::AbstractIIOPRequest::get_connection() const
{
  return connection;
}


// void* TIDorb::core::poa::AbstractIIOPRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::poa::AbstractIIOPRequest::_typeid()
// {
// 	//return CORBA::string_dup("AbstractIIOPRequest");
// 	return "AbstractIIOPRequest";
// }

