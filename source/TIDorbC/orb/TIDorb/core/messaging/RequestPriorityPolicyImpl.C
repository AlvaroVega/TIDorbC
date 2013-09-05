/////////////////////////////////////////////////////////////////////////
//
// File:        RequestPriorityPolicyImpl.C
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

#include "TIDorb/core/messaging/RequestPriorityPolicyImpl.h"

/**
 * @param range
 */
TIDorb::core::messaging::RequestPriorityPolicyImpl::RequestPriorityPolicyImpl(
                                                         Messaging::PriorityRange range){
  m_range = range;
  destroyed = false;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RequestPriorityPolicyImpl::~RequestPriorityPolicyImpl()
  throw (TIDThr::SystemException)
{
}

Messaging::PriorityRange 
TIDorb::core::messaging::RequestPriorityPolicyImpl::priority_range(){
  return m_range;
}

CORBA::PolicyType 
TIDorb::core::messaging::RequestPriorityPolicyImpl::policy_type(){
  return Messaging::REQUEST_PRIORITY_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RequestPriorityPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RequestPriorityPolicyImpl(m_range);
};

void TIDorb::core::messaging::RequestPriorityPolicyImpl::destroy(){
  destroyed = true;
}
    
TIDorb::core::messaging::RequestPriorityPolicyImpl* 
TIDorb::core::messaging::RequestPriorityPolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{
  Messaging::PriorityRange policy_value;
  Messaging::_PriorityRangeHelper::read(input, policy_value);
  
  if(policy_value.min > policy_value.max) {
    throw CORBA::MARSHAL();
  }
  
  return new RequestPriorityPolicyImpl(policy_value);        
}
