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
  //jagd 
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
  //synchronized public boolean createNewReader() {
  bool createNewReader();
    
  void createThread();
      
  /**
   * Counts the number of active threads (increment).
   * @param t Thread that becomes active.
   * @param firstTime It is true if thread t is active for the first time.
   */
  //synchronized public void setActive(Thread t, boolean firstTime) {
  void setActive(TIDThr::Thread* t, bool firstTime);
    
  /**
   * Counts the number of active threads (decrement).
   * @param t Thread that becomes inactive.
   */
  //synchronized public void setInactive(Thread t) {
  void setInactive(TIDThr::Thread* t);
    
  /**
   * Clean up when a thread dies.
   * @param t Thread that has died.
   */
  //synchronized public void threadHasDied(Thread t) {
  void threadHasDied(TIDThr::Thread* t);
    
  //synchronized public void deactivation()
  void deactivation();
  
  //synchronized public boolean threadCanDie(Thread t) {
  bool threadCanDie(TIDThr::Thread* t);
    
  //synchronized public void minThreadsHasChanged() {
  void minThreadsHasChanged();

  void maxThreadsHasChanged();
  
//MLG  
  CORBA::ULong getActives() { return _active; }  
//EMLG  
      
};
} //poa
} //core
} //TIDorb
#endif

