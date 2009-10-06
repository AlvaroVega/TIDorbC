/////////////////////////////////////////////////////////////////////////
//
// File:        RequestEndTimePolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/messaging/RequestEndTimePolicyImpl.h"


TIDorb::core::messaging::RequestEndTimePolicyImpl::RequestEndTimePolicyImpl(
                                                                  TimeBase::UtcT time)
{
  destroyed = false;
  endTime = time;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RequestEndTimePolicyImpl::RequestEndTimePolicyImpl(
                                                                    CORBA::ULongLong endTime)
{
  destroyed = false;
  this->endTime.time = endTime;
  this->endTime.inacclo = 0;
  this->endTime.inacchi = 0;
  this->endTime.tdf = 0;
  this->TIDThr::RefCounter::_add_ref();
}

   
TIDorb::core::messaging::RequestEndTimePolicyImpl::~RequestEndTimePolicyImpl()
  throw (TIDThr::SystemException)
{
}


 
TimeBase::UtcT TIDorb::core::messaging::RequestEndTimePolicyImpl::end_time(){
  return this->endTime;
}
    
CORBA::PolicyType TIDorb::core::messaging::RequestEndTimePolicyImpl::policy_type(){
  return Messaging::REQUEST_END_TIME_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RequestEndTimePolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RequestEndTimePolicyImpl(endTime);
};
    
void TIDorb::core::messaging::RequestEndTimePolicyImpl::destroy(){
  destroyed = true;
}
  

TIDorb::core::messaging::RequestEndTimePolicyImpl*
TIDorb::core::messaging::RequestEndTimePolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{
  TimeBase::UtcT end_time;
  TimeBase::_UtcTHelper::read(input, end_time);
  return new TIDorb::core::messaging::RequestEndTimePolicyImpl(end_time);        
}
