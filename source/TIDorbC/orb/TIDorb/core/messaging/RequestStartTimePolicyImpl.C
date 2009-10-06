/////////////////////////////////////////////////////////////////////////
//
// File:        RequestStartTimePolicyImpl.C
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

#include "TIDorb/core/messaging/RequestStartTimePolicyImpl.h"

/**
 * @param time
 */

TIDorb::core::messaging::RequestStartTimePolicyImpl::RequestStartTimePolicyImpl(
                                                             TimeBase::UtcT time){
  this->TIDThr::RefCounter::_add_ref();
  startTime = time;
  destroyed = false;
}
  
TIDorb::core::messaging::RequestStartTimePolicyImpl::~RequestStartTimePolicyImpl()
  throw (TIDThr::SystemException)
{
}


TimeBase::UtcT TIDorb::core::messaging::RequestStartTimePolicyImpl::start_time(){
  return startTime;
}

CORBA::PolicyType TIDorb::core::messaging::RequestStartTimePolicyImpl::policy_type(){
  return Messaging::REQUEST_START_TIME_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RequestStartTimePolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RequestStartTimePolicyImpl(startTime);
};
    
void TIDorb::core::messaging::RequestStartTimePolicyImpl::destroy(){
  destroyed = true;
}
    

TIDorb::core::messaging::RequestStartTimePolicyImpl*
TIDorb::core::messaging::RequestStartTimePolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{
  TimeBase::UtcT start_time;
  TimeBase::_UtcTHelper::read(input, start_time);
  return new TIDorb::core::messaging::RequestStartTimePolicyImpl(start_time);        
}
