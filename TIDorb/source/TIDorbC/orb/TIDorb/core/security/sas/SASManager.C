/////////////////////////////////////////////////////////////////////////
//
// File:        SASManager.C
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

#include "TIDorb/core/security/sas/SASManager.h"

#include "GSSUP.h"
#include "CSIIOP.h"

#include <vector>
#include <algorithm>

TIDorb::core::security::sas::SASManager::SASManager(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
  : m_orb(orb), conf(orb->conf())
{
  // Obtiene codec factory
  CORBA::Object_var obj = m_orb->resolve_initial_references("CodecFactory");
  IOP::CodecFactory_var factory = IOP::CodecFactory::_narrow(obj);
  
  // Crea codec
  IOP::Encoding encoding;
  encoding.format = IOP::ENCODING_CDR_ENCAPS;
  encoding.major_version = (CORBA::Octet) 1;
  encoding.minor_version = (CORBA::Octet) 2;

  m_codec = factory->create_codec(encoding); 

}


TIDorb::core::security::sas::SASManager::~SASManager()
  throw (TIDThr::SystemException)
{

}


TIDorb::core::security::sas::SASServiceContext*
TIDorb::core::security::sas::SASManager::get_client_SASServiceContext(TIDorb::core::iop::IOR* ior)
{
  
  CSI::SASContextBody_ptr sas_context_body = 
    new CSI::SASContextBody();
  

  CSI::ContextId context_id = check_equivalent_context(ior);

  if (context_id != 0) {

    CSI::MessageInContext message_in_context;
    message_in_context.client_context_id = context_id;
    message_in_context.discard_context = false;
    sas_context_body->in_context_msg(message_in_context);

    if (m_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "SASManager reusing SAS Context: ";
      msg << "{ context_id=" << message_in_context.client_context_id;
      msg << ", discard_context=" << message_in_context.discard_context;
      msg << " }";
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

  } else {

    CSI::EstablishContext establish_context;

    // TODO: fill establish_context using IOR and ORB info
    SSLIOP::SSL* _ssl = ior->get_SSL();
    CSIIOP::CompoundSecMechList* _csi = ior->get_CompoundSecMechList();

    // Conformance Level 0 implemented: stateless
    establish_context.client_context_id = 0; 

    // Conformance Level 1 not implemented 
    establish_context.authorization_token.length(0); 

    // TODO:
    //establish_context.identity_token.
    establish_context.identity_token.absent(false);

    // TODO: GSSUP: username + password
    // TODO: check also GSSUP at _csi
    // _ssl->getTargetSupports() as_context has EstablishTrustInClient
    //establish_context.client_authentication_token.length(0);
    if ( (strcmp(conf.gssup_user, "") != 0) &&
         (strcmp(conf.gssup_password, "") != 0) ) {
      CSI::GSSToken_ptr token = 
        this->create_GSSUP_GSSToken(conf.gssup_user, conf.gssup_password, 
                                    conf.csiv2_target_name);
      
      establish_context.client_authentication_token = (*token); // Copy all OctetSeq
      delete token;
    }
    
    if (m_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "SASManager creating a new stateless SAS EstablishContext ";
      msg << "{ context_id=" << establish_context.client_context_id;
      msg << ", identity_token=" << establish_context.identity_token._d();
      msg << " }";
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    sas_context_body->establish_msg(establish_context);

  }

  return new TIDorb::core::security::sas::SASServiceContext(sas_context_body);
}



CSI::SASContextBody_ptr 
TIDorb::core::security::sas::SASManager::validate_SASContextBody(
                                            CSI::SASContextBody_ptr body)
{
  TIDThr::Synchronized synchro(*this);


  // Evaluate body maching with �ORB conf &  ? 
  // Returns an ContextError or an CompleteEstablishContext

  CSI::SASContextBody_ptr return_body = new CSI::SASContextBody();


  if (body->_d() == CSI::MTEstablishContext) {

    if (m_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "SASManager validating SAS EstablishContext: ";
      msg << "{ context_id=" << body->establish_msg().client_context_id;
      msg << " }";
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    CSI::GSSToken_ptr gss_token_response = NULL;
    
    if (this->validate_GSSToken(
                 body->establish_msg().client_authentication_token,
                 gss_token_response)) {

      if (!body->establish_msg().client_context_id) {  // Stateless Context
        
        CSI::CompleteEstablishContext complete;
        complete.client_context_id = 0;
        complete.context_stateful = false;
        complete.final_context_token = (*gss_token_response); // Copy all OctetSeq
        return_body->complete_msg(complete);
        
      } else { // Statefull Context
        // TODO: check it
        
        // Add it to _established_contexts
        _established_contexts[body->establish_msg().client_context_id] = 
          body->establish_msg();

        CSI::CompleteEstablishContext complete;
        complete.client_context_id = 0;
        complete.context_stateful = true;
        complete.final_context_token = (*gss_token_response); // Copy all OctetSeq
        return_body->complete_msg(complete);
        
      }
      
    } else { // Token not valid
      
      CSI::ContextError error_msg;

      error_msg.major_status = 1;
      error_msg.minor_status = 1;
      error_msg.error_token = (*gss_token_response);

      return_body->error_msg(error_msg);
    }

    delete gss_token_response;

  } else if  (body->_d() == CSI::MTMessageInContext) {

    if (m_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "SASManager validating SAS MessageInContext: ";
      msg << "{ context_id=" << body->in_context_msg().client_context_id;
      msg << " }";
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }


    // Check if Context is at _established_contexts
    EstablishedContextTable::iterator it = 
      _established_contexts.find(body->in_context_msg().client_context_id);

    if (it == _established_contexts.end()) {

      CSI::ContextError error_msg;
      error_msg.major_status = 3;
      error_msg.minor_status = 1;
      error_msg.error_token = body->establish_msg().client_authentication_token;
      return_body->error_msg(error_msg);
    
    } else {

      CSI::CompleteEstablishContext complete;

      complete.client_context_id = 
        body->in_context_msg().client_context_id;
      complete.context_stateful = true;

      //TO-DO: Check body->in_context_msg().discard_context;

      // TODO:       
      complete.final_context_token.length(0); 
      
      return_body->complete_msg(complete);
      
    }
  }

  return return_body;
}



CSI::ContextId 
TIDorb::core::security::sas::SASManager::check_equivalent_context(TIDorb::core::iop::IOR* ior)

{
  TIDThr::Synchronized synchro(*this);

  // TODO: check if there is and equivalent context in _established_contexts

  return 0;
}



GSSUP::InitialContextToken_ptr 
TIDorb::core::security::sas::SASManager::create_GSSUP_ic_token(const char* username, 
                                                           const char* password,
                                                           const char* target_name)
{
  // obtain username/password from ORB->conf-> gssup_css options
  CORBA::ULong username_length = strlen(username);
  CORBA::ULong password_length = strlen(password);
  CORBA::ULong target_name_length = strlen(target_name);

  GSSUP::InitialContextToken_ptr ic_token = new GSSUP::InitialContextToken();

  ic_token->username.length(username_length);
  for (CORBA::ULong i = 0; i < username_length; i++) {
    ic_token->username[i] = (CORBA::Octet) username[i];
  } 

  ic_token->password.length(password_length);
  for (CORBA::ULong i = 0; i < password_length; i++) {
    ic_token->password[i] = (CORBA::Octet) password[i];
  } 

  ic_token->target_name.length(target_name_length);
  for (CORBA::ULong i = 0; i < target_name_length; i++) {
    ic_token->target_name[i] = (CORBA::Octet) target_name[i];
  } 

  return ic_token;
}


CSI::GSSToken_ptr 
TIDorb::core::security::sas::SASManager::create_GSSUP_GSSToken(
                                                     const char* username, 
                                                     const char* password,
                                                     const char* target_name)
{
  // GSSToken:
  //  (conforms RFC 2743: 3.1 Mechanism-Independent Token Format)
  //   token[0] = 0x60
  //   token[1] = length
  //   token[2] = 0x06
  //   token[3] = OID length
  //   token[4] = OID (GSSUP, KerberosV5, ... )
  //   token[5] = InitialContextToken


  // Get OID
  CORBA::OctetSeq_var oid_buffer = 
    TIDorb::core::security::sas::SASManager::convert_oid_to_asn1(GSSUP::GSSUPMechOID);
  //cerr << "SASManager::create_GSSToken OID" << endl;
  CORBA::OctetSeq_var oid_buffer_length = 
    TIDorb::core::security::sas::SASManager::convert_length_to_octets(oid_buffer->length());
  
  // Create Mechanism defined token: GSSUP, etc

  CORBA::Any any;
  if (true /* TODO select mechanism */ ) {   // InitialContextToken
    GSSUP::InitialContextToken_ptr ic_token =
      TIDorb::core::security::sas::SASManager::create_GSSUP_ic_token(username, password, target_name);

    any <<= *ic_token; 
    delete ic_token;
  }
  CORBA::OctetSeq_ptr mech_defined_token;
  mech_defined_token = m_codec->encode(any);

  
  

  CORBA::ULong token_length = 
    1 /* TAG for OBJECT IDENTIFIER */ +
    oid_buffer_length->length() /* OID length */ + 
    oid_buffer->length() /* OID */ +
    mech_defined_token->length() /* Mechanism defined token */;

  //cerr << "SASManager::create_GSSToken TokenLength" << endl;
  CORBA::OctetSeq_var token_length_length =
    TIDorb::core::security::sas::SASManager::convert_length_to_octets(token_length);

  CORBA::OctetSeq_ptr token_buffer = new CORBA::OctetSeq();
  CORBA::ULong token_buffer_length = 
    1 /* TAG for SEQUENCE */ +
    token_length_length->length() + 
    token_length;
    
  token_buffer->length(token_buffer_length);
  //cerr << "SASManager.create_GSSToken length = " << token_buffer_length << endl;

  CORBA::ULong i = 0;

  // TAG for SEQUENCE
  (*token_buffer)[i++] = 0x60;
  

  // Token Length
  for (CORBA::ULong j = 0; j < token_length_length->length(); j++) {
    (*token_buffer)[i++] = (*token_length_length)[j];
  }

  // TAG for OBJECT IDENTIFIER
  //cerr << "TAG for OID at i=" << i << endl;
  (*token_buffer)[i++] = 0x06;

  // OID Length
  for (CORBA::ULong j = 0; j < oid_buffer_length->length(); j++) {
    (*token_buffer)[i++] = (*oid_buffer_length)[j];
  }

  // OID
  for (CORBA::ULong j = 0; j < oid_buffer->length(); j++) {
    (*token_buffer)[i++] = (*oid_buffer)[j];
  }

  // InitialContextToken
  for (CORBA::ULong j = 0; j < mech_defined_token->length(); j++) {
    (*token_buffer)[i++] = (*mech_defined_token)[j];
  }
  
  delete mech_defined_token;

  if (m_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "SASManager created GSSUP token: ";
    msg << "{ username=" << username << " ,";
    msg << " password=" << password << " ,"; // hide this field??
    msg << " target_name=" << target_name << " }";
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  return (CSI::GSSToken_ptr) token_buffer;
}




CORBA::Boolean
TIDorb::core::security::sas::SASManager::validate_GSSToken(
                                          const CSI::GSSToken& gss_token,
                                          CSI::GSSToken_ptr& gss_token_response)
{
  CORBA::ULong token_length_aux = gss_token.length();
  // == will be the same that token_length + get_num_octets_from_octets (gss_token, index) + 1

  //cerr << "SASManager::validate_GSSToken gss_token length = " << token_length_aux << endl;
  CORBA::ULong index = 0;
  CORBA::Boolean valid = true;

  
  if (token_length_aux > 0) { // TODO: Check it
    
    // Check TAG
    if (gss_token[index++] != 0x60) {

      if (m_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "SASManager has detected an invalid GSSToken with TAG for SEQ: ";
        msg << gss_token[index-1];
        m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
      }
      valid = false;

    } else {

      // Get token length
      //cerr << "SASManager::validate_GSSToken Token length" << endl;
      CORBA::ULong token_length =  
        TIDorb::core::security::sas::SASManager::convert_octets_to_length((CORBA::OctetSeq&)gss_token, index);
    
      // Skip the octets which represents the token length
      index += TIDorb::core::security::sas::SASManager::get_num_octets_from_octets((CORBA::OctetSeq&)gss_token, index);
      

      // Check and skip TAG for ORBJECT IDENTIFIER
      //cerr << "TAG for OID at index=" << index << endl;
      if (gss_token[index++] != 0x06) {
        
        if (m_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "SASManager has detected an invalid GSSToken with TAG for OID: ";
          msg << gss_token[index-1];
          m_orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
        }
        valid = false;
        
      } else {

        // Get OID length
        //cerr << "SASManager::validate_GSSToken OID length" << endl;
        CORBA::ULong oid_length = 
          TIDorb::core::security::sas::SASManager::convert_octets_to_length((CORBA::OctetSeq&)gss_token, index);
        // Skip the octets which represents the OID length
        index += TIDorb::core::security::sas::SASManager::get_num_octets_from_octets((CORBA::OctetSeq&)gss_token, index);
        
        // Check OID
        CORBA::OctetSeq_var oid_buffer = new CORBA::OctetSeq();
        oid_buffer->length(oid_length);
        for (CORBA::ULong i = 0; i < oid_length; i++)
          (*oid_buffer)[i] = gss_token[index+i];
        // TODO: check OID content: GSSUP...
        CORBA::String_var asn1 = convert_asn1_to_oid(oid_buffer);
        //cerr << "SASManager::validate_GSSToken OID " << asn1 << endl;
        
        // Skip OID
        index += oid_length;

        
        if (strcmp(asn1, GSSUP::GSSUPMechOID) == 0) {

          // Check GSSUP ICToken
          CORBA::OctetSeq_var ic_token_buffer = new CORBA::OctetSeq();
          CORBA::ULong ic_token_length = token_length_aux - index;
          ic_token_buffer->length(ic_token_length);
          for (CORBA::ULong i = 0; i < ic_token_length; i++)
            (*ic_token_buffer)[i] = gss_token[index+i];
          
          GSSUP::ErrorCode error_code = 
            this->validate_GSSUP_token(ic_token_buffer, 
                                       conf.gssup_user, 
                                       conf.gssup_password, 
                                       conf.csiv2_target_name);
          valid = (error_code == 0);


          if (valid) { // response will be a final_context_token
            
            gss_token_response = 
              this->create_GSSUP_GSSToken(conf.gssup_user, conf.gssup_password, 
                                          conf.csiv2_target_name);
            
          } else { // response will be an error_token
            
            GSSUP::ErrorToken_ptr error_token = new GSSUP::ErrorToken(error_code);
            CORBA::Any any;
            any <<= *error_token;
            delete error_token;
            gss_token_response = (CSI::GSSToken*)m_codec->encode(any);
            
          }
          
        } else { // No implemented other mechanims
          valid = false;
        }

      }

    }
  
  } else { // length of gss_token is < 0
    valid = false;
  }
  
  if (gss_token_response == NULL) {
    gss_token_response = new CSI::GSSToken(0);
    gss_token_response->length(0);
  }
  

  return valid;
}


GSSUP::ErrorCode
TIDorb::core::security::sas::SASManager::validate_GSSUP_token(
                                             const CORBA::OctetSeq_ptr ic_token_buffer,
                                             const char* username, 
                                             const char* password,
                                             const char* target_name)
{
  GSSUP::ErrorCode error_code;

  CORBA::ULong username_length;
  CORBA::ULong password_length;
  CORBA::ULong target_name_length;


  if (m_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "SASManager validating GSSUP token ...";
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }


  const GSSUP::InitialContextToken* ic_token;

  CORBA::Any_var value_result;

  value_result = m_codec->decode(*ic_token_buffer);
  (*value_result) >>= ic_token;


  // Compare GSS token values with username/password provided
  username_length = ic_token->username.length();
  char* token_username = NULL;
  if (username_length > 0) {
    token_username = (char*) malloc(sizeof(char)*(username_length+1));
    CORBA::ULong i;
    for (i = 0; i < username_length; i++) 
      token_username[i] = ic_token->username[i];
    token_username[i] = '\0';
  }
  else
    token_username = strdup("");

  password_length = ic_token->password.length();
  char* token_password = NULL;
  if (password_length > 0) {
    token_password = (char*) malloc(sizeof(char)*(password_length+1));
    CORBA::ULong i ;
    for (i = 0; i < password_length; i++) 
      token_password[i] = ic_token->password[i];
    token_password[i] = '\0';
  }
  else
    token_password = strdup("");

  target_name_length = ic_token->target_name.length();
  char* token_target_name = NULL;
  if (target_name_length > 0) {
    token_target_name = (char*) malloc(sizeof(char)*(target_name_length+1));
    CORBA::ULong i;
    for (i = 0; i < target_name_length; i++) 
      token_target_name[i] = ic_token->target_name[i];
    token_target_name[i] = '\0';
  }
  else
    token_target_name = strdup("");


  if (m_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "SASManager created GSSUP token: ";
    msg << "{ tusername=" << token_username << " ,";
    msg << " tpassword=" << token_password << " ,"; // hide this field??
    msg << " ttarget_name=" << token_target_name << " } vs " ;
    msg << "{ username=" << username << " ,";
    msg << " password=" << password << " ,"; // hide this field??
    msg << " target_name=" << target_name << " }";
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  
  if (strcmp(token_username, username) != 0)
    error_code = GSSUP::GSS_UP_S_G_NOUSER;
  else if (strcmp(token_password, password) != 0)
    error_code = GSSUP::GSS_UP_S_G_BAD_PASSWORD;
  else if (strcmp(token_target_name, target_name) != 0)
    error_code = GSSUP::GSS_UP_S_G_BAD_TARGET;
  else
    error_code = 0; // OK
   
  if (m_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "SASManager GSSUP token validation: error_code = " << error_code;
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  free(token_username);
  free(token_password);
  free(token_target_name);

  return error_code;
}


const char* 
TIDorb::core::security::sas::SASManager::get_GSSToken_error_reason(
                                      const CSI::GSSToken& gss_token)
{
  char* reason;

  const GSSUP::ErrorToken* error_token;

  CORBA::Any_var value_result;

  value_result = m_codec->decode((const ::CORBA::OctetSeq&)gss_token);

  if ( (*value_result) >>= error_token ) {
    
    switch (error_token->error_code) {
    case GSSUP::GSS_UP_S_G_UNSPECIFIED:
      reason = strdup("GSSUP error UNSPECIFIED ");
      break;
    case GSSUP::GSS_UP_S_G_NOUSER:
      reason = strdup("GSSUP error NO USER ");
      break;
    case GSSUP::GSS_UP_S_G_BAD_PASSWORD:
      reason = strdup("GSSUP error BAD PASSWORD ");
      break;
    case GSSUP::GSS_UP_S_G_BAD_TARGET:
      reason = strdup("GSSUP ERROR BAD TARGET ");
      break;
    }
  } else {
    reason = strdup("GSSToken validation error");
  }

  return reason;
}


CORBA::OctetSeq_ptr 
TIDorb::core::security::sas::SASManager::convert_oid_to_asn1(const char* oid)
{
  string rest_of_str;
  string s_oid = oid;

  int pos_init = s_oid.find(':');
  if (pos_init > 0)
    rest_of_str = s_oid.substr(pos_init + 1);
  else
    rest_of_str = s_oid;

  vector<unsigned int> oid_int_vector;
  bool end = false;
  int pos_num = 0;
  string num;

  
  while (!end) {
    pos_num = rest_of_str.find('.');
    if (pos_num > 0) {
      num = rest_of_str.substr(0, pos_num);
      rest_of_str = rest_of_str.substr(pos_num + 1, rest_of_str.length());
      oid_int_vector.push_back(atoi(num.c_str()));
    }
    else {
      oid_int_vector.push_back(atoi(rest_of_str.c_str()));
      end = true;
    }
  }
 

  vector<unsigned char> oid_char_vector;
  oid_char_vector.push_back(6); // OID Tag
  oid_char_vector.push_back(0); // temp length

  int oid_int_vector_len = oid_int_vector.size();
  int oid_char_vector_len = 1;
  oid_char_vector.push_back(40 * oid_int_vector[0] + oid_int_vector[1]);

  for (int i = 2; i < oid_int_vector_len; i++) {

    CORBA::OctetSeq_ptr oid_char_tmp_vector = convert_int_to_asn1char(oid_int_vector[i]);
    for (CORBA::ULong i = 0; i < oid_char_tmp_vector->length(); i++){
      oid_char_vector.push_back((*oid_char_tmp_vector)[i]); 
      oid_char_vector_len++;
    }

  }
  oid_char_vector[1] = oid_char_vector_len;

  CORBA::OctetSeq* oid_asn1 = new CORBA::OctetSeq;
  oid_asn1->length(oid_char_vector.size());
  for (CORBA::ULong j = 0; j < oid_char_vector.size(); j++)
    (*oid_asn1)[j] = oid_char_vector[j];
  return oid_asn1;

}



char* TIDorb::core::security::sas::SASManager::convert_asn1_to_oid(
                                                   CORBA::OctetSeq& oid_asn1)
{                                                                               
  int oid_asn1_len = oid_asn1[1] + 2;                                                   
  vector<unsigned int> oid_int_vector;                                                 

  unsigned int large_num[64];
  int large_num_octets=0;

  CORBA::Long i;
  for (i = 3; i < oid_asn1_len; i++) {                                             
    if (oid_asn1[i] >= 127) {
      large_num[large_num_octets++] = (oid_asn1[i] % 128);
      for (CORBA::ULong j=0; j < large_num_octets; j++) {
        large_num[j] *= 128;
      }
    } 
    else {
      if (large_num_octets > 0) {
        unsigned int tot = 0;
        for (CORBA::ULong j=0; j < large_num_octets; j++)
          tot += large_num[j];
        tot += oid_asn1[i];
        large_num_octets=0;
        oid_int_vector.push_back(tot);
      }
      else 
        oid_int_vector.push_back(oid_asn1[i]);
    }
    if (large_num_octets >= 64) 
      throw CORBA::BAD_PARAM("Current sequence length exceeded");
  }

  TIDorb::util::StringBuffer oid_stream;
  int first_octet  = oid_asn1[2] / 40; // first number can be 0/1/2
  int second_octet = oid_asn1[2] - first_octet * 40;
  oid_stream << "oid:" << first_octet << "." << second_octet << ".";
  int vector_len = oid_int_vector.size();
  for (i = 0; i < vector_len - 1; i++) {
    oid_stream << oid_int_vector[i] << ".";
  }
  oid_stream << oid_int_vector[vector_len-1];
  return CORBA::string_dup(oid_stream.str().c_str());
}                                      




CORBA::OctetSeq_ptr 
TIDorb::core::security::sas::SASManager::convert_length_to_octets(CORBA::ULong length) 
{
  // Routine for translate an Integer into a OctetSeq conforms for Token length
  // RFC 2743: 3.1 Mechanism-Independet Token Format section 2. Token length octets

  CORBA::OctetSeq_ptr buffer = new CORBA::OctetSeq(0);
  CORBA::ULong i = 0;

  if (length < 128) {
    buffer->length(1);
    (*buffer)[i] = length;

  } else {
    CORBA::Octet oct_number[8]; // Max number:
    CORBA::Octet num_octets = 0;
    CORBA::ULong quotient = length / 256;
    CORBA::ULong rest = length % 256;
    CORBA::ULong swap = 0; // for temporal swapping
    while (quotient > 256 && num_octets < 8) {
      oct_number[num_octets++] = rest;
      // next step
      swap = quotient / 256;
      rest = quotient % 256;
      quotient = swap;
    }

    if (quotient < 256) {
      oct_number[num_octets++] = rest;
      oct_number[num_octets++] = quotient;
    }

    //cerr << "SASManager::convert_length_to_octets length = " << length;
    //cerr << "  num_octets = " << (int)num_octets << endl;
    if (num_octets >= 256) 
      throw CORBA::BAD_PARAM("Current sequence length exceeded");

    // The first octet will contain the number of octets
    buffer->length(num_octets+1);
    (*buffer)[i++] = (CORBA::Octet)(0x80 | num_octets);

    for (CORBA::Long j = num_octets - 1; j >= 0; j--) {
      //cerr << "SASManager::convert_length_to_octets octet = " << (int)oct_number[j] << endl;
      (*buffer)[i++] = oct_number[j];
    }
  }

  return buffer;
}


CORBA::ULong
TIDorb::core::security::sas::SASManager::convert_octets_to_length(
                                               const CORBA::OctetSeq& buffer,
                                               CORBA::ULong index) 
{

  // Routine for translate an OctetSeq into an Integer conforms Token length
  // RFC 2743: 3.1 Mechanism-Independet Token Format section 2. Token length octets

  CORBA::ULong length = 0;
  CORBA::ULong i = index;
  
  CORBA::Boolean has_octets = buffer[i] & 0x80;

  if (has_octets) {

    CORBA::ULong num_octets = buffer[i] & 0x7F;
    i++; // Skip first octet (which contains number of octets)
    CORBA::Octet exp = 0;
    while (num_octets > 0) {
      num_octets--;
      CORBA::ULong octet = buffer[i + num_octets];
      cerr << "SASManager::convert_octets_to_length: octet = " << octet << endl;
      length += pow(2, (exp*8)) * octet;
      exp++;
    }
    
  } else {
    length = buffer[i];
  }
  //cerr << "SASManager::convert_octets_to_length: index = " << index;
  //cerr << "  length = " << length << endl; 
  return length;
}



CORBA::ULong
TIDorb::core::security::sas::SASManager::get_num_octets_from_octets(
                                                const CORBA::OctetSeq& buffer,
                                                CORBA::ULong index) 
{

  CORBA::ULong num_octets;

  CORBA::Boolean has_octets = buffer[index] & 0x80;

  if (has_octets) 
    num_octets = 1 + (CORBA::ULong) buffer[index] & 0x7F;
  else
    num_octets = 1;

  //cerr << "SASManager::get_num_octets_from_octets: index = " << index;
  //cerr << "  num_octets = " << num_octets << endl;
  return num_octets;
}



CORBA::OctetSeq_ptr 
TIDorb::core::security::sas::SASManager::convert_int_to_asn1char(CORBA::ULong length) 
{
  CORBA::OctetSeq_ptr buffer = new CORBA::OctetSeq(0);
  CORBA::ULong i = 0;

  if (length < 128) {
    buffer->length(1);
    (*buffer)[i] = length;

  } else {
    CORBA::Octet oct_number[8]; // Max number:
    CORBA::Octet num_octets = 0;
    CORBA::ULong quotient = length / 128;
    CORBA::ULong rest = length % 128;
    CORBA::ULong swap = 0; // for temporal swapping
    while (quotient > 128 && num_octets < 8) {
      oct_number[num_octets++] = rest;
      // next step
      swap = quotient / 128;
      rest = quotient % 128;
      quotient = swap;
    }

    if (quotient < 128) {
      oct_number[num_octets++] = rest;
      oct_number[num_octets++] = quotient | 0x80;
    }

    //cerr << "SASManager::convert_length_to_octets length = " << length;
    //cerr << "  num_octets = " << (int)num_octets << endl;
    if (num_octets >= 128) 
      throw CORBA::BAD_PARAM("Current sequence length exceeded");

    buffer->length(num_octets);

    for (CORBA::Long j = num_octets - 1; j >= 0; j--) {
      //cerr << "SASManager::convert_length_to_octets octet = " << (int)oct_number[j] << endl;
      (*buffer)[i++] = oct_number[j];
    }
  }

  return buffer;
}



