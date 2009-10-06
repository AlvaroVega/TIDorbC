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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
