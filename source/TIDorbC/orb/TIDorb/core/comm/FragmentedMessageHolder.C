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
