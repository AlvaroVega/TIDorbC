//////////////////////////////////////////////////////////////////////////////////
//
// File:        SecurityManagerImpl.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2009
// Author:      
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security.h"

#ifndef _TIDORB_CORE_SECURITY_SECURITY_MANAGER_H_
#define _TIDORB_CORE_SECURITY_SECURITY_MANAGER_H_

#include "Security.h"
#include "SecurityLevel2.h"


namespace TIDorb {
namespace core {
namespace security {

/*
 * SecurityChecker at CORBASEC level 2
 */

class SecurityManagerImpl : public virtual SecurityLevel2::SecurityManager,
                            public TIDThr::RefCounter, 
                            public virtual CORBA::LocalObject
{

private:
  TIDorb::core::TIDORB* m_orb;
  
public:

  SecurityManagerImpl(TIDorb::core::TIDORB* orb) 
    throw (TIDThr::SystemException);

  ~SecurityManagerImpl() throw (TIDThr::SystemException);


  //
  // Attributes from SecurityLevel2
  //
  Security::MechandOptionsList* 
  supported_mechanisms();
  
  SecurityLevel2::CredentialsList* 
  own_credentials();
  
  SecurityLevel2::RequiredRights_ptr 
  required_rights_object();
  
  SecurityLevel2::PrincipalAuthenticator_ptr 
  principal_authenticator();
  
  SecurityLevel2::AccessDecision_ptr 
  access_decision();

  SecurityLevel2::AuditDecision_ptr 
  audit_decision();

	
  //
  // Operations from SecurityLevel2
  //
  SecurityLevel2::TargetCredentials_ptr 
  get_target_credentials(CORBA::Object_ptr obj_ref);

  void remove_own_credentials(SecurityLevel2::Credentials_ptr creds);

  ::CORBA::Policy_ptr 
  get_security_policy(::CORBA::PolicyType policy_type);




  //
  // From SecurityLevel3 idl
  // 
//   SecurityLevel3::CredentialsCurator_ptr credentials_curator();
    
//   SecurityLevel3::TargetCredentials_ptr get_target_credentials(CORBA::Object_ptr the_object);
    
//   SecurityLevel3::ContextEstablishmentPolicy_ptr 
//   create_context_estab_policy(SecurityLevel3::CredsDirective creds_directive, 
//                               const SecurityLevel3::OwnCredentialsList& creds_list, 
//                               SecurityLevel3::FeatureDirective use_client_auth, 
//                               SecurityLevel3::FeatureDirective use_target_auth, 
//                               SecurityLevel3::FeatureDirective use_confidentiality, 
//                               SecurityLevel3::FeatureDirective use_integrity);
  
//   SecurityLevel3::ObjectCredentialsPolicy_ptr 
//   create_object_creds_policy(const SecurityLevel3::OwnCredentialsList& creds_list);




};


}
}
}

#endif






