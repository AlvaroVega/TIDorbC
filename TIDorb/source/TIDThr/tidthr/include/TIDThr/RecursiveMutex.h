/*/////////////////////////////////////////////////////////////////////////
//
// File:        RecursiveMutex.h
// Description: This file contains the RecursiveMutex & RecursiveLock class declarations
// Rel:         01.00
// Created:     September, 2001
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDThr.h"

#ifndef __TID_THR_RECURSIVE_MUTEX_H__
#define __TID_THR_RECURSIVE_MUTEX_H__ 

#include <pthread.h>


namespace TIDThr {

// Threads locks conter in a mutex

class RecursiveLock {
  public:
    RecursiveLock(): m_thread(0), m_count(0) {}
    RecursiveLock(RecursiveLock& lock)
        : m_thread(lock.m_thread), 
          m_count(lock.m_count){}

    RecursiveLock& operator=(RecursiveLock& lock) 
    {
      m_thread = lock.m_thread;
      m_count = lock.m_count;
      return *this;
    }
    void setThread(pthread_t thr_id) {m_thread = thr_id;}
    pthread_t getThread() { return m_thread;}
    size_t getCount() { return m_count;}

    size_t incCount() {return ++m_count;}
    size_t decCount() {return --m_count;}

    void reset() 
    {
      m_thread = 0;
      m_count = 0;
    };

  private:
    pthread_t m_thread;
    size_t m_count;
  
};

class RecursiveMutex : public virtual RefCounter {
  friend class Synchronized;
  friend class Condition;

  public:
 
    //Default Constructor: init the mutex with the default arguments
    RecursiveMutex() throw (SystemException);

    virtual ~RecursiveMutex() throw (SystemException);
    
    // valid(): checks if the mutex is valid
    int valid () const { return m_error;}
  
    // lock(): mutex lock.
    void lock()
      throw (IllegalMutexStateException, // corrupted mutex or current thread cannot acquire the lock
             SystemException);

    // tryLock(): returns true if OK, or false if the lock is already locked
    bool trylock()
      throw (IllegalMutexStateException, // corrupted mutex
             SystemException); 
    
    //unlock(): unlock the mutex
    void unlock()
      throw (IllegalMutexStateException, // current thread is not the owner
             SystemException);

  protected:
    pthread_mutex_t* getDescriptor() {return &m_mutex;};

    RecursiveLock& getRecursiveLock() {return m_lock;}
    
    void setRecursiveLock(RecursiveLock& lock) {m_lock = lock;}

  private:
    // Posix Mutex descriptor
    pthread_mutex_t m_mutex; // mutex
    RecursiveLock m_lock; //recursive locks counter

    // Error Flag
    int m_error; 

    // Default mutex attributes
    static MutexAttr st_default_attr;

};

}

#endif
