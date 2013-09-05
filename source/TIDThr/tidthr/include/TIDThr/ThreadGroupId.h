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

