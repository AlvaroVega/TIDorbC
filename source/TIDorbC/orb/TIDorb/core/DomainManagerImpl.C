/////////////////////////////////////////////////////////////////////////
//
// File:        DomainManagerImpl.C
// Description:
//              
// Rel:         01.00
// Created:     September, 2009
//
// Revised:    
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights
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




TIDorb::core::DomainManagerImpl::DomainManagerImpl(TIDorb::core::TIDORB* orb)
  : _orb(orb)
{
  
}

TIDorb::core::DomainManagerImpl::~DomainManagerImpl() throw (TIDThr::SystemException)
{
  
}

CORBA::Policy_ptr 
TIDorb::core::DomainManagerImpl::get_domain_policy(CORBA::PolicyType policy_type)
{
  PolicyTableT::iterator it = m_policy_table.find(policy_type);
  
  if (it != m_policy_table.end()) 
    return (*it).second;
  else {
    TIDorb::util::StringBuffer msg;
    msg << "Policy type not found in this DomainManager";
    throw CORBA::INV_POLICY(msg.str().data());
  }      
}


void TIDorb::core::DomainManagerImpl::set_domain_policy(CORBA::Policy_ptr policy)
{
  // TODO: check if the policy is going to be overwrite
  CORBA::PolicyType type = policy->policy_type();

  PolicyTableT::iterator it = m_policy_table.find(type);
  if (it != m_policy_table.end()) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Policy " << type << " overwritten in DomainManager";
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }
  }

  m_policy_table[type] = policy;
}
