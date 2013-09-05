/////////////////////////////////////////////////////////////////////////
//
// File:        ThreadPool.h
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

#ifndef _TIDORB_CORE_POA_ThreadPool_H_
#define _TIDORB_CORE_POA_ThreadPool_H_

#include "TIDThr.h"

#include <list>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::list;
#endif

/**
 * Pool of threads.
*/
namespace TIDorb {
namespace core {
namespace poa {

class ThreadPool : public TIDorb::core::poa::QueueReaderManager, 
		public TIDorb::core::poa::ThreadStateListener,
		public TIDorb::core::poa::POAManagerConfListener 
{
private:  
  list<TIDorb::core::poa::ExecThread_ref> _pool;
  TIDorb::core::poa::POAManagerImpl* _poaManager;
  CORBA::ULong _used;
  CORBA::ULong _actuallyUsed;
  CORBA::ULong _active;
  bool _deactivated;
  TIDThr::RecursiveMutex recursive_mutex;
  TIDThr::ThreadGroupHandle _group; 
  unsigned long _maxThreads;  
  unsigned long _minThreads;  
public:
  /**
   * Constructor.
   * @param poaManager The POAManager which this thread pool belongs to.
   */
  ThreadPool(TIDorb::core::poa::POAManagerImpl* poaManager);

	
  virtual ~ThreadPool();
    	  
  /**
   * Test if another thread should be created.
   * @return Returns true if another thread has been started, false otherwise.
   */
  bool createNewReader();
    
      
  /**
   * Counts the number of active threads (increment).
   * @param t Thread that becomes active.
   * @param firstTime It is true if thread t is active for the first time.
   */
  void setActive(TIDThr::Thread* t, bool firstTime);
    
  /**
   * Counts the number of active threads (decrement).
   * @param t Thread that becomes inactive.
   */
  void setInactive(TIDThr::Thread* t);
    
  /**
   * Clean up when a thread dies.
   * @param t Thread that has died.
   */
  void threadHasDied(TIDThr::Thread* t);
    
  void deactivation();
  
  bool threadCanDie(TIDThr::Thread* t);
    
  void minThreadsHasChanged();

  void maxThreadsHasChanged();
  
  CORBA::ULong getActives() { return _active; }  

 private:
  void createThread();

};
} //poa
} //core
} //TIDorb
#endif

