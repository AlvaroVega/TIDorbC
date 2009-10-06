/*/////////////////////////////////////////////////////////////////////////
//
// File:        HandleT.h
// Description: This file contains the Handle templeate for MT memory management
// Rel:         01.00
// Created:     September, 2001
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es )
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////*/
#include "TIDThr.h"

#ifndef __TID_THR_HANDLE_T_H__
#define __TID_THR_HANDLE_T_H__

namespace TIDThr {

template <class T> 
class HandleT {

  public:

    HandleT(): value(0) {}
     
    HandleT(const HandleT& other) throw (SystemException)
      : value(other.value)
    {
      if(value)
        value->_add_ref();
    }

    HandleT(T* other) throw (SystemException)
      : value(other)
    { 
     if(value)
       value->_add_ref();
    }
  
    ~HandleT() throw (SystemException)
    {
      if(value)
        value->_remove_ref();
    }
  
    HandleT& operator=(const HandleT& other) throw (SystemException)
    {
      assign(other.value);
      return *this;
    }
   
    HandleT& operator=(T* val) throw (SystemException)
    {
      assign(val);
      return *this;
    }
    
    bool is_null() const {return (value == 0);}

    // == operators
    bool operator==(const HandleT& other) const {return (value == other.value);}
    bool operator ==(const T* other) const {return (value == other);}

    // < operators   
    bool operator <(const HandleT& other) const {return (value < other.value);}
    bool operator <(const T* other) const {return (value < other); }
  
     // <= operators   
    bool operator <=(const HandleT& other) const {return (value <= other.value);}
    bool operator <=(const T* other) const {return (value <= other);}

    // > operators   
    bool operator >(const HandleT& other) const {return (value > other.value);}
    bool operator >(const T* other) const {return (value > other);}
  
    // >= operators   
    bool operator >=(const HandleT& other) const {return (value >= other.value);}
    bool operator >=(const T* other) const {return (value >= other);}

    // != operators   
    bool operator !=(const HandleT& other) const {return (value != other.value);}
    bool operator !=(const T* other) const {return (value != other);}
    
    HandleT* operator&() {return this;}
  
    operator T*() const {return value;}
  
    T& operator*() const {return *value;}
  
    T& operator[](int i) const {return value[i];}
  
    T* getT() const {return value;}
  
    T* operator->()const {return value;}

  protected:
  
    void assign(T* val) throw (SystemException) 
    {
      if(value == val)
        return;
     
      if(value)
        value->_remove_ref();
        
      value = val;
      
      if(value)
        value->_add_ref();
    }
    
    T* value;

}; // class

} // namespace

#endif
