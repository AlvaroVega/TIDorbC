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
