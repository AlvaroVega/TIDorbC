/////////////////////////////////////////////////////////////////////////
//
// File:        RequestStartTimePolicyImpl.C
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

#include "TIDorb/core/messaging/RequestStartTimePolicyImpl.h"

/**
 * @param time
 */

TIDorb::core::messaging::RequestStartTimePolicyImpl::RequestStartTimePolicyImpl(
                                                             TimeBase::UtcT time){
  this->TIDThr::RefCounter::_add_ref();
  startTime = time;
  destroyed = false;
}
  
TIDorb::core::messaging::RequestStartTimePolicyImpl::~RequestStartTimePolicyImpl()
  throw (TIDThr::SystemException)
{
}


TimeBase::UtcT TIDorb::core::messaging::RequestStartTimePolicyImpl::start_time(){
  return startTime;
}

CORBA::PolicyType TIDorb::core::messaging::RequestStartTimePolicyImpl::policy_type(){
  return Messaging::REQUEST_START_TIME_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RequestStartTimePolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RequestStartTimePolicyImpl(startTime);
};
    
void TIDorb::core::messaging::RequestStartTimePolicyImpl::destroy(){
  destroyed = true;
}
    

TIDorb::core::messaging::RequestStartTimePolicyImpl*
TIDorb::core::messaging::RequestStartTimePolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{
  TimeBase::UtcT start_time;
  TimeBase::_UtcTHelper::read(input, start_time);
  return new TIDorb::core::messaging::RequestStartTimePolicyImpl(start_time);        
}
