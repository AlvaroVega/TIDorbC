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
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
