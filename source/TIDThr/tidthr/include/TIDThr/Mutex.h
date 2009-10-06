/*/////////////////////////////////////////////////////////////////////////
//
// File:        Mutex.h
// Description: This file contains the Mutex class declaration.
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


#ifndef __TID_THR_MUTEX_H__
#define __TID_THR_MUTEX_H__

#include <pthread.h>

namespace TIDThr {

class Mutex : public RefCounter {

  friend class Synchronized;
  friend class Condition;

  public:

    //Default Constructor: init the mutex with the default arguments
    Mutex() throw (SystemException);

    // Constructor with mutex user arguments 
    Mutex(const pthread_mutexattr_t* attr) 
      throw (IllegalArgumentException,
             SystemException);

   //Desctructor: decrements the mutex reference count, if 0 destroys it
    virtual ~Mutex() throw (SystemException);
  
    // valid(): checks if the mutex is valid
    int valid () const { return m_error;}
  
    // lock(): mutex lock.
    void lock()
      throw (IllegalMutexStateException, // corrupted mutex or current thread cannot acquire the lock
             SystemException);

    // tryLock(): returns true if OK, or false if the lock is already locked
    bool tryLock()
      throw (IllegalMutexStateException, // corrupted mutex
             SystemException); 
    
    //unlock(): unlock the mutex
    void unlock()
      throw (IllegalMutexStateException, // current thread is not the owner
             SystemException);

  protected:

	pthread_mutex_t* getDescriptor() {return &m_mutex;};

  private:

    // Posix Mutex descriptor
    pthread_mutex_t m_mutex; // mutex

    // Error Flag
    int m_error; 

    // Default mutex attributes
    static MutexAttr st_default_attr;

};

}

#endif

