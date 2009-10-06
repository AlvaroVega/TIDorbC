/////////////////////////////////////////////////////////////////////////
//
// File:        RebindPolicyImpl.C
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

#include "TIDorb/core/messaging/RebindPolicyImpl.h"


TIDorb::core::messaging::RebindPolicyImpl::RebindPolicyImpl(Messaging::RebindMode val){
  destroyed = false;
  m_value = val;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RebindPolicyImpl::~RebindPolicyImpl()
  throw (TIDThr::SystemException)
{
}

Messaging::RebindMode TIDorb::core::messaging::RebindPolicyImpl::rebind_mode(){
  return m_value;
}

CORBA::PolicyType TIDorb::core::messaging::RebindPolicyImpl::policy_type(){
  return Messaging::REBIND_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RebindPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RebindPolicyImpl(m_value);
};

void TIDorb::core::messaging::RebindPolicyImpl::destroy(){
  destroyed = true;
}
    
