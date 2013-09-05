/////////////////////////////////////////////////////////////////////////
//
// File:        QueueOrderPolicyImpl.C
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

#include "TIDorb/core/messaging/QueueOrderPolicyImpl.h"


/**
 * @param allowedOrders
 */
TIDorb::core::messaging::QueueOrderPolicyImpl::QueueOrderPolicyImpl(
                                                       Messaging::Ordering allowedOrders){
  destroyed = false;
  m_allowedOrders = allowedOrders;
  this->TIDThr::RefCounter::_add_ref();
}
  

TIDorb::core::messaging::QueueOrderPolicyImpl::~QueueOrderPolicyImpl()
  throw (TIDThr::SystemException)
{
}

Messaging::Ordering 
TIDorb::core::messaging::QueueOrderPolicyImpl::allowed_orders(){
  return m_allowedOrders;
}
  
CORBA::PolicyType 
TIDorb::core::messaging::QueueOrderPolicyImpl::policy_type(){
  return Messaging::QUEUE_ORDER_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::QueueOrderPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new QueueOrderPolicyImpl(m_allowedOrders);
};
  
void TIDorb::core::messaging::QueueOrderPolicyImpl::destroy(){
  destroyed = true;
}


