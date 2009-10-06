///////////////////////////////////////////////////////////////////////////
//
// File:        ThreadGroup.C
// Description: This file contains the ThreadGroup class methods
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

namespace TIDThr {

ThreadGroup::ThreadGroup(const char* name)
    throw(IllegalThreadStateException,
          IllegalArgumentException)
    : m_group_name(name),
      m_group_id(0),
      m_destroyed(false),
      m_group_father(NULL)
{
    pthread_attr_init(&m_group_attributes);
    pthread_attr_setscope(&m_group_attributes,PTHREAD_SCOPE_PROCESS);
    pthread_attr_setdetachstate(&m_group_attributes,PTHREAD_CREATE_DETACHED);

    m_state_mutex = new Mutex();
}

ThreadGroup::ThreadGroup(ThreadGroup *father,
                         const char* name,
                         const pthread_attr_t* attr)
      throw(IllegalThreadStateException,
            IllegalArgumentException)
      : m_group_name(name),
        m_group_id(0), 
        m_destroyed(false),
        m_group_father(father)
{
    if(father != NULL){  
        ThreadGroupHandle this_group(this);
        m_destroyed = father->m_destroyed;
        father->addGroup(this_group);
    }

    pthread_attr_init(&m_group_attributes);

    if (attr != NULL) {
        attrCopy(attr, &m_group_attributes);
    } else if(father != NULL) {
        attrCopy(father->getAttributes(), &m_group_attributes);
    }

    m_state_mutex = new Mutex();
}

ThreadGroup::~ThreadGroup() 
    throw (SystemException)
{
    pthread_attr_destroy(&m_group_attributes);

    if(!m_group_father.is_null()) {
        m_group_father->removeGroup(getId());
    }
}

void ThreadGroup::destroy()
    throw (IllegalThreadStateException)
{
    Synchronized synchro(*m_state_mutex);

    if(m_destroyed) {
        throw IllegalThreadStateException("ThreadGroup Already destroyed");
    }

    if((activeCount() > 0) || (activeGroupCount() > 0)) {
        throw IllegalThreadStateException("ThreadGroup not empty");
    }

    m_destroyed = true;
}


void 
ThreadGroup::addGroup(const ThreadGroupHandle& child) 
    throw (IllegalThreadStateException)
{
    Synchronized syncrho(*m_state_mutex);
 
    if(m_destroyed) {
        throw IllegalThreadStateException("Destroyed ThreadGroup");
    }
   
    child->setId(m_last_child_id);
  
    m_groups_table.insert(ThreadGroupTable::value_type(m_last_child_id,child));

    ++m_last_child_id;
}

void 
ThreadGroup::removeGroup(const ThreadGroupId& child_id)
{
    Synchronized syncrho(*m_state_mutex);
    m_groups_table.erase(child_id);
}

void ThreadGroup::addThread(const ThreadHandle& child)
    throw (IllegalThreadStateException)
{
    Synchronized synchro(*m_state_mutex);
 
    if(m_destroyed) {
        throw IllegalThreadStateException("Destroyed ThreadGroup");
    }
      
    m_threads_table.insert(ThreadTable::value_type(child->getId(),child));
}
 
void 
ThreadGroup::removeThread(const ThreadId& child_id)
{
    Synchronized synchro(*m_state_mutex);
    m_threads_table.erase(child_id);
}

size_t 
ThreadGroup::enumerate(ThreadHandle* list,
                       size_t list_size,
                       bool recurse) const
{
    Synchronized synchro(*m_state_mutex);
  
    // current group list
  
    ThreadTable::const_iterator thread_iter = m_threads_table.begin();
  
    size_t count = 0;
  
    size_t threads_table_size = m_threads_table.size();

    while((count < list_size) && (count < threads_table_size)) {
        list[count++] = (*thread_iter).second;
        thread_iter++;
    }
  
    // child groups list
  
    if((count > list_size) || (!recurse)) {
        return count;
    }
  
    ThreadGroupTable::const_iterator group_iter = m_groups_table.begin();
  
 
    while((count < list_size) && (group_iter != m_groups_table.end())) {
        count += ((*group_iter).second)->enumerate(&(list[count]), 
                                                   list_size - count,
                                                   recurse);
        group_iter++;
    }  
  
    return count;
}


void
ThreadGroup::list() const
{ 
    Synchronized synchro(*m_state_mutex);
    
    cout << *this << endl;
  
    // groups 
  
    ThreadGroupTable::const_iterator group_iter = m_groups_table.begin();

    const ThreadGroup* group  = NULL;
   
    while(group_iter != m_groups_table.end()) {
        group = (const ThreadGroup *) ((*group_iter).second);
        if(group){
            cout << '\t'<< *group <<  endl;
        }
        group_iter++;
    }
  
    // threads
  
    
    ThreadTable::const_iterator thread_iter = m_threads_table.begin();
  
    const Thread* thread = NULL; 
  
    while(thread_iter != m_threads_table.end()) {
        thread = (*thread_iter).second;
        if(thread){
            cout << '\t' << *thread << endl;
        }    
        thread_iter++;
    }
   
}

size_t
ThreadGroup::enumerate(ThreadGroupHandle* list, 
                       size_t list_size, 
                       bool recurse) const
{
    Synchronized synchro(*m_state_mutex);
  
    // current group list
  
    ThreadGroupTable::const_iterator groups_iter = m_groups_table.begin();
  
    size_t count = 0;
  
    size_t groups_table_size = m_groups_table.size();
  
    while((count < list_size) && (count < groups_table_size)) {
        list[count++] = (*groups_iter).second;
        groups_iter++;
    }
  
    // child groups list
  
    if((count >= list_size) || (!recurse)) {
        return count;
    }
 
    ThreadGroupTable::const_iterator child_groups_iter = m_groups_table.begin();
   
    while((count < list_size) && (child_groups_iter != m_groups_table.end())) {
        count += ((*child_groups_iter).second)->enumerate(&(list[count]), 
                                                          list_size - count,
                                                          recurse);
        child_groups_iter++;
    }  
  
    return count;
}

void 
ThreadGroup::attrCopy(const pthread_attr_t* from, pthread_attr_t* to)
    throw (IllegalArgumentException)
{
    if((from == NULL) || (to == NULL)) {
        throw IllegalArgumentException("Null pthread_attr_t pointer");
    }
  
    int ret = 0;   

    int deatached;
    ret= pthread_attr_getdetachstate(from,&deatached);
    if(ret) {
        throw IllegalArgumentException("Null from pthread_attr_t pointer",ret);
    }
    ret= pthread_attr_setdetachstate(to,deatached);
    if(ret) {
        throw IllegalArgumentException("Null to pthread_attr_t pointer",ret);
    }

    size_t size;
    
// #if !defined(__linux__)
    pthread_attr_getguardsize(from, &size);
    pthread_attr_setguardsize(to, size);
// #endif
    pthread_attr_getstacksize(from, &size);
    pthread_attr_setstacksize(to, size);

    int scope;
    pthread_attr_getscope(from, &scope);
    pthread_attr_setscope(to, scope);

    int policy;
    pthread_attr_getschedpolicy(from, &policy);
    pthread_attr_setschedpolicy(to, policy);

    pthread_attr_getinheritsched(from, &policy);
    pthread_attr_setinheritsched(to, policy);
  
    sched_param param;
  
    pthread_attr_getschedparam(from, &param);
    pthread_attr_setschedparam(to, &param);
}

} // namespace TIDThr 

ostream& operator<<(ostream& os, const TIDThr::ThreadGroup& group)
{
  os << "ThreadGroup " << group.getId() << " \"" << group.getName() << "\"";
  return os;
}
