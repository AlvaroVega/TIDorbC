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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////*/

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
