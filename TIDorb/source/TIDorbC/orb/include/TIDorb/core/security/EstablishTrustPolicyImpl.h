/////////////////////////////////////////////////////////////////////////
//
// File:        EstablishTrustPolicyImpl.h
// Description:
//              
// Rel:         01.00
// Created:     June, 2009
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security.h"

#ifndef _TIDORB_CORE_SECURITY_ESTABLISH_TRUST_POLICY_IMPL_H_
#define _TIDORB_CORE_SECURITY_ESTABLISH_TRUST_POLICY_IMPL_H_

#include "CORBA.h"
#include "SecurityLevel2.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace security {

class EstablishTrustPolicyImpl: 
    public virtual SecurityLevel2::EstablishTrustPolicy,
    public virtual CORBA::LocalObject,
    virtual public TIDThr::RefCounter 
{

private:

  Security::EstablishTrust m_establish_trust;
  
  bool destroyed;

public:
  
  EstablishTrustPolicyImpl(Security::EstablishTrust establish_trust);
  
  ~EstablishTrustPolicyImpl() throw (TIDThr::SystemException);
  
  Security::EstablishTrust trust();
  
  CORBA::PolicyType policy_type();
  
  CORBA::Policy_ptr copy() throw(CORBA::SystemException);

  void destroy();

  
};

}// security
}// core
}// TIDorb
#endif

