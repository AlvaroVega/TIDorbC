/*/////////////////////////////////////////////////////////////////////////
//
// File:        RefCount_var.h
// Description: RefCount_var template
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

#ifndef TIDorb_templates_RefCount_var_H_
#define TIDorb_templates_RefCount_var_H_

namespace TIDorb {
namespace templates {

template <class T> class RefCount_var
{
  public:
    RefCount_var() :m_ptr(0) {}
    RefCount_var(T* p) : m_ptr(p) {}
    
    RefCount_var(const RefCount_var& b) : m_ptr(b.m_ptr)
    {
      if (m_ptr != 0) {
        m_ptr->_add_ref();
      }
    }
    
    ~RefCount_var()
    {
      if (m_ptr != 0) {
        m_ptr->_remove_ref();
      }
    }
    
    RefCount_var& operator=(T* p)
    {
      if (m_ptr != 0) {
        m_ptr->_remove_ref();
      }
      m_ptr = p;
      return *this;
    }
    
    RefCount_var&  operator=(const RefCount_var& b)
    {
      if (m_ptr != b.m_ptr) {
        if (m_ptr != 0) m_ptr->_remove_ref();
        if ((m_ptr = b.m_ptr) != 0)
        m_ptr->_add_ref();
      }
      return *this;
    }

    operator T* () {return m_ptr;}
    operator const T* () const {return (const T*) m_ptr;}

    T* operator->() const {return m_ptr;}
    T* in() const { return m_ptr; }
    T*& inout() { return m_ptr; }
    T*& out()
    {
      if (m_ptr != 0) m_ptr->_remove_ref();
      m_ptr = 0;
      return m_ptr;
    }
    
    T* _retn()
    {
      T* retval = m_ptr;
      m_ptr = 0;
      return retval;
    }
    
  private:
    T* m_ptr;
};

}
}

#endif
