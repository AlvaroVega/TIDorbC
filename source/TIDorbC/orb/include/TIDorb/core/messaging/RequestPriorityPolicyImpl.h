/////////////////////////////////////////////////////////////////////////
//
// File:        RequestPriorityPolicyImpl.h
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

#ifndef _TIDORB_CORE_REQUEST_PRIORITY_POLICY_IMPL_H_
#define _TIDORB_CORE_REQUEST_PRIORITY_POLICY_IMPL_H_

namespace TIDorb {
namespace core {
namespace messaging {

class RequestPriorityPolicyImpl: public virtual Messaging::RequestPriorityPolicy,
                                 public virtual CORBA::LocalObject,
                                 virtual public TIDThr::RefCounter {

private:

  Messaging::PriorityRange m_range;

  bool destroyed;
 
public:

    /**
     * @param range
     */
  RequestPriorityPolicyImpl(Messaging::PriorityRange range);

  ~RequestPriorityPolicyImpl() throw (TIDThr::SystemException);

  Messaging::PriorityRange priority_range();

  CORBA::PolicyType policy_type();

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);

  void destroy();

  static RequestPriorityPolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);
  
};
}// messaging
}// core
}// TIDorb
#endif
