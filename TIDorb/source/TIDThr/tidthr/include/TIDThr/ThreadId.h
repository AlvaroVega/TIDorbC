/*/////////////////////////////////////////////////////////////////////////
//
// File:       ThreadId.h
// Description: This file contains the ThreadId class declaration.
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

#ifndef __TID_THR_THREAD_ID_H__
#define __TID_THR_THREAD_ID_H__ 

#include <pthread.h>

#ifdef TIDTHR_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDTHR_HAVE_NAMESPACE_STD
  using namespace std;
#endif

namespace TIDThr {

class ThreadId  {
  
  public:

    //Default Constructor:
    ThreadId(pthread_t id = 0) : m_thread_id(id){}

    //Copy Constructor  
    ThreadId(const ThreadId& other): m_thread_id(other.m_thread_id) {}

    pthread_t value() const {return m_thread_id;}

    ThreadId& operator=(const ThreadId& other)
    {
      m_thread_id = other.m_thread_id;
      return *this;      
    }

    ThreadId& operator=(pthread_t id) 
    {
      m_thread_id = id;
      return *this;      
    }

    bool operator==(const ThreadId& other) const
    {
      return (m_thread_id == other.m_thread_id);
    }

    bool operator!=(const ThreadId& other) const 
    {
      return (m_thread_id != other.m_thread_id);
    }

    bool operator>(const ThreadId& other) const
    {
      return (m_thread_id > other.m_thread_id);
    }

    bool operator>=(const ThreadId& other) const
    {
      return (m_thread_id >= other.m_thread_id);
    }

    bool operator<(const ThreadId& other) const
    {
      return (m_thread_id < other.m_thread_id);
    }

    bool operator<=(const ThreadId& other)const  
    {
      return (m_thread_id <= other.m_thread_id);
    }
    
  protected:

    // Posix Key descriptor
    pthread_t m_thread_id; 
    

}; //class ThreadId

} // namespace

ostream& operator<<(ostream& os, const TIDThr::ThreadId& other);

#endif


