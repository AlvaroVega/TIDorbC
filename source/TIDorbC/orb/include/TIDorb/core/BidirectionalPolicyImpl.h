//////////////////////////////////////////////////////////////////////////////////
//
// File:        BidirectionalPolicyImpl.h
// Description: BiDirPolicy::BidirectionalPolicy implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_BidirectionalPolicyImpl_H_
#define _TIDORB_CORE_BidirectionalPolicyImpl_H_


/**
 * Implementation of BidirectionalPolicy
 * @version 1.0
*/
namespace TIDorb {
namespace core {

class BidirectionalPolicyImpl : public BiDirPolicy::BidirectionalPolicy,
                                public CORBA::LocalObject
{
private:

	BiDirPolicy::BidirectionalPolicyValue _value;
	bool destroyed;
	
public:
  
  BidirectionalPolicyImpl(BiDirPolicy::BidirectionalPolicyValue value);
  
  //CORBA::Short value();

  BiDirPolicy::BidirectionalPolicyValue value() throw (CORBA::SystemException);
  
  //CORBA::Long policy_type();
  
  CORBA::PolicyType policy_type() throw (CORBA::SystemException);
  
  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
  
  void destroy() throw();
  
  static BidirectionalPolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);

};
}
}
#endif
