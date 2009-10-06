#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_LockList_H_
#define _TIDORB_core_comm_LockList_H_ 1

#include <map>
#include <stack>

#include "TIDThr.h"
#include "TIDorb/core/comm/Lock.h"
#include "TIDorb/core/comm/LockPool.h"

/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/LockList.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */



namespace TIDorb {
namespace core {
namespace comm {

class LockList : public virtual TIDThr::Monitor {	
  
  protected:
  
	 typedef map<TIDorb::core::comm::iiop::RequestId, Lock*> LockTable;
                 
      
	 LockTable active_locks;
     LockPool& lock_pool;
     
     bool destroyed;
	 
	
	public:
	 
     LockList(LockPool& pool) throw (TIDThr::SystemException);
	 ~LockList() throw (TIDThr::SystemException) {}
	 
     /**
      * Initialize a new lock
      */
	 Lock& activate_lock(TIDorb::core::comm::iiop::RequestId request_id);
     
     /**
      * Sets a reply message to a given requestId
      * @return true if exist a lock, false otherwise
      */
	 bool put_reply(TIDorb::core::comm::iiop::RequestId request_id,
                    TIDorb::core::comm::iiop::GIOPFragmentedMessage* message);
                      
	 void deactivate_lock(TIDorb::core::comm::iiop::RequestId request_id);
     
	 size_t size();
     
     /**
      *  Destrois the list, and waits for all locks will be cleaned
      */
	 void destroy();
};

}

}

}
#endif

