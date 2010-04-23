/////////////////////////////////////////////////////////////////////////
//
// File:        QOPPolicyImpl.C
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

#include "TIDorb/core/security/QOPPolicyImpl.h"



TIDorb::core::security::QOPPolicyImpl::QOPPolicyImpl(Security::QOP qop)
{
  destroyed = false;
  m_qop = qop;
  this->TIDThr::RefCounter::_add_ref();
}


TIDorb::core::security::QOPPolicyImpl::~QOPPolicyImpl() 
  throw (TIDThr::SystemException)
{

}

Security::QOP TIDorb::core::security::QOPPolicyImpl::qop()
{
  return m_qop;
}

CORBA::PolicyType TIDorb::core::security::QOPPolicyImpl::policy_type()
{
  return Security::SecQOPPolicy;
}

CORBA::Policy_ptr TIDorb::core::security::QOPPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new QOPPolicyImpl(m_qop);
}

void TIDorb::core::security::QOPPolicyImpl::destroy()
{
  destroyed = true;
}
