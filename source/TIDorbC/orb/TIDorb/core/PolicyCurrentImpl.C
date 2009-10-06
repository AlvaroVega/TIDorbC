/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyCurrentImpl.C
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

#include "TIDorb/core.h"


TIDorb::core::PolicyCurrentImpl::PolicyCurrentImpl(
                                       TIDorb::core::PolicyContextManagerImpl* manager)
  : m_manager(manager)
{

}

TIDorb::core::PolicyCurrentImpl:: ~PolicyCurrentImpl()
{

}
  
CORBA::PolicyList* 
TIDorb::core::PolicyCurrentImpl::get_policy_overrides(const CORBA::PolicyTypeSeq& ts)
{
  TIDorb::core::PolicyContext* context = 
    m_manager->tryToGetThreadContext(TIDThr::Thread::currentThread());
  
  if (context == NULL)
    return NULL;
  
  return context->getPolicies(ts);
}
  

void TIDorb::core::PolicyCurrentImpl::set_policy_overrides(
                                                const CORBA::PolicyList& policies,
                                                CORBA::SetOverrideType set_add)
  throw (CORBA::InvalidPolicies)
{
  TIDorb::core::PolicyContext* context =
    m_manager->getThreadContext(TIDThr::Thread::currentThread());
  
  context->setPolicies(policies, set_add);
}
  

TIDorb::core::PolicyContext* TIDorb::core::PolicyCurrentImpl::getPolicyContext()
{
  return m_manager->tryToGetThreadContext(TIDThr::Thread::currentThread());
}
