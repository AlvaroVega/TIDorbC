/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionMinRatioPolicyImpl.C
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

#include "TIDorb/core/ziop/CompressionMinRatioPolicyImpl.h"

TIDorb::core::ziop::CompressionMinRatioPolicyImpl::CompressionMinRatioPolicyImpl(
                                                ZIOP::CompressionMinRatioPolicyValue ratio)
  : m_ratio(ratio)
{
  this->TIDThr::RefCounter::_add_ref();
  destroyed = false;
}


TIDorb::core::ziop::CompressionMinRatioPolicyImpl::~CompressionMinRatioPolicyImpl() 
  throw (TIDThr::SystemException)
{
}

  
ZIOP::CompressionMinRatioPolicyValue
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::ratio()
{
  return m_ratio;
}
  

CORBA::PolicyType 
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::policy_type()
{
  return ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID;
}


CORBA::Policy_ptr 
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new CompressionMinRatioPolicyImpl(m_ratio);
}
    

void TIDorb::core::ziop::CompressionMinRatioPolicyImpl::destroy()
{
  destroyed = true;
}
    

TIDorb::core::ziop::CompressionMinRatioPolicyImpl* 
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::read(
                                          TIDorb::core::cdr::CDRInputStream& input)
{
  ZIOP::CompressionMinRatioPolicyValue value;
  ZIOP::_CompressionMinRatioPolicyValueHelper::read(input, value);
  // TODO: Check value
  return new TIDorb::core::ziop::CompressionMinRatioPolicyImpl(value);
}
