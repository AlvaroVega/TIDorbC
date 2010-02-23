///////////////////////////////////////////////////////////////////////////
//
// File:        Mutex.C
// Description: This file contains the Mutex class methods
// Rel:         01.00
// Created:     May, 2001
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
///////////////////////////////////////////////////////////////////////////

#include "TIDThr.h"

#include <errno.h>

namespace TIDThr {

MutexAttr Mutex::st_default_attr;


// *********************************************************************
// Constructores:
// *********************************************************************




Mutex::Mutex()
  throw (SystemException)
{
    // Inicializar el mutex:
    m_error = pthread_mutex_init(&(m_mutex), st_default_attr);

    if(m_error) {
        throw SystemException("Mutex::Mutex(): ",m_error);
    }
} //Mutex::Mutex()

Mutex::Mutex(const pthread_mutexattr_t* attr)
    throw (IllegalArgumentException,
           SystemException)
{
    m_error = pthread_mutex_init(&m_mutex, attr);
 
    if(m_error == EINVAL) {
        throw IllegalArgumentException
                ("Mutex::Mutex(pthread_mutexattr_t *attr)", EINVAL);
    } else {
        throw SystemException
            ("Mutex::Mutex(pthread_mutexattr_t *attr)): ", m_error);
    }
} //Mutex::Mutex(pthread_mutexattr_t *attr)

Mutex::~Mutex()
    throw (SystemException)
{
    if(m_error) {
        return;
    }

    // Intenta destruir un objeto mutex.
    int ret;
    ret = pthread_mutex_destroy(&m_mutex);
    if(ret) { // ERROR
        throw SystemException("Mutex::~Mutex",ret);
    }
} // Mutex::~Mutex() 

void Mutex::lock()
    throw (IllegalMutexStateException,
           SystemException)
{
    if (m_error) {
        throw IllegalMutexStateException
                ("Mutex::lock() Invalid Mutex", m_error);
    }

    int ret = pthread_mutex_lock(&m_mutex);

    if (!ret) {//OK!!
        return;
    }

    if(ret == EDEADLK) {
        throw IllegalMutexStateException
                ("Mutex::lock(): Current thread alreadys owns the lock",                                          
                 ret);
    }

#ifdef __sun 
    if(ret == EOWNERDEAD){ // inconsistent lock
        throw IllegalMutexStateException
                ("Mutex::lock(): Mutex Lock owner is dead", ret);
    }
#endif 

    throw SystemException("Mutex::lock()",ret);
} // lock


  // Mutex tryLock: returns 0 if OK, or !=0 if the lock is already locked
bool 
Mutex::tryLock()
    throw (IllegalMutexStateException,
           SystemException)
{
    if (m_error) {
        throw IllegalMutexStateException
                ("Mutex::trylock() Invalid Mutex", m_error);
    }
  
    int ret = pthread_mutex_trylock(&m_mutex);
  
    if (!ret) {
        return true;
    }

    if(ret == EBUSY) { // Is busy
        return false;
    }
    
    throw SystemException("Mutex::tryLock()",ret);

};


void
Mutex::unlock()
    throw (IllegalMutexStateException,
           SystemException)
{
    if (m_error) {
        throw IllegalMutexStateException
                ("Mutex::unlock() Invalid Mutex", m_error);
    }

    int ret = pthread_mutex_unlock(&m_mutex);

  
    if (!ret) {
        return;
    }

    // Error: unlock
    if(ret == EPERM) {
        throw IllegalMutexStateException 
        ("Mutex::unlock() current thread not owner");
    }
  
    throw SystemException("Mutex::lock()",ret);
}; // unlock

} // namespace TIDThr 
