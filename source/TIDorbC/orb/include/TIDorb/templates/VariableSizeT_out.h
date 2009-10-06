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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
