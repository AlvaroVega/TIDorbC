/*/////////////////////////////////////////////////////////////////////////
//
// File:        Condition.h
// Description: This file contains the Condition class declaration.
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

#ifndef __TID_THR_CONDITION_H__
#define __TID_THR_CONDITION_H__

#include <pthread.h>
#include <time.h>
#include <sys/types.h>

namespace TIDThr
{
class Condition : public RefCounter {

  public:

   //Default Constructor: init the condition with the default arguments
    Condition() 
      throw (SystemException);

    // Constructor with condition user arguments 
    Condition(const pthread_condattr_t *attr)
      throw (IllegalArgumentException,
             SystemException);

    //Destructor
    virtual ~Condition() throw (SystemException);

    // valid(): checks if the mutex is valid
    int valid () const { return m_error;}

    void wait(Mutex& mutex) 
      throw (IllegalConditionStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);

    int wait(Mutex& mutex, time_t millisecs)
      throw (IllegalConditionStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);

    int wait(Mutex& mutex, time_t millisecs, time_t nanosecs)
      throw (IllegalConditionStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);


    void wait(RecursiveMutex& mutex) 
      throw (IllegalConditionStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);

    int wait(RecursiveMutex& mutex, time_t millisecs)
      throw (IllegalConditionStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);

    int wait(RecursiveMutex& mutex, time_t millisecs, time_t nanosecs)
      throw (IllegalConditionStateException,
             IllegalArgumentException,
             InterruptedException,
             SystemException);

    void signal()
      throw(IllegalConditionStateException,
            SystemException);

    void broadcast()
      throw(IllegalConditionStateException,
            SystemException);

 
  private:
    // Posix Condition descriptor
    pthread_cond_t m_condition; // condition

    // Error Flag
    int m_error; 
};

} // namespace

#endif
