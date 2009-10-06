/////////////////////////////////////////////////////////////////////////
//
// File:        RequestPriorityPolicyImpl.C
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

#include "TIDorb/core/messaging/RequestPriorityPolicyImpl.h"

/**
 * @param range
 */
TIDorb::core::messaging::RequestPriorityPolicyImpl::RequestPriorityPolicyImpl(
                                                         Messaging::PriorityRange range){
  m_range = range;
  destroyed = false;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RequestPriorityPolicyImpl::~RequestPriorityPolicyImpl()
  throw (TIDThr::SystemException)
{
}

Messaging::PriorityRange 
TIDorb::core::messaging::RequestPriorityPolicyImpl::priority_range(){
  return m_range;
}

CORBA::PolicyType 
TIDorb::core::messaging::RequestPriorityPolicyImpl::policy_type(){
  return Messaging::REQUEST_PRIORITY_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RequestPriorityPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RequestPriorityPolicyImpl(m_range);
};

void TIDorb::core::messaging::RequestPriorityPolicyImpl::destroy(){
  destroyed = true;
}
    
TIDorb::core::messaging::RequestPriorityPolicyImpl* 
TIDorb::core::messaging::RequestPriorityPolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{
  Messaging::PriorityRange policy_value;
  Messaging::_PriorityRangeHelper::read(input, policy_value);
  
  if(policy_value.min > policy_value.max) {
    throw CORBA::MARSHAL();
  }
  
  return new RequestPriorityPolicyImpl(policy_value);        
}
