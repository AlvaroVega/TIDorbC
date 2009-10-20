/////////////////////////////////////////////////////////////////////////
//
// File:        HeartbeatPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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

#include "TIDorb/core/HeartbeatPolicyImpl.h"




TIDorb::core::HeartbeatPolicyImpl::HeartbeatPolicyImpl
        (const ::FT::HeartbeatPolicyValue& value)
{
        _value = value;
        destroyed = false;
}




TIDorb::core::HeartbeatPolicyImpl::HeartbeatPolicyImpl
        (CORBA::ULong interval_millis, CORBA::ULong timeout_millis)
{
        _value.heartbeat          = true;
        _value.heartbeat_interval = interval_millis * 10000L;   // unit: 100 nanosecs.
        _value.heartbeat_timeout  = timeout_millis  * 10000L;   // unit: 100 nanosecs.
        destroyed = false;
}




FT::HeartbeatPolicyValue TIDorb::core::HeartbeatPolicyImpl::heartbeat_policy_value()
  throw(CORBA::SystemException)
{
        if (destroyed)
                throw CORBA::OBJECT_NOT_EXIST();

        return _value;
}




CORBA::PolicyType TIDorb::core::HeartbeatPolicyImpl::policy_type()
  throw(CORBA::SystemException)
{
        if (destroyed)
                throw CORBA::OBJECT_NOT_EXIST();

        return FT::HEARTBEAT_POLICY;
}




CORBA::Policy_ptr TIDorb::core::HeartbeatPolicyImpl::copy()
  throw(CORBA::SystemException)
{
        if (destroyed)
                throw CORBA::OBJECT_NOT_EXIST();

        return new HeartbeatPolicyImpl(_value);
}




void TIDorb::core::HeartbeatPolicyImpl::destroy() throw()
{
        destroyed = true;
}
