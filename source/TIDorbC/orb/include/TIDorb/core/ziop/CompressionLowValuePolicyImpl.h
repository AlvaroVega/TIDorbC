/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionLowValuePolicyImpl.h
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

#ifndef _TIDORB_CORE_ZIOP_COMPRESSION_LOW_VALUE_POLICY_IMPL_H_
#define _TIDORB_CORE_ZIOP_COMPRESSION_LOW_VALUE_POLICY_IMPL_H_

#include "ZIOP.h"
#include "Compression.h"

namespace TIDorb {
namespace core {
namespace ziop {

class CompressionLowValuePolicyImpl: public virtual ZIOP::CompressionLowValuePolicy,
                                     public virtual CORBA::LocalObject,
                                     virtual public TIDThr::RefCounter {
private:

  CORBA::ULong m_low_value;
  bool destroyed;

public:

  CompressionLowValuePolicyImpl(CORBA::ULong low_value);

  ~CompressionLowValuePolicyImpl() throw (TIDThr::SystemException);
  
  CORBA::ULong low_value();
  
  CORBA::PolicyType policy_type();

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
    
  void destroy();
    
  static CompressionLowValuePolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);
};
}// ziop
}// core
}// TIDorb
#endif
