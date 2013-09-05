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

