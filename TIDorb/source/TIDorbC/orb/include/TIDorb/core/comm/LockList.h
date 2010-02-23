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

