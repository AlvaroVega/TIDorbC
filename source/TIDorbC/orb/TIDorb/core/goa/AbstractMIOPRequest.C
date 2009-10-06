/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractMIOPRequest.C
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

#include "TIDorb/core/goa/AbstractMIOPRequest.h"

TIDorb::core::goa::AbstractMIOPRequest::AbstractMIOPRequest
  (TIDorb::core::poa::POAKey* poakey,
   TIDorb::core::poa::POAImpl* root_poa,
   TIDorb::core::comm::Connection* conn)
  : TIDorb::core::poa::QueuedRequest(poakey, root_poa),
    connection(conn)
{
}

TIDorb::core::goa::AbstractMIOPRequest::~AbstractMIOPRequest()
{
}

TIDorb::core::comm::Connection*
TIDorb::core::goa::AbstractMIOPRequest::get_connection() const
{
  return connection;
}

// void* TIDorb::core::goa::AbstractMIOPRequest::_impl()
// {
// 	return this;
// }

// const char* TIDorb::core::goa::AbstractMIOPRequest::_typeid()
// {
// 	//return CORBA::string_dup("AbstractMIOPRequest");
// 	return "AbstractMIOPRequest";
// }

