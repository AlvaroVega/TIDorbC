/////////////////////////////////////////////////////////////////////////
//
// File:        CompletionWaiter.h
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

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_CompletationWaiter_H_
#define _TIDORB_CORE_POA_CompletationWaiter_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace poa {

class CompletionWaiter {

private:  

	CORBA::ULong _active_requests;
  	bool _stopWaiting;
	TIDorb::core::TIDORB* _orb;
	
	TIDThr::Monitor monitor;
  
public:

	CompletionWaiter(TIDorb::core::TIDORB* orb);
	~CompletionWaiter();
	
	/**
   	* Begins a new request (increments active request counter).
   	*/
  	void beginRequest();
    
  	/**
   	* Ends a request (decrements active request counter and notifies).
   	*/
  	void endRequest();
  	
  	/**
   	* @return Number of active requests.
   	*/
  	CORBA::ULong getActiveRequests();
  	
  	/**
   	* Tests if an operation should wait for completion.
   	* @return Returns true if it must wait.
   	*/
	bool conditionToWait();

	/**
   	* Waits until there are no active requests.
   	*/
  	void waitForCompletion();
  	
  	/**
   	* Stops waiting for completion.
   	*/
  	void stopWaiting();
  	
   
};

} //poa
} //core
} //TIDorb
#endif
