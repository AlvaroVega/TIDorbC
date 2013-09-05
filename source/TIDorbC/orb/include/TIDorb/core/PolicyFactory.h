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
///////////////////////////////////////////////////////////////////////////////////

#ifndef _TIDORB_CORE_POLICY_FACTORY_H
#define _TIDORB_CORE_POLICY_FACTORY_H

#include "CORBA.h"
#include "PortableServer.h"
#include "BiDirPolicy.h"
#include "FT.h"
#include "Messaging.h"
#include "ZIOP.h"
#include "SecurityLevel2.h"
#include "Security.h"

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

    static TIDorb::core::security::QOPPolicyImpl*
      create_QOPPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

    static TIDorb::core::security::EstablishTrustPolicyImpl*
      create_EstablishTrustPolicyImpl(const CORBA::Any& val)
      throw(CORBA::PolicyError);

};

}
}

#endif
