/*/////////////////////////////////////////////////////////////////////////
//
// File:        ThreadGroupId.h
// Description: This file contains the ThreadGroupId class declaration.
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

#ifndef __TID_THR_THREADGROUP_ID_H__
#define __TID_THR_THREADGROUP_ID_H__ 

#ifdef TIDTHR_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDTHR_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDThr {

typedef unsigned int GroupT;

class ThreadGroupId  {
 
  public:

    //Default Constructor:
    ThreadGroupId(GroupT i = 0U) : m_id(i){}
  
    ThreadGroupId(const ThreadGroupId& other): m_id(other.m_id) {}

    GroupT value() const {return m_id;}

    ThreadGroupId& operator=(const ThreadGroupId& other) 
    {
      m_id = other.m_id;
      return *this;      
    }

    ThreadGroupId& operator++() 
    {
      m_id++;
      return *this;
    }

    ThreadGroupId& operator=(GroupT i) 
    {
      m_id = i;
      return *this;      
    }

    bool operator==(const ThreadGroupId& other) const 
    {
      return (m_id == other.m_id);
    }

    bool operator!=(const ThreadGroupId& other) const  
    {
      return (m_id != other.m_id);
    }

    bool operator>(const ThreadGroupId& other) const 
    {
      return (m_id > other.m_id);
    }

    bool operator>=(const ThreadGroupId& other) const 
    {
      return (m_id >= other.m_id);
    }

    bool operator<(const ThreadGroupId& other) const 
    {
      return (m_id < other.m_id);
    }

    bool operator<=(const ThreadGroupId& other) const  
    {
      return (m_id <= other.m_id);
    }

 private:

    GroupT m_id; 

};
} // namespace

ostream& operator<<(ostream& os, const TIDThr::ThreadGroupId& other);

#endif

