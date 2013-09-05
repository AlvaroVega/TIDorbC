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
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security.h"


TIDorb::core::security::SecurityManagerImpl::SecurityManagerImpl(
                                                      TIDorb::core::TIDORB* orb) 
  throw (TIDThr::SystemException)
  : m_orb(orb)
{
  this->TIDThr::RefCounter::_add_ref();
}

TIDorb::core::security::SecurityManagerImpl::~SecurityManagerImpl() 
  throw (TIDThr::SystemException)
{

}


Security::MechandOptionsList* 
TIDorb::core::security::SecurityManagerImpl::supported_mechanisms()
{

  TIDorb::core::PolicyManagerImpl* policy_manager = m_orb->getPolicyManager();
    
  TIDorb::core::PolicyContext& context = policy_manager->getPolicyContext();
  
  Security::MechandOptionsList* mech_list = new Security::MechandOptionsList();
  CORBA::ULong mech_list_length = 0;
  mech_list->length(mech_list_length);

  // Check policy_type is a Security Policy implemented in TIDorb
  CORBA::Policy_ptr policy = NULL;
  policy = context.getPolicy(Security::SecEstablishTrustPolicy);
  if (policy != NULL) {

    SecurityLevel2::EstablishTrustPolicy_ptr et_policy = 
      SecurityLevel2::EstablishTrustPolicy::_narrow(policy); 
  
    if (et_policy->trust().trust_in_client) {
      mech_list->length(mech_list_length + 1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::EstablishTrust");
      (*mech_list)[mech_list_length].options_supported = Security::EstablishTrustInClient;
      mech_list_length++;
    }

    if (et_policy->trust().trust_in_target) {
      mech_list->length(mech_list_length + 1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::EstablishTrust");
      (*mech_list)[mech_list_length].options_supported = Security::EstablishTrustInTarget;
      mech_list_length++;
    }

    CORBA::release(et_policy);
    CORBA::release(policy);
  }
  policy = NULL;
  policy = context.getPolicy(Security::SecQOPPolicy);
  if (policy != NULL) {

    SecurityLevel2::QOPPolicy_ptr qop_policy = 
      SecurityLevel2::QOPPolicy::_narrow(policy); 

    switch (qop_policy->qop()) {
    case Security::SecQOPNoProtection: 
      mech_list->length(mech_list_length+1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::QOP");
      (*mech_list)[mech_list_length].options_supported = Security::NoProtection;
      mech_list_length++;
      break;

    case Security::SecQOPIntegrity:
      mech_list->length(mech_list_length+1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::QOP");
      (*mech_list)[mech_list_length].options_supported = Security::Integrity;
      mech_list_length++;
      break;

    case Security::SecQOPConfidentiality:
      mech_list->length(mech_list_length+1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::QOP");
      (*mech_list)[mech_list_length].options_supported = Security::Confidentiality;
      mech_list_length++;
      break;

    case Security::SecQOPIntegrityAndConfidentiality:
      mech_list->length(mech_list_length+1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::QOP");
      (*mech_list)[mech_list_length].options_supported = Security::Integrity;
      mech_list_length++;

      mech_list->length(mech_list_length+1);
      (*mech_list)[mech_list_length].mechanism_type = 
        CORBA::string_dup("Security::QOP");
      (*mech_list)[mech_list_length].options_supported = Security::Confidentiality;
      mech_list_length++;
      break;
    }

    CORBA::release(policy);
  }

  return mech_list;
}
  
SecurityLevel2::CredentialsList* 
TIDorb::core::security::SecurityManagerImpl::own_credentials()
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
  return NULL;
//   SecurityLevel2::CredentialsList* credentials = new SecurityLevel2::CredentialsList();
//   credentials->length(0);
//   return credentials;
}
  
SecurityLevel2::RequiredRights_ptr 
TIDorb::core::security::SecurityManagerImpl::required_rights_object()
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
  return NULL;
}

SecurityLevel2::PrincipalAuthenticator_ptr 
TIDorb::core::security::SecurityManagerImpl::principal_authenticator()
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
  return NULL;
}

SecurityLevel2::AccessDecision_ptr 
TIDorb::core::security::SecurityManagerImpl::access_decision()
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
  return NULL;
}

SecurityLevel2::AuditDecision_ptr 
TIDorb::core::security::SecurityManagerImpl::audit_decision()
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
  return NULL;
}

SecurityLevel2::TargetCredentials_ptr 
TIDorb::core::security::SecurityManagerImpl::get_target_credentials(
                                                     CORBA::Object_ptr obj_ref)
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
  return NULL;
}

void TIDorb::core::security::SecurityManagerImpl::remove_own_credentials(
                                               SecurityLevel2::Credentials_ptr creds)
{
  throw CORBA::NO_IMPLEMENT();
  // TODO
 }

::CORBA::Policy_ptr 
TIDorb::core::security::SecurityManagerImpl::get_security_policy(
                                                   ::CORBA::PolicyType policy_type)
{
  TIDorb::core::PolicyManagerImpl* policy_manager = m_orb->getPolicyManager();
    
  TIDorb::core::PolicyContext& context = policy_manager->getPolicyContext();

  // Check policy_type is a Security Policy implemented in TIDorb
  if ( (policy_type != Security::SecEstablishTrustPolicy) &&
       (policy_type != Security::SecQOPPolicy) )
    return NULL;

  return context.getPolicy(policy_type);

}
