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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_CompletationWaiter_H_
#define _TIDORB_CORE_POA_CompletationWaiter_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace poa {

//PRA
//class CompletionWaiter : public TIDThr::Monitor {
class CompletionWaiter {
//EPRA

private:  

	CORBA::ULong _active_requests;
  	bool _stopWaiting;
	TIDorb::core::TIDORB* _orb;
	
	TIDThr::Monitor monitor;
  
public:

//MLG
	//PRA
	//CompletionWaiter(TIDorb::core::TIDORB* orb) throw(TIDThr::SystemException);
	//~CompletionWaiter() throw(TIDThr::SystemException);
	CompletionWaiter(TIDorb::core::TIDORB* orb);
	~CompletionWaiter();
	//EPRA
//EMLG
	
	/**
   	* Begins a new request (increments active request counter).
   	*/
  	//synchronized protected void beginRequest();
  	void beginRequest();
    
  	/**
   	* Ends a request (decrements active request counter and notifies).
   	*/
  	//synchronized protected void endRequest();
  	void endRequest();
  	
  	/**
   	* @return Number of active requests.
   	*/
  	//synchronized protected int getActiveRequests();
  	CORBA::ULong getActiveRequests();
  	
  	/**
   	* Tests if an operation should wait for completion.
   	* @return Returns true if it must wait.
   	*/
	bool conditionToWait();

	/**
   	* Waits until there are no active requests.
   	*/
  	//synchronized protected void waitForCompletion();
  	void waitForCompletion();
  	
  	/**
   	* Stops waiting for completion.
   	*/
  	//synchronized protected void stopWaiting();
  	void stopWaiting();
  	
   
};

} //poa
} //core
} //TIDorb
#endif
