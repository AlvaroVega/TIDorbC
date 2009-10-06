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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

