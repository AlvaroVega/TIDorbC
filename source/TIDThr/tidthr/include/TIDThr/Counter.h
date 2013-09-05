/*//////////////////////////////////////////////////////////////////////////
//
// File:        Counter.h
// Description: This file contains the Counter class declaration.
//             
// Rel:         01.00
// Created:     September, 2001
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
//////////////////////////////////////////////////////////////////////////*/


#include "TIDThr.h"

#ifndef __TID_THR_COUNTER__
#define __TID_THR_COUNTER__ 

#include <sys/types.h>

namespace TIDThr {

class Counter : public Monitor {
    
  private:
    size_t m_count;
    
  public:
    
    Counter() throw (SystemException) : m_count(0) {}
    
    virtual ~Counter() throw (SystemException) {}
    
    size_t inc() throw (IllegalMutexStateException, SystemException);
    
    size_t dec() throw (IllegalMutexStateException, SystemException);
    
    size_t value() {
      return m_count;
    }
    
    void waitForZero()
      throw (IllegalMutexStateException,
      		 InterruptedException,
             SystemException);

};
} // namespace

#endif
