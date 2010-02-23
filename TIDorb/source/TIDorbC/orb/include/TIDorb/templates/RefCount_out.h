/*/////////////////////////////////////////////////////////////////////////
//
// File:        RefCount_out.h
// Description: RefCount_out template
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

#ifndef _TIDORB_TEMPLATES_REFCOUNT_OUT_H_
#define _TIDORB_TEMPLATES_REFCOUNT_OUT_H_

namespace TIDorb {
namespace templates {
  
template<class T> class RefCount_out
{
  public:
    RefCount_out(T*& p) : m_ptr(p) 
    { 
      m_ptr = 0; 
    }
    
    RefCount_out(RefCount_var<T>& p) : m_ptr(p.m_ptr) 
    {
      if (m_ptr != 0) m_ptr->_remove_ref();
      m_ptr = 0;
    }
    
    RefCount_out(const RefCount_out& p) : m_ptr(p.m_ptr) {}

    RefCount_out& operator=(const RefCount_out& p)
    {
      m_ptr = p.m_ptr;
      return *this;
    }
    
    RefCount_out& operator=(T* p) 
    {
      m_ptr = p; 
      return *this; 
    }
    
    operator T*&() 
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
    
  private:
    
    T*& m_ptr;
    // assignment from T_var not allowed
    void operator=(const RefCount_var<T>&){}
};

}
}

#endif

