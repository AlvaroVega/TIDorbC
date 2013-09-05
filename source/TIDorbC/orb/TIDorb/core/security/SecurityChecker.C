/////////////////////////////////////////////////////////////////////////
//
// File:        SecurityChecker.C
// Description:
//              
// Rel:         01.00
// Created:     September, 2009
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

#include "TIDorb/core/security/SecurityChecker.h"

/**
 * 
 * 
 * @param policy
 * @return mode
 */

int TIDorb::core::security::SecurityChecker::get_verify_mode(
                        const TIDorb::core::PolicyContext& context)
{
  int mode = SSL_VERIFY_NONE;
  
  CORBA::Policy_ptr policy = 
    context.getPolicy(Security::SecEstablishTrustPolicy);
  
  if (policy != NULL) {
    
    SecurityLevel2::EstablishTrustPolicy_ptr qop_policy = 
      SecurityLevel2::EstablishTrustPolicy::_narrow(policy);
    CORBA::release(policy);

    if (qop_policy->trust().trust_in_target)
      mode = SSL_VERIFY_PEER;
    
    if (qop_policy->trust().trust_in_client)
      mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT; 
    CORBA::release(qop_policy);

  }
  return mode;

}


 void TIDorb::core::security::SecurityChecker::checkQOP(
                                             TIDorb::core::TIDORB* orb, 
                                             const TIDorb::core::PolicyContext& context, 
                                             Security::AssociationOptions sec_options)

{

  /* 
    SecQOPNoProtection,
    SecQOPIntegrity,
    SecQOPConfidentiality,
    SecQOPIntegrityAndConfidentiality
  */


  // At client side:
  //    2.4.6.2 Secure Association Options

  CORBA::Policy_ptr policy = 
    context.getPolicy(Security::SecQOPPolicy);

  Security::AssociationOptions requires = sec_options;

  if (policy != NULL) {

    SecurityLevel2::QOPPolicy_ptr qop_policy = 
      SecurityLevel2::QOPPolicy::_narrow(policy);
    CORBA::release(policy);
    

    if (requires == 0) {
      TIDorb::util::StringBuffer msg;
      msg << "SecurityChecker detected no requiremets for SecQOPPolicy ";
      throw CORBA::INV_POLICY(msg.str().data());
    }

    if ( (qop_policy->qop() == Security::SecQOPNoProtection) && 
         ! (requires & Security::NoProtection) &&
         ((requires & Security::Integrity) || (requires & Security::Confidentiality) ) ) {
      TIDorb::util::StringBuffer msg;
      msg << "SecurityChecker detected SecQOPNoProtection insufficient ";
      msg << requires;
      throw CORBA::INV_POLICY(msg.str().data());
    }
    if ( (qop_policy->qop() == Security::SecQOPIntegrity) && 
         !(requires & Security::Integrity) && 
         (requires & Security::Confidentiality) ) {
      TIDorb::util::StringBuffer msg;
      msg << "SecurityChecker detected SecQOPIntegrity insufficient ";
      msg << requires;
      throw CORBA::INV_POLICY(msg.str().data());
    }   
    if ( (qop_policy->qop() == Security::SecQOPConfidentiality) && 
         !(requires & Security::Confidentiality) &&
         (requires & Security::Integrity) ) {
      TIDorb::util::StringBuffer msg;
      msg << "SecurityChecker detected SecQOPConfidenciality insufficient ";
      msg << requires;
      throw CORBA::INV_POLICY(msg.str().data());
    }
    if ( (qop_policy->qop() == Security::SecQOPIntegrityAndConfidentiality) && 
         !(requires & Security::Confidentiality) &&
         !(requires & Security::Integrity) &&
         (requires & Security::NoProtection) ) {
      TIDorb::util::StringBuffer msg;
      msg << "SecurityChecker detected SecQOPIntegrityAndConfidentiality insufficient ";
      msg << requires;
      throw CORBA::INV_POLICY(msg.str().data());
    }

  } else {

    if (!requires & Security::SecQOPNoProtection) {
      TIDorb::util::StringBuffer msg;
      msg << "SecurityChecker detected no SecQOP policy ";
      msg << requires;
      throw CORBA::NO_PERMISSION(msg.str().data());
    }

  }

  // TODO: returns QOPPolicy value to be applied ???
  return;
}



// TODO: Check what happens when ORB client sends with minor QOP to ORB server (without checking QOP)



void TIDorb::core::security::SecurityChecker::checkEstablishTrust(
                                             TIDorb::core::TIDORB* orb, 
                                             const TIDorb::core::PolicyContext& context, 
                                             CSIIOP::CompoundSecMechList* mechs)

{
  //cerr << "SecurityChecker::checkEstablishTrust " << endl;
  CORBA::Policy_ptr policy = 
    context.getPolicy(Security::SecEstablishTrustPolicy);

 if (policy != NULL) {
   
   SecurityLevel2::EstablishTrustPolicy_ptr qop_policy = 
      SecurityLevel2::EstablishTrustPolicy::_narrow(policy);
    CORBA::release(policy);

    // TOOO: ensure addecuate security mecanism is going to be used
    // also allow to disable security in some requests

    if (qop_policy->trust().trust_in_client) {
      // Check against ORB configuration
      const TIDorb::core::ConfORB& conf = orb->conf();

      if ( (!(strcmp(conf.ssl_private_key, "") != 0) || // SSL enabled
            !(strcmp(conf.ssl_certificate, "") != 0) ) &&
           (!(strcmp(conf.gssup_user, "") != 0) ||   // GSSUP enabled
            !(strcmp(conf.gssup_password, "") != 0) ) ) {
        TIDorb::util::StringBuffer msg;
        msg << "Invalid trust_in_client provided: ";
        msg << "ORB ssl_certificate/ssl_private_key or GSSUP not provided";
        CORBA::release(qop_policy);
        throw CORBA::INV_POLICY(msg.str().data());                                
      }

      // TODO:
      // SSLContext::set_verify(get_verify_mode(qop_policy));

    }

    
    if (qop_policy->trust().trust_in_target) {
      // Check against IOR values
      
      CORBA::Boolean matches = false;

      CORBA::ULong num_mechs = mechs->mechanism_list.length();
      
      CORBA::ULong i = 0;
      
      while ( (i < num_mechs) && (!matches) ) {
        if (mechs->mechanism_list[i].target_requires & CSIIOP::EstablishTrustInTarget) 
          matches = true;
        i++;        
      }

      if (!matches) {
        TIDorb::util::StringBuffer msg;
        msg << "Invalid trust_in_target provided: ";
        msg << "not matches with any mechanism";
        CORBA::release(qop_policy);
        throw CORBA::INV_POLICY(msg.str().data());
      }
    }

    CORBA::release(qop_policy);

 } else {

   // TODO:

 }
 
}
