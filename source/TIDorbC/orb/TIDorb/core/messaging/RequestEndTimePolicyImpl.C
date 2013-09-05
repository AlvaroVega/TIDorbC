/////////////////////////////////////////////////////////////////////////
//
// File:        RequestEndTimePolicyImpl.C
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

#include "TIDorb/core/messaging/RequestEndTimePolicyImpl.h"


TIDorb::core::messaging::RequestEndTimePolicyImpl::RequestEndTimePolicyImpl(
                                                                  TimeBase::UtcT time)
{
  destroyed = false;
  endTime = time;
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::messaging::RequestEndTimePolicyImpl::RequestEndTimePolicyImpl(
                                                                    CORBA::ULongLong endTime)
{
  destroyed = false;
  this->endTime.time = endTime;
  this->endTime.inacclo = 0;
  this->endTime.inacchi = 0;
  this->endTime.tdf = 0;
  this->TIDThr::RefCounter::_add_ref();
}

   
TIDorb::core::messaging::RequestEndTimePolicyImpl::~RequestEndTimePolicyImpl()
  throw (TIDThr::SystemException)
{
}


 
TimeBase::UtcT TIDorb::core::messaging::RequestEndTimePolicyImpl::end_time(){
  return this->endTime;
}
    
CORBA::PolicyType TIDorb::core::messaging::RequestEndTimePolicyImpl::policy_type(){
  return Messaging::REQUEST_END_TIME_POLICY_TYPE;
}

CORBA::Policy_ptr TIDorb::core::messaging::RequestEndTimePolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new RequestEndTimePolicyImpl(endTime);
};
    
void TIDorb::core::messaging::RequestEndTimePolicyImpl::destroy(){
  destroyed = true;
}
  

TIDorb::core::messaging::RequestEndTimePolicyImpl*
TIDorb::core::messaging::RequestEndTimePolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{
  TimeBase::UtcT end_time;
  TimeBase::_UtcTHelper::read(input, end_time);
  return new TIDorb::core::messaging::RequestEndTimePolicyImpl(end_time);        
}
