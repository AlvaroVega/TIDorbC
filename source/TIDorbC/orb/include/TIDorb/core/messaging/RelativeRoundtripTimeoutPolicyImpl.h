/////////////////////////////////////////////////////////////////////////
//
// File:        RelativeRoundtripTimeoutPolicyImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/messaging.h"

#ifndef _TIDORB_CORE_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY_IMPL_H_
#define _TIDORB_CORE_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY_IMPL_H_

namespace TIDorb {
namespace core {
namespace messaging {

class RelativeRoundtripTimeoutPolicyImpl: 
    public virtual Messaging::RelativeRoundtripTimeoutPolicy,
    public virtual CORBA::LocalObject,
    virtual public TIDThr::RefCounter {

private:

  TimeBase::TimeT m_value;

  bool destroyed;

public:

  RelativeRoundtripTimeoutPolicyImpl(TimeBase::TimeT val);

  ~RelativeRoundtripTimeoutPolicyImpl() throw (TIDThr::SystemException);
  
  TimeBase::TimeT relative_expiry();

  CORBA::PolicyType policy_type();

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);

  void destroy();
    

};
}// messaging
}// core
}// TIDorb
#endif
