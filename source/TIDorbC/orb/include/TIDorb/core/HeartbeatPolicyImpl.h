//////////////////////////////////////////////////////////////////////////////////
//
// File:        HeartbeatPolicyImpl.h
// Description: ::FT::HeartbeatPolicy implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_HeartbeatPolicyImpl_H_
#define _TIDORB_CORE_HeartbeatPolicyImpl_H_


/**
 * Implementation of HeartbeatPolicy
 * @version 1.0
*/

#include "CORBA.h"
#include "FT.h"


namespace TIDorb {
namespace core {

class HeartbeatPolicyImpl : public virtual ::FT::HeartbeatPolicy,
                            public virtual CORBA::LocalObject
{
private:
	::FT::HeartbeatPolicyValue _value;
	bool destroyed;
	
public:
  
  HeartbeatPolicyImpl(const ::FT::HeartbeatPolicyValue& value);
  HeartbeatPolicyImpl(CORBA::ULong interval_millis, CORBA::ULong timeout_millis);
  
  ::FT::HeartbeatPolicyValue heartbeat_policy_value() throw (CORBA::SystemException);
  
  CORBA::PolicyType policy_type() throw (CORBA::SystemException);
  
  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
  
  void destroy() throw();

};
}
}
#endif
