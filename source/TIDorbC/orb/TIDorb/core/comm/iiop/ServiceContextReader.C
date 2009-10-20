//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServiceContextReader.C
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

#include "TIDorb/core/comm/iiop/ServiceContextReader.h"	
	
TIDorb::core::comm::iiop::ServiceContext* TIDorb::core::comm::iiop::ServiceContextReader::read(
	TIDorb::core::cdr::CDRInputStream& input)
{
	//int id = input.read_ulong();
	CORBA::ULong id;
	input.read_ulong(id);
	TIDorb::core::comm::iiop::ServiceContext* serv_ctx = NULL; 
	switch(id) {
		// Standart CORBA Service Contexts
		//case IOP::BI_DIR_IIOP.value:
		case BI_DIR_IIOP:
			serv_ctx = new TIDorb::core::comm::iiop::BiDirServiceContext();
			serv_ctx->partial_read(input);
			return serv_ctx;
                case IOP::INVOCATION_POLICIES:
                        serv_ctx = new TIDorb::core::comm::iiop::InvocationPoliciesContext();
                       	serv_ctx->partial_read(input);
                        return serv_ctx;
      		default:
        		serv_ctx = new TIDorb::core::comm::iiop::ServiceContext(id);
        		serv_ctx->partial_read(input);
        		return serv_ctx; 
	}
	
};

