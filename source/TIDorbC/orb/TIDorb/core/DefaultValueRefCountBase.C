/////////////////////////////////////////////////////////////////////////
//
// File:        DefaultValueRefCountBase.C
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

namespace CORBA {


DefaultValueRefCountBase::DefaultValueRefCountBase()
{ 
  __ref_count_ = 1;

  int ret = pthread_mutex_init(&__ref_mutex_,NULL);

  if(ret)
    throw CORBA::NO_RESOURCES();
}

DefaultValueRefCountBase::~DefaultValueRefCountBase()
{
  pthread_mutex_destroy(&__ref_mutex_);
}

CORBA::ULong DefaultValueRefCountBase::_refcount_value()
{
  return __ref_count_;
}

void DefaultValueRefCountBase::_count(CORBA::ULong v)
{
  __ref_count_ = v;
}

ValueBase* DefaultValueRefCountBase::_add_ref()
{
  int ret;
  
  if(ret = pthread_mutex_lock(&__ref_mutex_))
    throw CORBA::UNKNOWN();
  
  __ref_count_++;
  
  if(ret = pthread_mutex_unlock(&__ref_mutex_)) 
    throw CORBA::UNKNOWN();

  return this;
}

void DefaultValueRefCountBase::_remove_ref()
{
  CORBA::ULong value;
  int ret;
  
  if(ret = pthread_mutex_lock(&__ref_mutex_))
    throw CORBA::UNKNOWN();

  value = --__ref_count_;

  if(ret = pthread_mutex_unlock(&__ref_mutex_)) 
    throw CORBA::UNKNOWN();

  if(value == 0)
    delete this;
}

ValueBase* DefaultValueRefCountBase::_copy_value()
{
  throw CORBA::NO_IMPLEMENT();
}

} // namespace CORBA	
