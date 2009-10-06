/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyManagerImpl.C
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
#include "TIDorb/core/poa/policies.h"




TIDorb::core::PolicyManagerImpl::PolicyManagerImpl(TIDorb::core::TIDORB* orb)
  : _orb(orb)
{
}




CORBA::PolicyList* TIDorb::core::PolicyManagerImpl::get_policy_overrides(
                                                              const CORBA::PolicyTypeSeq& ts)
{
  TIDorb::core::ConfORB& conf = const_cast<TIDorb::core::ConfORB&>(_orb->conf());
  return conf.getPolicyContext().getPolicies(ts);
}




void TIDorb::core::PolicyManagerImpl::set_policy_overrides(const CORBA::PolicyList& policies,
                                                           CORBA::SetOverrideType set_add)
  throw (CORBA::InvalidPolicies)
{
  TIDorb::core::ConfORB& conf = const_cast<TIDorb::core::ConfORB&>(_orb->conf());
  conf.getPolicyContext().setPolicies(policies, set_add);
}
                     


TIDorb::core::PolicyContext& TIDorb::core::PolicyManagerImpl::getPolicyContext()
{
  TIDorb::core::ConfORB& conf = const_cast<TIDorb::core::ConfORB&>(_orb->conf());
  return conf.getPolicyContext();
}
