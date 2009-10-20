/*/////////////////////////////////////////////////////////////////////////
//
// File:        VariableSizeT_var.h
// Description: VariableSizeT_var template
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

#ifndef _TIDORB_templates_VariableSizeT_var_H_
#define _TIDORB_templates_VariableSizeT_var_H_


namespace TIDorb {
namespace templates {
  
// C++
template<typename T> class VariableSizeT_var {
  public:
    VariableSizeT_var() : m_ptr(0) {}
    VariableSizeT_var(T *t) : m_ptr(t) {}
    VariableSizeT_var(const T& t) : m_ptr(new T(t)) {}
    
    VariableSizeT_var(const VariableSizeT_var &t) : m_ptr(0) 
    {
      if (t.m_ptr != 0)
        m_ptr = new T(*t.m_ptr);
    }
    
    ~VariableSizeT_var() { delete m_ptr; }
    
    VariableSizeT_var &operator=(T *t) 
    {
      if (t != m_ptr) {
        delete m_ptr;
        m_ptr = t;
      }
      return *this;
    }
    
    VariableSizeT_var &operator=(const T& t) 
    {
      if (&t != m_ptr) {
        delete m_ptr;
        m_ptr = new T(t);
      }
    
      return *this;
    }
    
    VariableSizeT_var &operator=(const VariableSizeT_var &t) 
    {
      if (this != &t) {
        delete m_ptr;
        if (t.m_ptr != 0)
          m_ptr = new T(*t.m_ptr);
        else
          m_ptr = 0;
      }
    
      return *this;
    }
    
    T* operator->() 
    {
      return m_ptr; 
    }
    
    const T* operator->() const 
    {
      return m_ptr; 
    }
    
    operator T* () 
    {
      return m_ptr; 
    }
    
    operator const T* () const 
    {
      return (const T*) m_ptr; 
    }
    
    operator const T& () const
    {
      return *m_ptr;
    }

    operator T& ()
    {
      return *m_ptr;
    }
    const T& in() const 
    {
      return *m_ptr; 
    }
    
    T& inout() 
    {
      return *m_ptr; 
    }
    
    T*& out() 
    {
      delete m_ptr;
      m_ptr = 0;
      return m_ptr;
    }
    
    T* _retn() 
    {
      T* tmp = m_ptr;
      m_ptr = 0;
      return tmp;
    }
  
  private:
    T* m_ptr;
};

}
}

#endif

