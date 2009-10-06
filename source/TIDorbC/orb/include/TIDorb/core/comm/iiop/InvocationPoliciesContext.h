/////////////////////////////////////////////////////////////////////////
//
// File:        InvocationPoliciesContext.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
//
// Revised:
//
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_INVOCATIONPOLICIESCONTEXT_H_
#define _TIDORB_CORE_COMM_IIOP_INVOCATIONPOLICIESCONTEXT_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class InvocationPoliciesContext : public ServiceContext {

public:

  TIDorb::core::PolicyContext* m_policies;

  InvocationPoliciesContext() throw (TIDThr::SystemException);

  InvocationPoliciesContext(TIDorb::core::PolicyContext* policies) 
    throw (TIDThr::SystemException);
 
  ~InvocationPoliciesContext() throw (TIDThr::SystemException);
  
  TIDorb::core::PolicyContext* getPolicies();

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  
  void write(TIDorb::core::cdr::CDROutputStream& output) const;

	
};
} //iiop	
} //comm
} //core
} //TIDorb
#endif

