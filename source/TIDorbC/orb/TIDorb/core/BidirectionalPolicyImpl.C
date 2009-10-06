/////////////////////////////////////////////////////////////////////////
//
// File:        BidirectionalPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/BidirectionalPolicyImpl.h"
  
TIDorb::core::BidirectionalPolicyImpl::BidirectionalPolicyImpl(BiDirPolicy::BidirectionalPolicyValue value) {
	_value = value;
	destroyed = false;
};
    
BiDirPolicy::BidirectionalPolicyValue TIDorb::core::BidirectionalPolicyImpl::value() throw(CORBA::SystemException)
{
	if (destroyed) 
		throw CORBA::OBJECT_NOT_EXIST();

    	return _value;
};
  
CORBA::PolicyType TIDorb::core::BidirectionalPolicyImpl::policy_type() throw(CORBA::SystemException)
{
	if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();

  return BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE;
};
  
CORBA::Policy_ptr TIDorb::core::BidirectionalPolicyImpl::copy() throw(CORBA::SystemException)
{
	if (destroyed) 
  		throw CORBA::OBJECT_NOT_EXIST();

    	return new BidirectionalPolicyImpl(_value);
};
  
void TIDorb::core::BidirectionalPolicyImpl::destroy() throw()
{
	destroyed = true;
};

TIDorb::core::BidirectionalPolicyImpl* 
TIDorb::core::BidirectionalPolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{

  BiDirPolicy::BidirectionalPolicyValue value;
  BiDirPolicy::_BidirectionalPolicyValueHelper::read(input, value);

  if ((value != BiDirPolicy::BOTH) && 
      (value != BiDirPolicy::NORMAL)) {
    throw CORBA::MARSHAL();
  }
  
  return new TIDorb::core::BidirectionalPolicyImpl(value);        
}
