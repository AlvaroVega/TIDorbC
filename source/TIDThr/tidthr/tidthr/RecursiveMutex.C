///////////////////////////////////////////////////////////////////////////
//
// File:        RecursiveMutex.C
// Description: This file contains the Recursive Mutex class methods
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

#include <pthread.h>
#include <errno.h>

namespace TIDThr {

// Default mutex attributes initialization

MutexAttr RecursiveMutex::st_default_attr;


// *********************************************************************
// Constructores:
// *********************************************************************


RecursiveMutex::RecursiveMutex()
    throw (SystemException)
{
    m_error = pthread_mutex_init(&m_mutex, st_default_attr);
  
    if(m_error) {
        throw SystemException("RecursiveMutex::RecursiveMutex(): ",m_error);
    }
} //RecursiveMutex::RecursiveMutex()

RecursiveMutex::~RecursiveMutex()
    throw (SystemException)
{
    if(m_error) {
        return;
    }

    // Try to destroy the mutex
    int ret;
    ret = pthread_mutex_destroy(&m_mutex);
    if(ret) { // ERROR
        throw SystemException("RecursiveMutex::~RecursiveMutex(): ",ret);
    }
} // RecursiveMutex::~RecursiveMutex() 

void RecursiveMutex::lock()
    throw (IllegalMutexStateException,
           SystemException)
{
    if (m_error) {
        throw IllegalMutexStateException
            ("RecursiveMutex::lock() Invalid Mutex", m_error);
    }
      
    int ret = pthread_mutex_lock(&m_mutex);
    
    if (!ret)  { // first lock, OK!!
        m_lock.reset();
        m_lock.setThread(pthread_self());
        m_lock.incCount();
        return;
    }

    switch(ret) {
        case EDEADLK:
            if(m_lock.getCount() == 0) {
                throw SystemException
                    ("RecursiveMutex::lock(): unexpected error,"
                     " current thread alreadys owns the lock", ret);
            } else {
                m_lock.incCount();
                return;
            }
        break;
#ifdef __sun 
        case EOWNERDEAD: // inconsistent lock
            throw IllegalMutexStateException
                    ("RecursiveMutex::lock(): Mutex Lock owner is dead", ret);
        break;
#endif

        default:
            throw SystemException("RecursiveMutex::lock(): ",ret);
    } // lock
}


// Mutex trylock: returns true if OK, or false if the lock is already locked
bool
RecursiveMutex::trylock()
    throw (IllegalMutexStateException,
           SystemException)
{
    if (m_error) {
        throw IllegalMutexStateException
                ("RecursiveMutex::trylock() Invalid Mutex", m_error);
    }
  
    int ret = pthread_mutex_trylock(&m_mutex);
  
    if (!ret) { // first lock, OK!!
        m_lock.reset();
        m_lock.setThread(pthread_self());
        m_lock.incCount();
        return true;
    } else if(ret == EAGAIN) { // Maximun of recursive locks
        throw SystemException
                ("RecursiveMutex::trylock() no Resources",EAGAIN);
    } else if(ret == EBUSY) {
        if(m_lock.getThread() == pthread_self()) {
            m_lock.incCount();
            return true;
        } else {
            return false;
        }
    } else { 
        throw SystemException("RecursiveMutex::trylock()",ret);
    }
};


void
RecursiveMutex::unlock()
    throw (IllegalMutexStateException,
           SystemException)
{
    if (m_error) {
        throw IllegalMutexStateException
            ("RecursiveMutex::unlock() Invalid Mutex", m_error);
    }
    
    if(m_lock.getThread() != pthread_self()) {
        throw IllegalMutexStateException 
                ("RecursiveMutex::unlock() current thread not owner"); 
    }

    if(m_lock.decCount() == 0) {
        m_lock.reset();
        int ret = pthread_mutex_unlock(&m_mutex);  
    
        if (!ret) {
            return;
        }
    
        // Error: unlock
    
        if(ret == EPERM) {
            throw SystemException
                    ("RecursiveMutex::unlock() internal error,"
                     " current thread not owner",ret);
        } else {
            throw SystemException("RecursiveMutex::unlock()",ret);
        }
    }
}; // unlock


} // namespace TIDThr 
