/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionLowValuePolicyImpl.C
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

#include "TIDorb/core/ziop/CompressionLowValuePolicyImpl.h"

TIDorb::core::ziop::CompressionLowValuePolicyImpl::CompressionLowValuePolicyImpl(
                                                              CORBA::ULong low_value)
  : m_low_value(low_value)
{
  this->TIDThr::RefCounter::_add_ref();
  destroyed = false;
}


TIDorb::core::ziop::CompressionLowValuePolicyImpl::~CompressionLowValuePolicyImpl() 
  throw (TIDThr::SystemException)
{
}

  
CORBA::ULong TIDorb::core::ziop::CompressionLowValuePolicyImpl::low_value()
{
  return m_low_value;
}
  

CORBA::PolicyType 
TIDorb::core::ziop::CompressionLowValuePolicyImpl::policy_type()
{
  return ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID;
}


CORBA::Policy_ptr 
TIDorb::core::ziop::CompressionLowValuePolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new CompressionLowValuePolicyImpl(m_low_value);
}
    

void TIDorb::core::ziop::CompressionLowValuePolicyImpl::destroy()
{
  destroyed = true;
}
    

TIDorb::core::ziop::CompressionLowValuePolicyImpl* 
TIDorb::core::ziop::CompressionLowValuePolicyImpl::read(
                                          TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::ULong value;
  ZIOP::_CompressionLowValuePolicyValueHelper::read(input, value);
  // TODO: Check value
  return new TIDorb::core::ziop::CompressionLowValuePolicyImpl(value);
}
