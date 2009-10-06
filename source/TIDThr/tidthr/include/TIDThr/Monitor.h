/*/////////////////////////////////////////////////////////////////////////
//
// File:        TMonitor.h
// Description: This file contains the Monitor class declaration.
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDThr.h"

#ifndef __TID_THR_MONITOR__
#define __TID_THR_MONITOR__ 

#include <time.h>

namespace TIDThr {

class Monitor: public RecursiveMutex {

  public:

    Monitor(const pthread_condattr_t *cond_attr = NULL)
      throw (SystemException);
             
    virtual ~Monitor() throw (SystemException) {}

    void wait()
      throw (IllegalMonitorStateException,
             InterruptedException,
             SystemException);

    void wait(time_t millisecons)
      throw (IllegalMonitorStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);

    void wait(time_t millisecons, time_t nanos)
     throw (IllegalMonitorStateException,
            IllegalArgumentException,
            InterruptedException,
            SystemException);

    void notify()
     throw (IllegalMonitorStateException,
            SystemException);

    void notifyAll()
     throw (IllegalMonitorStateException,
            SystemException);
    
  private:
    Condition  m_condition;
};


}

#endif

