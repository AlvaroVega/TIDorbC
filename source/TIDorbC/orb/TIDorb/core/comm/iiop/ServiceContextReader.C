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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

