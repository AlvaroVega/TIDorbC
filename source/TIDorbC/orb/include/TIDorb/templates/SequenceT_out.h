/*/////////////////////////////////////////////////////////////////////////
//
// File:        SequenceT_out.h
// Description: SequenceT_out template
//              
// Rel:         01.00
// Created:     November, 2002
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

#ifndef _TIDORB_templates_SequenceT_out_H_
#define _TIDORB_templates_SequenceT_out_H_



namespace TIDorb {
namespace templates {
  
template<typename T, typename ContentT> class SequenceT_out
{
  public:
      
    SequenceT_out(T*& p) : m_ptr(p) 
    { 
      m_ptr = 0; 
    }
    
    SequenceT_out(SequenceT_var<T,ContentT>& p) : m_ptr(p.out()) {}
    
    SequenceT_out(const SequenceT_out& p) : m_ptr(p.m_ptr) {}
    
    SequenceT_out& operator=(const SequenceT_out& p)
    {
      m_ptr = p.m_ptr;
      return *this;
    }
    
    SequenceT_out& operator=(T* p) 
    {
      m_ptr = p;
      return *this; 
    }
    
    operator T*() 
    {
      return m_ptr; 
    }
    
    operator const T*() const 
    {
      return m_ptr; 
    }
    
    T*& ptr() 
    {
      return m_ptr; 
    }
    
    T* operator->() 
    {
      return m_ptr; 
    }
    
    ContentT& operator[](CORBA::ULong i) 
    {
      return (*m_ptr)[i];
    }
    
  private:
    
    T*& m_ptr;
    // assignment from SequenceT_var not allowed
    void operator=(const SequenceT_var<T,ContentT>&){}
};
}
}

#endif
