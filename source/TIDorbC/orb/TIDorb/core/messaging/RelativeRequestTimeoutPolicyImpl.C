/////////////////////////////////////////////////////////////////////////
//
// File:        RelativeRequestTimeoutPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
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

#include "TIDorb/core/messaging/RelativeRequestTimeoutPolicyImpl.h"

TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::RelativeRequestTimeoutPolicyImpl(
                                                                       TimeBase::TimeT val)
{
  destroyed = false;
  m_value = val;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::~RelativeRequestTimeoutPolicyImpl()
  throw (TIDThr::SystemException)
{
}

TimeBase::TimeT 
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::relative_expiry(){
  return m_value;
}

CORBA::PolicyType 
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::policy_type(){
  return Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RelativeRequestTimeoutPolicyImpl(m_value);
};

void TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::destroy(){
  destroyed = true;
}
      
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl* 
TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl::read(
                                                      TIDorb::core::cdr::CDRInputStream& input)
{
  TimeBase::TimeT policy_value;
  TimeBase::_TimeTHelper::read(input, policy_value);
  return new RelativeRequestTimeoutPolicyImpl(policy_value);        
}
    
