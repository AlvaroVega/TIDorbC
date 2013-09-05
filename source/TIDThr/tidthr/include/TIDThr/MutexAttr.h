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
