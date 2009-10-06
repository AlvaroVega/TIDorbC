/*/////////////////////////////////////////////////////////////////////////
//
// File:        MutexAttr.h
// Description: This file contains the MutexAttr class declaration.
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

#ifndef __TID_THR_MUTEX_ATTR_H__
#define __TID_THR_MUTEX_ATTR_H__


#include <pthread.h>


namespace TIDThr {


class MutexAttr {

  public:
    MutexAttr() {
      pthread_mutexattr_init(&value);
//#if !defined(__linux__)
      // pthread_mutexattr_settype(&value,PTHREAD_MUTEX_ERRORCHECK);
      pthread_mutexattr_settype(&value,PTHREAD_MUTEX_ERRORCHECK);
//#endif
    };

    ~MutexAttr(){
      pthread_mutexattr_destroy(&value);
    };

    operator pthread_mutexattr_t*() {
      return &value;
    };

  private:
    pthread_mutexattr_t value;
};

} // namespace

#endif
