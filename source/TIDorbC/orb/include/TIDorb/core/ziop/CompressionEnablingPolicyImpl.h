/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionEnablingPolicyImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2007
//
// Revised:
//
// Copyright 2007 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/ziop.h"

#ifndef _TIDORB_CORE_ZIOP_COMPRESSION_ENABLING_POLICY_IMPL_H_
#define _TIDORB_CORE_ZIOP_COMPRESSION_ENABLING_POLICY_IMPL_H_

#include "ZIOP.h"

namespace TIDorb {
namespace core {
namespace ziop {

class CompressionEnablingPolicyImpl: public virtual ZIOP::CompressionEnablingPolicy,
                                     public virtual CORBA::LocalObject,
                                     virtual public TIDThr::RefCounter {
private:

  CORBA::Boolean m_compression_enabled;
  bool destroyed;

public:

  CompressionEnablingPolicyImpl(CORBA::Boolean enabled);

  ~CompressionEnablingPolicyImpl() throw (TIDThr::SystemException);
  
  CORBA::Boolean compression_enabled();
  
  CORBA::PolicyType policy_type();

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
    
  void destroy();
    
  static CompressionEnablingPolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);
};
}// ziop
}// core
}// TIDorb
#endif
