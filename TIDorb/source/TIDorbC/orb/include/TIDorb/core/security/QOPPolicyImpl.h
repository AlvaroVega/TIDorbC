/////////////////////////////////////////////////////////////////////////
//
// File:        QOPPolicyImpl.h
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

#ifndef _TIDORB_CORE_SECURITY_QOP_POLICY_IMPL_H_
#define _TIDORB_CORE_SECURITY_QOP_POLICY_IMPL_H_

#include "CORBA.h"
#include "SecurityLevel2.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace security {

class QOPPolicyImpl: public virtual SecurityLevel2::QOPPolicy,
                     public virtual CORBA::LocalObject,
                     virtual public TIDThr::RefCounter 
{

private:

  Security::QOP m_qop;
  
  bool destroyed;

public:
  
  QOPPolicyImpl(Security::QOP qop);
  
  ~QOPPolicyImpl() throw (TIDThr::SystemException);
  
  Security::QOP qop();
  
  CORBA::PolicyType policy_type();
  
  CORBA::Policy_ptr copy() throw(CORBA::SystemException);

  void destroy();

  
};

}// security
}// core
}// TIDorb
#endif

