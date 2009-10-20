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
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
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
