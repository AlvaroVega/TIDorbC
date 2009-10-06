/////////////////////////////////////////////////////////////////////////
//
// File:        HeartbeatEnablePolicyImpl.C
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

#include "TIDorb/core/HeartbeatEnabledPolicyImpl.h"




TIDorb::core::HeartbeatEnabledPolicyImpl::HeartbeatEnabledPolicyImpl(CORBA::Boolean value)
{
        _value = value;
        destroyed = false;
}




CORBA::Boolean TIDorb::core::HeartbeatEnabledPolicyImpl::heartbeat_enabled_policy_value()
  throw(CORBA::SystemException)
{
        if (destroyed)
                throw CORBA::OBJECT_NOT_EXIST();

        return _value;
}




CORBA::PolicyType TIDorb::core::HeartbeatEnabledPolicyImpl::policy_type()
  throw(CORBA::SystemException)
{
        if (destroyed)
                throw CORBA::OBJECT_NOT_EXIST();

        return FT::HEARTBEAT_ENABLED_POLICY;
}




CORBA::Policy_ptr TIDorb::core::HeartbeatEnabledPolicyImpl::copy()
  throw(CORBA::SystemException)
{
        if (destroyed)
                throw CORBA::OBJECT_NOT_EXIST();

        return new HeartbeatEnabledPolicyImpl(_value);
}




void TIDorb::core::HeartbeatEnabledPolicyImpl::destroy() throw()
{
        destroyed = true;
}
