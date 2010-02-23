/*/////////////////////////////////////////////////////////////////////////
//
// File:        SequenceT_out.h
// Description: SequenceT_var template
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

#ifndef _TIDORB_templates_SequenceT_var_H_
#define _TIDORB_templates_SequenceT_var_H_


namespace TIDorb {
namespace templates {
  
// C++
template<typename T, typename ContentT > class SequenceT_var {
  public:
    SequenceT_var() : m_ptr(0) {}
    SequenceT_var(T *t) : m_ptr(t) {}
    SequenceT_var(const T& t) : m_ptr(new T(t)) {}
    
    SequenceT_var(const SequenceT_var &t) : m_ptr(0) 
    {
      if (t.m_ptr != 0)
        m_ptr = new T(*t.m_ptr);
    }
    
    ~SequenceT_var() { delete m_ptr; }
    
    SequenceT_var &operator=(T *t) 
    {
      if (t != m_ptr) {
        delete m_ptr;
        m_ptr = t;
      }
      return *this;
    }
    
    SequenceT_var &operator=(const T& t) 
    {
      if (&t != m_ptr) {
        delete m_ptr;
        m_ptr = new T(t);
      }
    
      return *this;
    }
    
    SequenceT_var &operator=(const SequenceT_var &t) 
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
      return * ((const T*) m_ptr);
    }
    
    operator T& ()
    {
      return *m_ptr;
    }
    
    ContentT& operator[](CORBA::ULong i) 
    {
      return (*m_ptr)[i];
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

