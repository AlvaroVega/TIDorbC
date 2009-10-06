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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
