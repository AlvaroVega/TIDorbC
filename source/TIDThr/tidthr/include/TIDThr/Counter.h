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
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
