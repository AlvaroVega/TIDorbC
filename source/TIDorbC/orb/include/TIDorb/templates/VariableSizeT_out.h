/*/////////////////////////////////////////////////////////////////////////
//
// File:        VariableSizeT_out.h
// Description: VariableSizeT_out template
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

#ifndef _TIDORB_templates_VariableSizeT_out_H_
#define _TIDORB_templates_VariableSizeT_out_H_

namespace TIDorb {
namespace templates {
  
template<typename T> class VariableSizeT_out
{
  public:
      
    VariableSizeT_out(T*& p) : m_ptr(p) 
    { 
      m_ptr = 0; 
    }
    
    VariableSizeT_out(VariableSizeT_var<T>& p) : m_ptr(p.out()) {}
    
    VariableSizeT_out(const VariableSizeT_out& p) : m_ptr(p.m_ptr) {}
    
    VariableSizeT_out& operator=(const VariableSizeT_out& p)
    {
      m_ptr = p.m_ptr;
      return *this;
    }
    
    VariableSizeT_out& operator=(T* p) 
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
    
  private:
    
    T*& m_ptr;
    // assignment from T_var not allowed
    void operator=(const VariableSizeT_var<T>&){}
};
}
}

#endif
