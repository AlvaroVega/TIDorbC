/*/////////////////////////////////////////////////////////////////////////
//
// File:        Synchronized.h
// Description: This file contains the Synchronized class declaration.
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

#ifndef __TID_THR_SYNCHRONIZED_H__
#define __TID_THR_SYNCHRONIZED_H__ 

namespace TIDThr {

class Synchronized {
 
  public:
    // Constructores
    
    Synchronized(Mutex& mutex)
      throw (IllegalMutexStateException,
             SystemException)
    : m_unlock(false),
      m_mutex_ref(&mutex),
      m_r_mutex_ref(NULL)
      
    {
      mutex.lock();
      m_unlock = true;
    }

    Synchronized(RecursiveMutex& mutex)
      throw (IllegalMutexStateException,
             SystemException)
    : m_unlock(false),
      m_mutex_ref(NULL),
      m_r_mutex_ref(&mutex)
      
    {
      mutex.lock();
      m_unlock = true;
    }

    virtual ~Synchronized()
      throw (IllegalMutexStateException,
             IllegalMonitorStateException,
             SystemException)
    {
        if(m_unlock) {
            if (m_mutex_ref != NULL) {
                m_mutex_ref->unlock();
            } else {
                try {
                  m_r_mutex_ref->unlock();
                } catch(const IllegalMutexStateException illegal) {
                  throw IllegalMonitorStateException(illegal.getReason(),
                                                     illegal.getErrno());
                }
            }
        }
    }

  private:
    // unlock at destructor
    bool m_unlock;

    Mutex* m_mutex_ref;
    RecursiveMutex* m_r_mutex_ref;    
  
};

}

#endif

