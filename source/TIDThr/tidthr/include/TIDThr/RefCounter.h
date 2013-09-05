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

