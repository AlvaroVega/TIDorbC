/////////////////////////////////////////////////////////////////////////
//
// File:        EstablishTrustPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     June, 2009
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

#include "TIDorb/core/security/EstablishTrustPolicyImpl.h"



TIDorb::core::security::EstablishTrustPolicyImpl::EstablishTrustPolicyImpl(
                                          Security::EstablishTrust establish_trust)
{
  destroyed = false;
  m_establish_trust = establish_trust;
  this->TIDThr::RefCounter::_add_ref();
}


TIDorb::core::security::EstablishTrustPolicyImpl::~EstablishTrustPolicyImpl() 
  throw (TIDThr::SystemException)
{

}

Security::EstablishTrust TIDorb::core::security::EstablishTrustPolicyImpl::trust()
{
  return m_establish_trust;
}

CORBA::PolicyType TIDorb::core::security::EstablishTrustPolicyImpl::policy_type()
{
  return Security::SecEstablishTrustPolicy;
}

CORBA::Policy_ptr TIDorb::core::security::EstablishTrustPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new EstablishTrustPolicyImpl(m_establish_trust);
}

void TIDorb::core::security::EstablishTrustPolicyImpl::destroy()
{
  destroyed = true;
}
