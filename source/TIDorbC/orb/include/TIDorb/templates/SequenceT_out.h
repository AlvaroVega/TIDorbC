/*/////////////////////////////////////////////////////////////////////////
//
// File:        SequenceT_out.h
// Description: SequenceT_out template
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

#ifndef _TIDORB_templates_SequenceT_out_H_
#define _TIDORB_templates_SequenceT_out_H_



namespace TIDorb {
namespace templates {
  
template<typename T, typename ContentT> class SequenceT_out
{
  public:
      
    SequenceT_out(T*& p) : m_ptr(p) 
    { 
      m_ptr = 0; 
    }
    
    SequenceT_out(SequenceT_var<T,ContentT>& p) : m_ptr(p.out()) {}
    
    SequenceT_out(const SequenceT_out& p) : m_ptr(p.m_ptr) {}
    
    SequenceT_out& operator=(const SequenceT_out& p)
    {
      m_ptr = p.m_ptr;
      return *this;
    }
    
    SequenceT_out& operator=(T* p) 
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
    
    ContentT& operator[](CORBA::ULong i) 
    {
      return (*m_ptr)[i];
    }
    
  private:
    
    T*& m_ptr;
    // assignment from SequenceT_var not allowed
    void operator=(const SequenceT_var<T,ContentT>&){}
};
}
}

#endif
