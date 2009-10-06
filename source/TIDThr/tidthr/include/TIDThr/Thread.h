/*/////////////////////////////////////////////////////////////////////////
//
// File:        Thread.h
// Description: This file contains the Thread class declaration.
//             
// Rel:         01.00
// Created:     September, 2001
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////*/

#include "TIDThr.h"

#ifndef __TID_THR_THREAD__
#define __TID_THR_THREAD__ 


#include <pthread.h>

#ifdef TIDTHR_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDTHR_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
#endif


namespace TIDThr {
  
  //Library initialization

void init() throw (SystemException);

class Thread : public  Runnable
{
  friend class Condition;
  friend void init () throw (SystemException);
  
public:

  Thread(const char* name = "")
    throw (SystemException);

  Thread(Runnable* target, 
         const char* name = "")
    throw (IllegalThreadStateException,
           IllegalArgumentException,
           SystemException);
    
  Thread(ThreadGroup* group,
         Runnable* target = NULL, 
         const char* name = "")
    throw (IllegalThreadStateException,
           IllegalArgumentException,
           SystemException);

  virtual ~Thread() throw (SystemException);

  const char *getName() const {return m_thread_name.data();}

  void setName(const char *name){m_thread_name = name;}

  void setDaemon(bool value);

  bool isDaemon() const {return m_thread_daemon;}

  int getPriority() const
    throw (IllegalThreadStateException);

  void setPriority(int value) 
    throw (IllegalThreadStateException,
           IllegalArgumentException);

  ThreadGroup* getThreadGroup() const {return m_thread_group;}

  bool isAlive() const {return m_thread_state == TIDTHR_RUNNING;};

  void join() 
    throw (IllegalThreadStateException,
           InterruptedException,
           SystemException);

  void interrupt()
    throw (IllegalThreadStateException);

  // interrupted: Tests whether the current thread has been interrupted. 
  // The interrupted status of the thread is cleared by this method. 
  // In other words, if this method were to be called twice in succession, 
  // the second call would return false (unless the current
  // thread were interrupted again, after the first call had cleared 
  // its interrupted status and before the second call had examined it).
  // Returns: true if the current thread has been interrupted; false otherwise.
  static bool interrupted();

  // Tests whether this thread has been interrupted. 
  // The interrupted status of the thread is unaffected by this method.
  // Returns: true if this thread has been interrupted; false otherwise.

  bool isInterrupted() 
  {
    return m_thread_wait_interrupted;
  };

  void start() 
    throw (IllegalThreadStateException);

  virtual void run() {};

  static void sleep(time_t milliseconds) 
    throw (InterruptedException,
           IllegalArgumentException);

  static void yield();

  static Thread* currentThread();

  // POSIX threads extensions

  void set_attributes(const pthread_attr_t* attributes)
    throw (IllegalArgumentException,
           IllegalThreadStateException);

  static pthread_t getCurrentThreadId() 
  {
    return pthread_self();
  }

  const ThreadId& getId() const
    throw (IllegalThreadStateException)
  {
      return m_thread_id;
  }

  static void* getSpecific(Key& key);
 
  static void setSpecific(Key& key, void* value)
    throw (IllegalArgumentException,
           SystemException);
  //#if !defined(__linux__)
  static void setConcurrency(int new_level) 
    throw (IllegalArgumentException)
  {
    int ret;
    ret = pthread_setconcurrency(new_level);
    if(ret)
      throw IllegalArgumentException("Thread::setConcurrency()", ret);
  }

  static int getConcurrency() 
  {
    return pthread_getconcurrency();
  }
  //#endif
  static void waitThreadTermination()
    throw (InterruptedException,
           SystemException)
  {
    if(!st_initialized)
      throw SystemException("TIDThr is not correctly initialized");

    st_thread_counter->waitForZero();
  }
 
  // For internal use
  static void st_thread_death(void *ptr);
  static void* st_perform_run(void *thread_handle);
    
protected:

  bool consumeInterruption();
  
  bool consumeInterruptionSync();
  
  static void preWaitCondition();
  
  void doPreWaitCondition();

  static bool postWaitCondition();
  
  bool doPostWaitCondition();

  void threadDeath(); 

  void* performRun(ThreadHandle* thread_handle);

private:

  //static members

  // Library correctly initializated
  static bool st_initialized;

  // Current Thread Information Key
  static Key* st_private_key;

  // Active Daemon Thread Count for wait ThreadTermination
  static Counter* st_thread_counter;

  // Root Thread Group

  static ThreadGroupHandle* st_root_group;

  // ThreadGroup
  ThreadGroupHandle m_thread_group;

  enum State {TIDTHR_ERROR, TIDTHR_CREATED, TIDTHR_RUNNING, TIDTHR_DEAD};

  State m_thread_state;

  bool m_thread_daemon;

  bool m_thread_wait_interrupted;

  bool m_thread_in_wait;

  RunnableHandle m_thread_target;

  MutexHandle m_thread_mutex;
 
  //    MutexHandler interruption_mutex;

  // Posix Thread descriptor
  ThreadId m_thread_id;
  
  // Thread Attributes
  pthread_attr_t* m_thread_attributes;

  string m_thread_name;
};

} // namespace

ostream& operator << (ostream& os, const TIDThr::Thread& th);

#endif

