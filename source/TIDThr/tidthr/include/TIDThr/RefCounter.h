/*/////////////////////////////////////////////////////////////////////////
//
// File:        RefCounter.h
// Description: This file contains the refenence counter base class for memory management
//              See also: TIDThr_HandleT.h
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

#ifndef __TID_THR_REF_COUNTER_H__
#define __TID_THR_REF_COUNTER_H__

#include <pthread.h>
#include <sys/types.h>

namespace TIDThr {

class RefCounter {
  protected:
   // Reference Count. When the reference was 0, the object can be destroyed
    size_t __ref_count_;
    pthread_mutex_t __ref_mutex_;

  public:
    // Constructor
    RefCounter() throw (SystemException);
 
    virtual ~RefCounter() throw (SystemException);
//MLG
    size_t _count()
    {
  	return __ref_count_;
    }

    // add_ref: increments the refs couts
    void _add_ref() throw (SystemException)
    {
  	int ret;

        ret = pthread_mutex_lock(&__ref_mutex_);
        if(ret){
    		throw SystemException("RefCounter::add_ref pthread_mutex_lock: ",ret);
  	}

  	__ref_count_++;

        ret = pthread_mutex_unlock(&__ref_mutex_);
  	if(ret){
    		throw SystemException("RefCounter::add_ref pthread_mutex_unlock: ",ret);
  	}
    }

    // remove_ref decrements the refs count
    // return 0 if the holder can be deleted

    void _remove_ref() throw (SystemException)
    {
  	size_t value;
  	int ret;

        ret = pthread_mutex_lock(&__ref_mutex_);
  	if(ret){
    		throw SystemException("RefCounter::remove_ref pthread_mutex_lock: ",ret);
  	}

  	value = --__ref_count_;

        ret = pthread_mutex_unlock(&__ref_mutex_);
  	if(ret){
    		throw SystemException("RefCounter::remove_ref pthread_mutex_unlock: ",ret);
  	}

  	if(value == 0)
    		delete this;
    }

  protected:
    void _count(size_t v)
    {
  	__ref_count_ = v;
    }
//EMLG
};

}

#endif

