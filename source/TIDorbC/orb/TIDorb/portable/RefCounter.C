/////////////////////////////////////////////////////////////////////////
//
// File:        RefCounter.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "CORBA.h"

namespace TIDorb {
namespace portable {
    
RefCounter::RefCounter()
{ 
  __ref_count_ = 1;
  
  int ret = pthread_mutex_init(&__ref_mutex_,NULL);
  
  if(ret)
    throw CORBA::NO_RESOURCES();
}
  

RefCounter::~RefCounter()
{
  pthread_mutex_destroy(&__ref_mutex_);
}

// add_ref: increments the refs couts
void RefCounter::_add_ref()
{
    int ret;

    ret = pthread_mutex_lock(&__ref_mutex_);
    if(ret)
        throw CORBA::UNKNOWN();     

    __ref_count_++;

    ret = pthread_mutex_unlock(&__ref_mutex_);
    if(ret)
        throw CORBA::UNKNOWN();     
}


// remove_ref decrements the refs count
// return 0 if the holder can be deleted

void RefCounter::_remove_ref()
{
    CORBA::ULong value;
    int ret;

    ret = pthread_mutex_lock(&__ref_mutex_);
    if(ret)
        throw CORBA::UNKNOWN();

        
    value = --__ref_count_;

    ret = pthread_mutex_unlock(&__ref_mutex_);
    if(ret)
        throw CORBA::UNKNOWN();         

    if(value == 0)
            delete this;
}

} // portable
} // TIDorb


