/////////////////////////////////////////////////////////////////////////
//
// File:        RelativeRoundtripTimeoutPolicyImpl.C
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

#include "TIDorb/core/messaging/RelativeRoundtripTimeoutPolicyImpl.h"


TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::RelativeRoundtripTimeoutPolicyImpl(
                                                                             TimeBase::TimeT val){
  destroyed = false;
  m_value = val;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::~RelativeRoundtripTimeoutPolicyImpl() throw (TIDThr::SystemException){
}
  
TimeBase::TimeT TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::relative_expiry(){
  return m_value;
}

CORBA::PolicyType TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::policy_type(){
  return Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RelativeRoundtripTimeoutPolicyImpl(m_value);
};

void TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl::destroy(){
  destroyed = true;
}
    

