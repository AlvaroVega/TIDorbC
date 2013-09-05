/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/LockPool.h
  
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

