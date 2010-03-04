//////////////////////////////////////////////////////////////////////////////////
//
// File:        LockList.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

using TIDorb::core::comm::iiop::RequestId;
using TIDorb::core::comm::iiop::GIOPFragmentedMessage;


namespace TIDorb {
namespace core {
namespace comm {

LockList::LockList(LockPool& pool)
 throw (TIDThr::SystemException)
 : lock_pool(pool),
   destroyed(false)
{}
    
Lock& LockList::activate_lock(RequestId id)
{
    TIDThr::Synchronized synchro(*this);
    
    if(destroyed) {
        throw CORBA::COMM_FAILURE("Destroying connection");
    }
    
    Lock* lock = NULL;
    
    if(active_locks.find(id) != active_locks.end()) {
        throw CORBA::INTERNAL("Cannot activate twice a RequestId in a LockList");        
    } else {
        lock = lock_pool.get_lock();
    }
    
    lock->request_id(id);
    
	active_locks[id] = lock;
    
    return *lock;
}
	
bool LockList::put_reply(RequestId id,
                         GIOPFragmentedMessage* message)
{
    TIDThr::Synchronized synchro(*this);
    
    LockTable::iterator it = active_locks.find(id);
    
    if( it == active_locks.end()) {
        delete message;
        return false;        
    } else {
        (*it).second->put_reply(message);
        return true;
    }	
}
	
void LockList::deactivate_lock(RequestId id)
{
    TIDThr::Synchronized synchro(*this);
    
    LockTable::iterator it = active_locks.find(id);
    
    if( it == active_locks.end()) {
        throw CORBA::INTERNAL("Cannot deactivate RequestId in a LockList");        
    }
        
    Lock* lock = (*it).second;
        
	active_locks.erase(it);
            
    lock_pool.put_lock(lock);
    
    if(destroyed && (active_locks.size() == 0)){
        notifyAll();
    }
}
	
size_t LockList::size()
{
	return active_locks.size();
}
 
void LockList::destroy() 
{
  TIDThr::Synchronized synchro(*this);
  
  if(!destroyed) {
      destroyed = true; 
      
      LockTable::iterator it = active_locks.begin();
      LockTable::iterator end = active_locks.end();
      Lock* lock = NULL;
      while(it != end) {
        lock = (*it).second;
        
        TIDThr::Synchronized sync(*lock);
        lock->notify();
        
        ++it;  
      }
  }
  
  // wait lock removal
  
  while (active_locks.size() < 0) {
    wait();
  }
	
}

} // namespace
}
}
