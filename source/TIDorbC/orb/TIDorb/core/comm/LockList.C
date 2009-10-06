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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
    TIDThr::Synchronized (*this);
    
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
    TIDThr::Synchronized (*this);
    
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
    TIDThr::Synchronized (*this);
    
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
    TIDThr::Synchronized (*this);
	return active_locks.size();
}
 
void LockList::destroy() 
{
  TIDThr::Synchronized (*this);
  
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
