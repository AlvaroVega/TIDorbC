/*/////////////////////////////////////////////////////////////////////////
//
// File:        ValuetypeT_out.h
// Description: ValueTypeT_out template
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

#ifndef _TIDORB_TEMPLATES_VALUETYPE_OUT_H_
#define _TIDORB_TEMPLATES_VALUETYPE_OUT_H_

namespace TIDorb {
namespace templates {
  
template<class T> class ValuetypeT_out
{
  public:
    ValuetypeT_out(T*& p) : m_ptr(p) 
    { 
      m_ptr = 0; 
    }
    
    ValuetypeT_out(ValuetypeT_var<T>& p) : m_ptr(p.m_ptr) 
    {
      if (m_ptr != 0) {
        m_ptr->_remove_ref();
      }
      m_ptr = 0;
    }
    
    ValuetypeT_out(const ValuetypeT_out& p) : m_ptr(p.m_ptr) {}

    ValuetypeT_out& operator=(const ValuetypeT_out& p)
    {
      m_ptr = p.m_ptr;
      return *this;
    }
    
    ValuetypeT_out& operator=(T* p) 
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
    void operator=(const ValuetypeT_var<T>&){}
};

}
}

#endif

