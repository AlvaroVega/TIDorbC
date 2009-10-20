/*/////////////////////////////////////////////////////////////////////////
//
// File:        InterfaceT_var.h
// Description: InterfaceT_var template
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

#ifndef _TIDORB_templates_InterfaceT_var_H_
#define _TIDORB_templates_InterfaceT_var_H_


namespace TIDorb {
namespace templates {

template<class InterfaceT> class InterfaceT_var
{

#ifdef __GNUC__ 
  template<class T> friend class InterfaceT_out;
#else
  friend class InterfaceT_out<InterfaceT>;
#endif

  public:

    InterfaceT_var() : m_ptr(InterfaceT::_nil()) {}

    InterfaceT_var (InterfaceT* p) : m_ptr (p) {}

    InterfaceT_var(const InterfaceT_var& a)
      : m_ptr(InterfaceT::_duplicate(a.m_ptr)){ }

    ~InterfaceT_var()
    {
      CORBA::release(m_ptr);
    }

    InterfaceT_var& operator=(InterfaceT* p)
    {
      if(m_ptr != p)
      {
        CORBA::release(m_ptr);
        m_ptr = p;
      }  
      
      return *this;
    }

    InterfaceT_var& operator=(const InterfaceT_var& a)
    {
  
      
      if(m_ptr!= a.m_ptr){
        CORBA::release(m_ptr);
        m_ptr = InterfaceT::_duplicate(a.m_ptr);
      }
      
      return *this;
    }

    operator InterfaceT*() const
    {
      return m_ptr;
    }

    operator InterfaceT*&()
    {
      return m_ptr;
    }

    
    InterfaceT* operator->() const
    {
      return m_ptr;
    }

    InterfaceT* in() const
    {
      return m_ptr;
    }

    InterfaceT*& out()
    {
      if(m_ptr != 0) {
        CORBA::release(m_ptr);
      }
      m_ptr = 0;
      return m_ptr;
    }

    InterfaceT*& inout()
    {
      return m_ptr;
    }

    InterfaceT* _retn()
    {
      InterfaceT* tmp = m_ptr;
      m_ptr = 0;
      return tmp;
    }

private:

    InterfaceT* m_ptr;

};

}
}

#endif

