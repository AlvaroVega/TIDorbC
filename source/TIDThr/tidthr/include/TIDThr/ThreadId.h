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


