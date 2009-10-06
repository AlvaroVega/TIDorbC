/////////////////////////////////////////////////////////////////////////
//
// File:        FragmentedMessageHolder.C
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

#include "TIDorb/core/comm/FragmentedMessageHolder.h"
 
TIDorb::core::comm::FragmentedMessageHolder::FragmentedMessageHolder
    (TIDorb::core::comm::iiop::GIOPFragmentedMessage* msg) 
: RemovableObject()
{
	message = msg;
}

TIDorb::core::comm::FragmentedMessageHolder::~FragmentedMessageHolder()
{
    delete message;
}

TIDorb::core::comm::iiop::GIOPFragmentedMessage*
TIDorb::core::comm::FragmentedMessageHolder::consume_message() 
{
    TIDorb::core::comm::iiop::GIOPFragmentedMessage* msg  = message;
    message = NULL;
    return msg;
}


/**
 * The message will always can be removed.
 */
bool TIDorb::core::comm::FragmentedMessageHolder::canBeRemoved()
{
	return true;
}
