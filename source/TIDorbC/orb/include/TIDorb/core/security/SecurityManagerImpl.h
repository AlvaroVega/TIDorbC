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






