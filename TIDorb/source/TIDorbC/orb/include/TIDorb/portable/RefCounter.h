/*/////////////////////////////////////////////////////////////////////////
//
// File:        RefCounter.h
// Description: This file contains the refenence counter base class for memory management
//              
// Rel:         01.00
// Created:     November, 2002
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

#include "TIDorb/portable.h"

#ifndef _TIDORB_PORTABLE_REFCOUNTER_H_
#define _TIDORB_PORTABLE_REFCOUNTER_H_

#include <pthread.h>
#include <sys/types.h>

namespace TIDorb {
namespace portable {

class RefCounter {  

  public:
    // Constructor
    RefCounter();
 
    virtual ~RefCounter();

    CORBA::ULong  _refcount_value()
    {
  		return __ref_count_;
    }

    // add_ref: increments the refs couts
    virtual void _add_ref();
    


    // remove_ref decrements the refs count
    // return 0 if the holder can be deleted

    virtual void _remove_ref();
    

  protected:
    void _count(CORBA::ULong v)
    {
  		__ref_count_ = v;
    }

  private:
   // Reference Count. When the reference was 0, the object can be destroyed
    CORBA::ULong __ref_count_;
    pthread_mutex_t __ref_mutex_;
};

} // portable
} // TIDorb

#endif
