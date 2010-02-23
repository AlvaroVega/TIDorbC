/*/////////////////////////////////////////////////////////////////////////
//
// File:        TIDThr_ThreadGroup.h
// Description: This file contains the ThreadGroup class declaration.
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

#ifndef __TID_THR_THREADGROUP__
#define __TID_THR_THREADGROUP__ 

#ifdef TIDTHR_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#include <string>

#ifdef TIDTHR_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::string;
  using std::map;
#endif

#include <pthread.h>

namespace TIDThr {

typedef map<ThreadId,ThreadHandle> ThreadTable;
typedef map<ThreadGroupId,ThreadGroupHandle> ThreadGroupTable;


class ThreadGroup: public  RefCounter {

  friend class Thread;

  public:
         
    ThreadGroup(const char* name)
      throw(IllegalThreadStateException,
            IllegalArgumentException);

    ThreadGroup(ThreadGroup* father = NULL, 
                       const char* name ="",
                       const pthread_attr_t* attr = NULL)
    throw (IllegalThreadStateException,
           IllegalArgumentException);

    virtual ~ThreadGroup() throw (SystemException);

    size_t activeCount() const {return m_threads_table.size();}

    size_t activeGroupCount() const {return m_groups_table.size();}

    void destroy()
      throw (IllegalThreadStateException);

    bool isDestroyed() const {return m_destroyed;}

    size_t enumerate(ThreadGroupHandle* list, 
                     size_t list_size, 
                     bool recurse = false) const;
    
    size_t enumerate(ThreadHandle* list, 
                     size_t list_size, 
                     bool recurse = false) const;

    int getMaxPriority() const {return m_max_priority;}
    
    void setMaxPriority(int pri) {m_max_priority = pri;}

    const char* getName() const {return m_group_name.data();}

    ThreadGroup* getParent() const {return m_group_father;}

    bool parentOf(const ThreadGroup& g) const
    {
      return (m_groups_table.count(g.getId()) > 0);
    }
    
    void interrupt(){}

    bool isDaemon() const { return m_daemon;}

    void setDaemon(bool value) {m_daemon = value;}

    void list() const;
 
    // POSIX Extensions

    const pthread_attr_t* getAttributes() const 
    {return &m_group_attributes;}

    static void attrCopy(const pthread_attr_t* from,
                         pthread_attr_t* to)
      throw (IllegalArgumentException);

    void setId(const ThreadGroupId& id) {m_group_id = id;}

    const ThreadGroupId& getId() const {return m_group_id; }


  protected:
    // return the child_id assigned to the child

    void addGroup(const ThreadGroupHandle& child) 
      throw (IllegalThreadStateException);
  
    void removeGroup(const ThreadGroupId& child_id);
  
    void addThread(const ThreadHandle& child)
      throw (IllegalThreadStateException);
   
    void removeThread(const ThreadId& child_id);


  private:

    string m_group_name;    
    ThreadGroupId m_group_id;
    ThreadGroupId m_last_child_id;
    
    MutexHandle m_state_mutex;

    int m_max_priority;

    bool m_destroyed;

    bool m_daemon;

    pthread_attr_t m_group_attributes;

    ThreadTable m_threads_table;

    ThreadGroupTable m_groups_table;

    ThreadGroupHandle m_group_father;

};

} // namespace

ostream& operator << (ostream& os, const TIDThr::ThreadGroup& group); 

#endif

