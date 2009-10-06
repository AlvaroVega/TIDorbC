/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionMinRatioPolicyImpl.h
// Description:
//              
// Rel:         01.00
// Created:     May, 2008
//
// Revised:
//
// Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/ziop.h"

#ifndef _TIDORB_CORE_ZIOP_COMPRESSION_MIN_RATIO_POLICY_IMPL_H_
#define _TIDORB_CORE_ZIOP_COMPRESSION_MIN_RATIO_POLICY_IMPL_H_

#include "ZIOP.h"
#include "Compression.h"

namespace TIDorb {
namespace core {
namespace ziop {

class CompressionMinRatioPolicyImpl: public virtual ZIOP::CompressionMinRatioPolicy,
                                     public virtual CORBA::LocalObject,
                                     virtual public TIDThr::RefCounter {
private:

  ZIOP::CompressionMinRatioPolicyValue m_ratio;
  bool destroyed;

public:

  CompressionMinRatioPolicyImpl(ZIOP::CompressionMinRatioPolicyValue ratio);

  ~CompressionMinRatioPolicyImpl() throw (TIDThr::SystemException);
  
  ZIOP::CompressionMinRatioPolicyValue ratio();
  
  CORBA::PolicyType policy_type();

  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
    
  void destroy();
    
  static CompressionMinRatioPolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);
};
}// ziop
}// core
}// TIDorb
#endif
