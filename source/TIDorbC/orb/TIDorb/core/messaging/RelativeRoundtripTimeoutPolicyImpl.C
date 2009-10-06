/////////////////////////////////////////////////////////////////////////
//
// File:        RelativeRoundtripTimeoutPolicyImpl.C
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

#include "TIDorb/core/messaging/RelativeRoundtripTimeoutPolicyImpl.h"


TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::RelativeRoundtripTimeoutPolicyImpl(
                                                                             TimeBase::TimeT val){
  destroyed = false;
  m_value = val;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::~RelativeRoundtripTimeoutPolicyImpl() throw (TIDThr::SystemException){
}
  
TimeBase::TimeT TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::relative_expiry(){
  return m_value;
}

CORBA::PolicyType TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::policy_type(){
  return Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RelativeRoundtripTimeoutPolicyImpl(m_value);
};

void TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::destroy(){
  destroyed = true;
}
    

