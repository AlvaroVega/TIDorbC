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
