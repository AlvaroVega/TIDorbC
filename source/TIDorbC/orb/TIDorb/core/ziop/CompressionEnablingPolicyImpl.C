/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionEnablingPolicyImpl.C
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

#include "TIDorb/core/ziop/CompressionEnablingPolicyImpl.h"



TIDorb::core::ziop::CompressionEnablingPolicyImpl::CompressionEnablingPolicyImpl(
                                                     CORBA::Boolean enabled)
  : m_compression_enabled(enabled)
{
  this->TIDThr::RefCounter::_add_ref();
  destroyed = false;
}

TIDorb::core::ziop::CompressionEnablingPolicyImpl::~CompressionEnablingPolicyImpl() 
  throw (TIDThr::SystemException)
{

}
  

CORBA::Boolean
TIDorb::core::ziop::CompressionEnablingPolicyImpl::compression_enabled()
{
  return m_compression_enabled;
}


CORBA::PolicyType 
TIDorb::core::ziop::CompressionEnablingPolicyImpl::policy_type()
{
  return ZIOP::COMPRESSION_ENABLING_POLICY_ID;
}

CORBA::Policy_ptr 
TIDorb::core::ziop::CompressionEnablingPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();

  return new CompressionEnablingPolicyImpl(m_compression_enabled);
}

void TIDorb::core::ziop::CompressionEnablingPolicyImpl::destroy()
{
  destroyed = true;
}

TIDorb::core::ziop::CompressionEnablingPolicyImpl* 
TIDorb::core::ziop::CompressionEnablingPolicyImpl::read(
                                         TIDorb::core::cdr::CDRInputStream& input)
{
  ZIOP::CompressionEnablingPolicyValue value;
  // TODO: Check value
  ZIOP::_CompressionEnablingPolicyValueHelper::read(input, value);
  return new CompressionEnablingPolicyImpl(value);
}
