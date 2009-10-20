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
