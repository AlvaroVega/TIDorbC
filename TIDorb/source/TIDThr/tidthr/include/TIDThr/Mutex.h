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

