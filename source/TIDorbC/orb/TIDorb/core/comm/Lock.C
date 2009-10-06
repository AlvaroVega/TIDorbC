//////////////////////////////////////////////////////////////////////////////////
//
// File:        Lock.C
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

#include "TIDorb/core/comm.h"

using TIDorb::core::comm::iiop::GIOPFragmentedMessage;

namespace TIDorb {
namespace core {
namespace comm {
    
Lock::Lock() throw (TIDThr::SystemException)
: _request_id(0), message(NULL)
{
}
Lock::~Lock() throw (TIDThr::SystemException)
{
    delete message;
}

	
void Lock::put_reply(GIOPFragmentedMessage* msg)
{
  //    TIDThr::Synchronized(*this);
    message = msg;
    set_completed();
}
	
GIOPFragmentedMessage* Lock::consume_reply()
{
    GIOPFragmentedMessage* tmp = message;
    message = NULL;
    return tmp;
}

void Lock::clear()
{
    delete message;
    message = NULL;
}

}
}
}
