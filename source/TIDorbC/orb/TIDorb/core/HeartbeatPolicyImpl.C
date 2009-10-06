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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
