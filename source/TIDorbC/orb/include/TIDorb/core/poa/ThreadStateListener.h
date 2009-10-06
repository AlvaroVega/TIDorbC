/////////////////////////////////////////////////////////////////////////
//
// File:        ThreadStateListener.h
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

#ifndef _TIDORB_CORE_POA_ThreadStateListener_H_
#define _TIDORB_CORE_POA_ThreadStateListener_H_

#include "TIDThr.h"

/**
 * Listener of thread state changes.
*/
namespace TIDorb {
namespace core {
namespace poa {

class ThreadStateListener {
public:  
	virtual bool threadCanDie(TIDThr::Thread* t)=0;

	virtual void threadHasDied(TIDThr::Thread* t)=0;
  
  	virtual void setActive(TIDThr::Thread* t, bool firstTime)=0;

  	virtual void setInactive(TIDThr::Thread* t);

	ThreadStateListener();
	virtual ~ThreadStateListener();
};

} //poa
} //core
} //TIDorb
#endif
