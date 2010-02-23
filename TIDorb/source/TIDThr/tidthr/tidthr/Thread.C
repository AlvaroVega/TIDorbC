///////////////////////////////////////////////////////////////////////////
//
// File:        Thread.C
// Description: This file contains the Thread class methods
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

#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

namespace TIDThr {

Key* Thread::st_private_key(NULL);

Counter* Thread::st_thread_counter(NULL);

ThreadGroupHandle* Thread::st_root_group(NULL);

bool Thread::st_initialized(false);


//PRA
extern "C" { typedef void* (*pthread_function_t)(void*); }
//EPRA


void
init()
    throw (SystemException)
{
    if(Thread::st_initialized) {
        //	  throw SystemException("TIDThr already initialized");
        return;
    }

    Thread::st_private_key = new Key();
    Thread::st_thread_counter = new Counter();
    Thread::st_root_group = new ThreadGroupHandle();
    *(Thread::st_root_group) = new ThreadGroup("MainGroup");
    Thread::st_initialized = true;

}

Thread::Thread(const char* name)
    throw (SystemException)
    : m_thread_group(NULL),
      m_thread_state(TIDTHR_ERROR),
      m_thread_daemon(true),
      m_thread_wait_interrupted(false),
      m_thread_in_wait(false),
      m_thread_target(NULL),
      m_thread_id(0),      
      m_thread_attributes(NULL),
      m_thread_name(name)
{
    if(!st_initialized) {
        throw SystemException("TIDThr is not initialized");
    }

    m_thread_group = *st_root_group;

    m_thread_mutex = new Mutex;

    m_thread_state = TIDTHR_CREATED;
}

Thread::Thread(Runnable* target, 
               const char* name)
    throw (IllegalThreadStateException,
           IllegalArgumentException,
           SystemException)
    : m_thread_group(NULL),
      m_thread_state(TIDTHR_ERROR), 
      m_thread_daemon(true),
      m_thread_wait_interrupted(false),
      m_thread_in_wait(false),      
      m_thread_target(target),
      m_thread_id(0),
      m_thread_attributes(NULL),
      m_thread_name(name)
{

    if(!st_initialized) {
        throw SystemException("TIDThr is not initialized");
    }

    m_thread_group = *st_root_group;

    m_thread_mutex = new Mutex;

    if(m_thread_target.is_null()) {
        throw IllegalArgumentException("Thread::Thread NULL target");
    }

    m_thread_state = TIDTHR_CREATED;
}

Thread::Thread(ThreadGroup* group,
               Runnable* target, 
               const char* name)
    throw (IllegalThreadStateException,
           IllegalArgumentException,
           SystemException)
    : m_thread_group(group),
      m_thread_state(TIDTHR_ERROR),
      m_thread_daemon(1),
      m_thread_wait_interrupted(false),
      m_thread_in_wait(false),     
      m_thread_target(target),
      m_thread_id(0), 
      m_thread_attributes(NULL),     
      m_thread_name(name)
{

    if(!st_initialized) {
        throw SystemException("TIDThr is not initialized");
    }

    m_thread_mutex = new Mutex;

    if(m_thread_group.is_null()) {
        throw IllegalArgumentException("Thread::Thread NULL group");
    }

    m_thread_state = TIDTHR_CREATED;
}

Thread::~Thread()
    throw (SystemException)
{
    if(m_thread_attributes != NULL ) {
        pthread_attr_destroy(m_thread_attributes);
//MLG        
        //free(m_thread_attributes);
//EMLG        
    }
}

void
Thread::setDaemon(bool value)
{
    Synchronized synchro(*m_thread_mutex);

    if(!value) { 
        if(m_thread_daemon) {          
            m_thread_daemon = false;              
            if(m_thread_state ==  TIDTHR_RUNNING) {
                st_thread_counter->dec();
            }
        }
    } else {
        if(!m_thread_daemon) {
            m_thread_daemon = true;
            if(m_thread_state ==  TIDTHR_RUNNING) {
                st_thread_counter->inc();  
            }
        }  
    }
}

int
Thread::getPriority() const
    throw (IllegalThreadStateException)
{
    if(m_thread_state == TIDTHR_ERROR){
        throw IllegalThreadStateException
                ("Thread::getPriority() Invalid Object");
    }

    sched_param param;
    int policy;

    if(m_thread_state != TIDTHR_CREATED) {
        if(pthread_getschedparam(m_thread_id.value(),&policy, &param)) {
            return param.sched_priority;
        }      
    }
    // else the thread is not active, return the Thread creation priority
  
    const pthread_attr_t* attr = 
        (m_thread_attributes == NULL) ?  
            m_thread_group->getAttributes() : m_thread_attributes;

    pthread_attr_getschedparam(attr, &param);

    return param.sched_priority;
}

void
Thread::setPriority(int value) 
    throw (IllegalThreadStateException,
           IllegalArgumentException)
{
    if(m_thread_state == TIDTHR_ERROR) {
        throw IllegalThreadStateException
            ("Thread::setPriority() Invalid Object");
    }

    sched_param param;
    int policy;

    int ret;

    /*synchronized(thread_mutex) */ 
    {
        Synchronized synchro(*m_thread_mutex);

        if(m_thread_state == TIDTHR_CREATED) {
            if(m_thread_attributes == NULL) {
                m_thread_attributes = 
                    (pthread_attr_t *) malloc(sizeof(pthread_attr_t));
                    
                pthread_attr_init(m_thread_attributes);
                
                ThreadGroup::attrCopy(m_thread_group->getAttributes(), 
                                      m_thread_attributes);
            }

            pthread_attr_getschedparam(m_thread_attributes, &param);

            param.sched_priority = value;
            ret = pthread_attr_setschedparam(m_thread_attributes, &param);
            if(ret) {
                throw IllegalArgumentException
                        ("Thread::setPriority(int value)",ret);
            }

            return;
        }
    } // end sinchronized(thread_mutex)

    //THREAD RUNNING OR DEATH
    ret = pthread_getschedparam(m_thread_id.value(), &policy, &param);
    if(ret) {
        throw IllegalThreadStateException
            ("Thread::setPriority(int value) pthread_getschedparam(): "
             "thread is dead",ret);
    }

    param.sched_priority = value;
  
    ret = pthread_setschedparam(m_thread_id.value(),policy, &param);

    if(!ret) {
        return;
    } else if(ret == ESRCH) {
        throw IllegalThreadStateException
            ("Thread::setPriority(int value) pthread_setchdparam(): "
             "thread is dead",ret);
    } else {
        throw IllegalArgumentException("Thread::setPriority(int value)",ret);
    }
} //    ThreadGroup &getThreadGroup();

void
Thread::join() 
    throw (IllegalThreadStateException,
           InterruptedException,
           SystemException)
{
    if(m_thread_state == TIDTHR_ERROR) {
        throw IllegalThreadStateException("Thread::join() Invalid Object");
    }

    int ret = pthread_join(m_thread_id.value(),NULL);
 
    switch(ret) {
    case 0:
        return;
    case ESRCH:
        throw IllegalThreadStateException("Thread::join() Invalid Object",ret);
    case EINTR:
        throw InterruptedException("Thread::join()", ret);
    default:
        throw SystemException("Thread::join()",ret);
    }
}

void Thread::interrupt()
    throw (IllegalThreadStateException)
{
    Synchronized synchro(*m_thread_mutex);

    int ret = 0;

    if(m_thread_state == TIDTHR_ERROR) {
        throw IllegalThreadStateException
                ("Thread::interrupt() Invalid Object",ret);
    }

    if(m_thread_in_wait == true) {
        m_thread_wait_interrupted = true;
    }
    ret = pthread_kill(m_thread_id.value(),SIGINT);
    if(ret) {
        throw IllegalThreadStateException
                ("Thread::interrupt() Invalid Object",ret);
    }
};

bool 
Thread::interrupted()
{
    ThreadHandle handle = currentThread();
  
    if(!handle.is_null()) {
        return handle->consumeInterruptionSync();
    } else {
        return false;
    }
};

bool
Thread::consumeInterruptionSync()
{
    Synchronized synchro(*m_thread_mutex);
    return consumeInterruption();
};

bool
Thread::consumeInterruption()
{
    bool temp = m_thread_wait_interrupted;
    m_thread_wait_interrupted = false;
    return temp;
};

void
Thread::preWaitCondition()
{
    Thread* thread = currentThread();
    if(thread != NULL){
        thread->doPreWaitCondition();
    }
};

void
Thread::doPreWaitCondition()
{
    Synchronized synchro(*m_thread_mutex);
    m_thread_in_wait = true;
};


bool
Thread::postWaitCondition()
{
    Thread* thread = currentThread();
    if(thread != NULL){
        return thread->doPostWaitCondition();
    } else {
        return false;
    }
}

bool
Thread::doPostWaitCondition()
{
    Synchronized synchro(*m_thread_mutex);
    m_thread_in_wait = false;
    return consumeInterruption();
}


void* 
Thread::performRun(ThreadHandle* handle)
{
    // set the cancelation handler
    pthread_cleanup_push(st_thread_death,handle);
  
    setSpecific(*st_private_key,handle);

    m_thread_id = pthread_self();
  
    m_thread_group->addThread(*handle);
 
    if(!m_thread_target.is_null()) {
        m_thread_target->run();
    } else {
        run();
    }
  
    // thread death, execute also the thread_death
    pthread_cleanup_pop(1);

    return 0;
}


/*synchronized*/ 
void
Thread::start() 
    throw (IllegalThreadStateException)
{
    // prevent this method from ending before the thread has dead
    ThreadHandle my_handle = this;     
  
    {
        Synchronized synchro(*m_thread_mutex);
    
        
        if(m_thread_state == TIDTHR_ERROR) {
            throw IllegalThreadStateException
                    ("Thread::start() Invalid Object");
        } else if(m_thread_state == TIDTHR_RUNNING) {
            throw IllegalThreadStateException
                    ("Thread::start() Thread already Running");
        } else if(m_thread_state == TIDTHR_DEAD) {
            throw IllegalThreadStateException
                    ("Thread::start() Thread is dead");
        }
    
        const pthread_attr_t* attr = 
            (m_thread_attributes == NULL)? 
                m_thread_group->getAttributes() : m_thread_attributes;
    
        int ret;
    
        ThreadHandle* handle = new ThreadHandle(this);
    
        if(m_thread_daemon) {
            st_thread_counter->inc();
        }
    
        pthread_t thread_desc;
    
        //PRA
        ret = pthread_create(&thread_desc, attr, (pthread_function_t) st_perform_run, handle);
        if(ret) {
            delete handle;

            m_thread_state = TIDTHR_ERROR;    
            
            if(m_thread_daemon) {
                st_thread_counter->dec();
            }
    
            throw IllegalThreadStateException("Thread::start()", ret);
        }
        //EPRA
    }
}

void 
Thread::threadDeath()
{
    m_thread_state = TIDTHR_DEAD;

    m_thread_group->removeThread(getId());

    if(m_thread_daemon) {
        st_thread_counter->dec();
    }
}

void 
Thread::sleep(time_t milliseconds) 
      throw (InterruptedException,
             IllegalArgumentException)
{
    int ret = 0;

    struct timespec interval; 

    // seconds in the millis
    interval.tv_sec = milliseconds / 1000L;

    // remain millis
    interval.tv_nsec = (milliseconds % 1000L) * 1000000L; 
 
    struct timespec remainder = {0,0};
    
    do { 
        ret = nanosleep(&interval,&remainder);
        if(ret) {
            if(errno == EINVAL) {
                throw IllegalArgumentException("Thread::sleep()");
            }
            if(errno == EINTR) {
                throw InterruptedException("Thread::sleep()");
            }
        }
    } while((remainder.tv_sec > 0) || (remainder.tv_nsec > 100000L));
    //while spureous wake-ups:
    // sleep while the remainder was at least 1 millisecont
}

void
Thread::yield() 
{
  //Solaris & Linux
  sched_yield();
  //HP? pthread_yield()
};

Thread* 
Thread::currentThread() 
{
  ThreadHandle* handle = (ThreadHandle*) getSpecific(*st_private_key);
 
  if(handle) {
    return (Thread*) *handle;
  } else {
    return NULL;
  }
}

// POSIX threads extensions



//Library Extensions

void*
Thread::getSpecific(Key& key)
{
  return pthread_getspecific(key.theKey());
}

void
Thread::setSpecific(Key& key, void* value)
    throw (IllegalArgumentException,
           SystemException)
{
    int ret;
    ret = pthread_setspecific(key.theKey(), value);
    if(ret) {
        if(ret == EINVAL) {
            throw IllegalArgumentException("Thread::setSpecific()", ret);
        } else {
            throw SystemException("Thread::setSpecific()",ret);
        }
    }
};

void 
Thread::st_thread_death(void *thread_handle)
{
    // Gets the Thread descritor and deletes it
 
    ThreadHandle* thread_p = (ThreadHandle*) thread_handle; 

    if(thread_p) {    
        (*thread_p)->threadDeath();
        delete thread_p;
    }
}

void*
Thread::st_perform_run(void *thread_handle)
{
    ThreadHandle* thread_p = (ThreadHandle*) thread_handle;

    if(thread_p) {
        return (*thread_p)->performRun(thread_p);
    } else {
        return 0;
    }
}

} // namespace TIDThr 

ostream& operator<<(ostream& os, const TIDThr::Thread& th)
{
  os << "Thread " << th.getId() << " in " << *(th.getThreadGroup());

  return os;
}
