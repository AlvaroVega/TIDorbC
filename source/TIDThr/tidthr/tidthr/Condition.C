///////////////////////////////////////////////////////////////////////////
//
// File:        TIDThr_Condition.C
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

#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>


namespace TIDThr {

// Constructores:

Condition::Condition()
    throw (SystemException)
{
    // Inicializar el mutex:
  
    m_error = pthread_cond_init(&m_condition, NULL);
  
    if(m_error){ 
        throw SystemException("Condition::Condition(): ",m_error);
    }
} //Condition:Condition()
  
Condition::Condition(const pthread_condattr_t *attr)
    throw (IllegalArgumentException,
           SystemException)
{
    // Inicializar el mutex:

    m_error = pthread_cond_init(&(m_condition), attr);

    if(m_error){ 
        throw SystemException("Condition::Condition(): ",m_error);
    }
} //Condition::Condition()


Condition::~Condition() 
    throw (SystemException)
{
    if(m_error)
        return;
    // Intenta destruir un objeto condition.
    int ret = pthread_cond_destroy(&(m_condition));

    if (ret) { // ERROR
        throw SystemException("Condition::~Condition(): ",ret);
    }
} // ~Condition()


void
Condition::wait(Mutex& mutex)
  throw (IllegalConditionStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)

{
    if (m_error) {
        throw IllegalConditionStateException
            ("Condition::wait() Invalid Condition:",
            m_error);
    }

    int ret = 0;

    Thread::preWaitCondition();

    ret= pthread_cond_wait(&m_condition, mutex.getDescriptor());

    if(Thread::postWaitCondition()) {
        throw InterruptedException();
    }

    if (ret) { // Error
        if (ret == EINVAL) {
            throw IllegalArgumentException("Condition::wait()", ret);
        } else {
            throw SystemException("Condition::wait unexpected error: ",ret);
        }
    } 
} // wait


int
Condition::wait(Mutex& mutex,time_t millisecs)
  throw (IllegalConditionStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)
{
    return wait(mutex, millisecs,0UL);
}

int
Condition::wait(Mutex& mutex, time_t millisecs, time_t nanosecs)
  throw (IllegalConditionStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)
{
    // avoid 0 secs wait
    if((millisecs == 0UL) && (nanosecs == 0UL)) {
        wait(mutex);
        return 0;
    }

    if (m_error) {
        throw IllegalConditionStateException
                ("Condition::wait() Invalid Condition",
                 m_error);
    }
 
    int ret = 0;
  
    struct timeval current_time;
  
    gettimeofday(&current_time, NULL);
  
    timespec abstime;

    abstime.tv_sec  = current_time.tv_sec + (millisecs / (time_t)1000UL);
    abstime.tv_nsec = nanosecs + 
     				((millisecs % ((time_t)1000UL)) * ((time_t)1000000UL)) +
                    (current_time.tv_usec * ((time_t)1000UL));

    // normalize
  
    abstime.tv_sec += (abstime.tv_nsec / ((time_t) 1000000000UL));
    abstime.tv_nsec %= ((time_t) 1000000000UL);
  
    Thread::preWaitCondition();
  
    ret = pthread_cond_timedwait(&m_condition,
                                 mutex.getDescriptor(), 
                                 &abstime);

    if(Thread::postWaitCondition()) {
        throw InterruptedException();
    }
    
    if (ret) { // Error
        if (ret == ETIMEDOUT) {
            return ret;
        } else if (ret == EINVAL) {
            throw IllegalArgumentException("Condition::wait()", ret);
        } else{
            throw SystemException("Condition::wait() unexpected error: ",ret);
        }
    }
    
    return 0;
}


void
Condition::wait(RecursiveMutex& mutex)
  throw (IllegalConditionStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)
{
    if (m_error) {
        throw IllegalConditionStateException
                ("Condition::wait() Invalid Condition",
                 m_error);
    }

    int ret = 0;

    Thread::preWaitCondition();

    RecursiveLock lock = mutex.getRecursiveLock();

    ret= pthread_cond_wait(&m_condition, mutex.getDescriptor());

    mutex.setRecursiveLock(lock);

    if(Thread::postWaitCondition()) {
        throw InterruptedException();
    }

    if (ret) { // Error
        if (ret == EINVAL) {
            throw IllegalArgumentException("Condition::wait()", ret);
        } else {
            throw SystemException("Condition::wait(): ",ret);
        }
    } 
} // wait


int
Condition::wait(RecursiveMutex& mutex,time_t millisecs)
  throw (IllegalConditionStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)
{
    return wait(mutex, millisecs,0UL);
}

int
Condition::wait(RecursiveMutex& mutex,time_t millisecs, time_t nanosecs)
  throw (IllegalConditionStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)
{

    // avoid 0 secs wait
    if((millisecs == 0UL) && (nanosecs == 0UL)) {
        wait(mutex);
        return 0;
    }
   
    if (m_error) {
        throw IllegalConditionStateException
                ("Condition::wait() Invalid Condition",
                 m_error);
    }

    int ret = 0;
  
    struct timeval current_time;
  
    gettimeofday(&current_time, NULL);
  
    timespec abstime;
    
    abstime.tv_sec  = current_time.tv_sec + (millisecs / (time_t)1000UL);
    abstime.tv_nsec = nanosecs + 
     				((millisecs % ((time_t)1000UL)) * ((time_t)1000000UL)) +
                    (current_time.tv_usec * ((time_t)1000UL));

    // normalize
  
    abstime.tv_sec += (abstime.tv_nsec / ((time_t) 1000000000UL));
    abstime.tv_nsec %= ((time_t) 1000000000UL);
  
    Thread::preWaitCondition();

    RecursiveLock lock = mutex.getRecursiveLock();
  
    ret = pthread_cond_timedwait(&m_condition, 
                                 mutex.getDescriptor(), 
                                 &abstime);

    mutex.setRecursiveLock(lock);

    if(Thread::postWaitCondition()) {
        throw InterruptedException();
    }
    
    if (ret) { // Error
        if (ret == ETIMEDOUT) {
            return ret;
        } else if (ret == EINVAL) {
            throw IllegalArgumentException("Condition::wait()", ret);
        } else {
            throw SystemException("Condition::wait(): ",ret);
        }
    }
    
    return 0;
}

void
Condition::signal()
  throw(IllegalConditionStateException,
        SystemException)
{
    if (m_error) {
        throw IllegalConditionStateException
               ("Condition::signal() Invalid Condition",
                m_error);
    }

    int ret;
    ret = pthread_cond_signal(&m_condition);
    if (ret){
        throw SystemException("Condition::signal(): ",ret);
    }
}


void
Condition::broadcast()
  throw(IllegalConditionStateException,
        SystemException)
{
    if (m_error) {
        throw IllegalConditionStateException
               ("Condition::broadcast() Invalid Condition", 
                m_error);
    }

    int ret;
    ret = pthread_cond_broadcast(&m_condition);
    if (ret) {
        throw SystemException("Condition::broadcast(): ",ret);
    }
}

} // namespace TIDThr
