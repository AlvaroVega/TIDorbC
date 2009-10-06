/*/////////////////////////////////////////////////////////////////////////
//
// File:        FixedSizeT_var.h
// Description: FixedSizeT_var template
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

#ifndef _TIDORB_templates_FixedSizeT_var_H_
#define _TIDORB_templates_FixedSizeT_var_H_


namespace TIDorb {
namespace templates {
  
// C++
template<typename T> class FixedSizeT_var {
  public:
    FixedSizeT_var() : m_ptr(0) {}
    FixedSizeT_var(T *t) : m_ptr(t) {}
    FixedSizeT_var(const T& t) : m_ptr(new T(t)) {}
    
    FixedSizeT_var(const FixedSizeT_var &t) : m_ptr(0) 
    {
      if (t.m_ptr != 0)
        m_ptr = new T(*t.m_ptr);
    }
    
    ~FixedSizeT_var() { delete m_ptr; }
    
    FixedSizeT_var &operator=(T *t) 
    {
      if (t != m_ptr) {
        delete m_ptr;
        m_ptr = t;
      }
      return *this;
    }
    
    FixedSizeT_var &operator=(const T& t) 
    {
      if (&t != m_ptr) {
        delete m_ptr;
        m_ptr = new T(t);
      }
    
      return *this;
    }
    
    FixedSizeT_var &operator=(const FixedSizeT_var &t) 
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
    
    T& out() {
        if (m_ptr == 0)
            m_ptr = new T;
        return *m_ptr;
    }
    
    T _retn() {return *m_ptr;}
  
  private:
    T* m_ptr;
};

}
}

#endif

