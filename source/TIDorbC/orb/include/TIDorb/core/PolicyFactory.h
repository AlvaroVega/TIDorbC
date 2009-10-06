//////////////////////////////////////////////////////////////////////////////////
//
// File:        PolicyFactory.h
// Description: PolicyFactory implementation.
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

#ifndef _TIDORB_CORE_POLICY_FACTORY_H
#define _TIDORB_CORE_POLICY_FACTORY_H

#include "CORBA.h"
#include "PortableServer.h"
#include "BiDirPolicy.h"
#include "FT.h"
#include "Messaging.h"
#include "ZIOP.h"

namespace TIDorb {
namespace core {

class PolicyFactory {
  public:
    /**
     * Attempts to instanciate a <B>CORBA</B> policy.
     * @param type the CORBA policy identifier
     * @param val the policy value inserted in a <code>Any</code>
     * @throws CORBA::PolicyError
     */

    static CORBA::Policy_ptr create_policy(CORBA::PolicyType type, const CORBA::Any& val)
      throw (CORBA::PolicyError);

    static void validate_for_orb(const CORBA::PolicyType policy)
      throw (CORBA::PolicyError);

    static CORBA::Policy_ptr readPolicy(TIDorb::core::cdr::CDRInputStream& input);

  protected:
    static TIDorb::core::poa::policies::ThreadPolicyImpl* 
      create_ThreadPolicy(const CORBA::Any& val)
        throw (CORBA::PolicyError);
    static TIDorb::core::poa::policies::LifespanPolicyImpl* 
      create_LifespanPolicy(const CORBA::Any& val)
                throw (CORBA::PolicyError);
    static TIDorb::core::poa::policies::IdUniquenessPolicyImpl* 
      create_IdUniquenessPolicy(const CORBA::Any& val)
                throw (CORBA::PolicyError);
    static TIDorb::core::poa::policies::IdAssignmentPolicyImpl* 
      create_IdAssignmentPolicy(const CORBA::Any& val)
                throw (CORBA::PolicyError);
    static TIDorb::core::poa::policies::ServantRetentionPolicyImpl* 
      create_ServantRetentionPolicy(const CORBA::Any& val)
                throw (CORBA::PolicyError) ;
    static TIDorb::core::poa::policies::RequestProcessingPolicyImpl* 
      create_RequestProcessingPolicy(const CORBA::Any& val)
                throw (CORBA::PolicyError);
    static BidirectionalPolicyImpl* 
      create_BidirectionalPolicy(const CORBA::Any& val)
                throw(CORBA::PolicyError);

    // FT extensions
    static HeartbeatPolicyImpl* 
      create_HeartbeatPolicy(const CORBA::Any& val)
                throw(CORBA::PolicyError);
    static HeartbeatEnabledPolicyImpl* 
      create_HeartbeatEnabledPolicy(const CORBA::Any& val)
                throw(CORBA::PolicyError);

    // Qos extensions
    static TIDorb::core::messaging::QueueOrderPolicyImpl* 
      create_QueueOrderPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::messaging::RebindPolicyImpl* 
      create_RebindPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError); 

    static TIDorb::core::messaging::RelativeRequestTimeoutPolicyImpl* 
      create_RelativeRequestTimeoutPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl* 
      create_RelativeRoundtripTimeoutPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::messaging::RequestEndTimePolicyImpl* 
      create_RequestEndTimePolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::messaging::RequestPriorityPolicyImpl* 
      create_RequestPriorityPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);
      
    static TIDorb::core::messaging::RequestStartTimePolicyImpl* 
      create_RequestStartTimePolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    // ZIOP extensions
    static TIDorb::core::ziop::CompressionEnablingPolicyImpl*
      create_CompressionEnablingPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::ziop::CompressorIdLevelListPolicyImpl*
      create_CompressorIdLevelListPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::ziop::CompressionLowValuePolicyImpl*
      create_CompressionLowValuePolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::ziop::CompressionMinRatioPolicyImpl*
      create_CompressionMinRatioPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

};

}
}

#endif
