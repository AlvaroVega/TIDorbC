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

