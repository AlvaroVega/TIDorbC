/////////////////////////////////////////////////////////////////////////
//
// File:        RelativeRequestTimeoutPolicyImpl.C
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

#include "TIDorb/core/messaging/RelativeRequestTimeoutPolicyImpl.h"

TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::RelativeRequestTimeoutPolicyImpl(
                                                                       TimeBase::TimeT val)
{
  destroyed = false;
  m_value = val;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::~RelativeRequestTimeoutPolicyImpl()
  throw (TIDThr::SystemException)
{
}

TimeBase::TimeT 
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::relative_expiry(){
  return m_value;
}

CORBA::PolicyType 
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::policy_type(){
  return Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RelativeRequestTimeoutPolicyImpl(m_value);
};

void TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::destroy(){
  destroyed = true;
}
      
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl* 
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::read(
                                                      TIDorb::core::cdr::CDRInputStream& input)
{
  TimeBase::TimeT policy_value;
  TimeBase::_TimeTHelper::read(input, policy_value);
  return new RelativeRequestTimeoutPolicyImpl(policy_value);        
}
    
