/////////////////////////////////////////////////////////////////////////
//
// File:        QueueOrderPolicyImpl.C
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

#include "TIDorb/core/messaging/QueueOrderPolicyImpl.h"


/**
 * @param allowedOrders
 */
TIDorb::core::messaging::QueueOrderPolicyImpl::QueueOrderPolicyImpl(
                                                       Messaging::Ordering allowedOrders){
  destroyed = false;
  m_allowedOrders = allowedOrders;
  this->TIDThr::RefCounter::_add_ref();
}
  

TIDorb::core::messaging::QueueOrderPolicyImpl::~QueueOrderPolicyImpl()
  throw (TIDThr::SystemException)
{
}

Messaging::Ordering 
TIDorb::core::messaging::QueueOrderPolicyImpl::allowed_orders(){
  return m_allowedOrders;
}
  
CORBA::PolicyType 
TIDorb::core::messaging::QueueOrderPolicyImpl::policy_type(){
  return Messaging::QUEUE_ORDER_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::QueueOrderPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new QueueOrderPolicyImpl(m_allowedOrders);
};
  
void TIDorb::core::messaging::QueueOrderPolicyImpl::destroy(){
  destroyed = true;
}


