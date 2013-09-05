/////////////////////////////////////////////////////////////////////////
//
// File:        RebindPolicyImpl.C
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

#include "TIDorb/core/messaging/RebindPolicyImpl.h"


TIDorb::core::messaging::RebindPolicyImpl::RebindPolicyImpl(Messaging::RebindMode val){
  destroyed = false;
  m_value = val;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RebindPolicyImpl::~RebindPolicyImpl()
  throw (TIDThr::SystemException)
{
}

Messaging::RebindMode TIDorb::core::messaging::RebindPolicyImpl::rebind_mode(){
  return m_value;
}

CORBA::PolicyType TIDorb::core::messaging::RebindPolicyImpl::policy_type(){
  return Messaging::REBIND_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RebindPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RebindPolicyImpl(m_value);
};

void TIDorb::core::messaging::RebindPolicyImpl::destroy(){
  destroyed = true;
}
    
