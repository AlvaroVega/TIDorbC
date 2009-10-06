/////////////////////////////////////////////////////////////////////////
//
// File:        RequestStartTimePolicyImpl.h
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

#ifndef _TIDORB_CORE_REQUEST_START_TIME_POLICY_IMPL_H_
#define _TIDORB_CORE_REQUEST_START_TIME_POLICY_IMPL_H_

namespace TIDorb {
namespace core {
namespace messaging {

class RequestStartTimePolicyImpl: public virtual Messaging::RequestStartTimePolicy,
                                  public virtual CORBA::LocalObject,
                                  virtual public TIDThr::RefCounter {

private:

  TimeBase::UtcT startTime;       
  
  bool destroyed;

public:
   

  RequestStartTimePolicyImpl(TimeBase::UtcT time);

  ~RequestStartTimePolicyImpl() throw (TIDThr::SystemException);
  
  TimeBase::UtcT start_time();
  
  CORBA::PolicyType policy_type();

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
    
  void destroy();
    
  static RequestStartTimePolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);
};
}// messaging
}// core
}// TIDorb
#endif
