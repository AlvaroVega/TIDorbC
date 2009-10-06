/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/LockPool.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_LockPool_H_
#define _TIDORB_core_comm_LockPool_H_ 1

#include <stack>

#include "TIDThr.h"
#include "TIDorb/core/comm/Lock.h"

namespace TIDorb {
namespace core {
namespace comm {

class LockPool : public virtual TIDThr::RecursiveMutex {	
  
  protected:
                  
     typedef  stack<TIDorb::core::comm::Lock*> LockStack;          
	 
     LockStack locks;
	 	
	public:
	 LockPool() throw (TIDThr::SystemException) {}
	 ~LockPool() throw (TIDThr::SystemException) {}
	     
	 Lock* get_lock();                       
	 void put_lock(Lock*);
     
};

}
}
}

#endif

