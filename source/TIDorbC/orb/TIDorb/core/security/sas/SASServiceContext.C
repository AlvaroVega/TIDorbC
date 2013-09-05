//////////////////////////////////////////////////////////////////////////////////
//
// File:        SASServiceContext.C
// Description: 
//
// Rel:         01.00
// Created:     June, 2009
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

#include "TIDorb/core/security/sas/SASServiceContext.h"


TIDorb::core::security::sas::SASServiceContext::SASServiceContext()
  throw (TIDThr::SystemException)
  : ServiceContext(IOP::SecurityAttributeService)
{
  m_sas_context_body = NULL;
}


TIDorb::core::security::sas::SASServiceContext::~SASServiceContext()
  throw (TIDThr::SystemException)
{
  delete m_sas_context_body;
}


TIDorb::core::security::sas::SASServiceContext::SASServiceContext(
                          CSI::SASContextBody* sas_context_body) 
  throw (TIDThr::SystemException)
  : ServiceContext(IOP::SecurityAttributeService)
{
  m_sas_context_body = sas_context_body;
}


CSI::SASContextBody_ptr
TIDorb::core::security::sas::SASServiceContext::getSASContextBody()
{
  return m_sas_context_body;  
}


void TIDorb::core::security::sas::SASServiceContext::partial_read(
                                    TIDorb::core::cdr::CDRInputStream& input)
{
  if (m_sas_context_body != NULL) 
    delete m_sas_context_body;

  m_sas_context_body = new CSI::SASContextBody();
  
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();
  
  try {

    CSI::MsgType _d;
    encapsulation->read_short(_d);

    switch (_d) {
    case CSI::MTEstablishContext:
      {
        CSI::EstablishContext establish_context;
        CORBA::ULong length;
        CSI::IdentityTokenType it_type;

        // ContextId
        encapsulation->read_ulonglong(establish_context.client_context_id);

        // AuthorizationToken
        encapsulation->read_ulong(length);
        establish_context.authorization_token.length(length);

        //encapsulation->skip(length);
        for (int i = 0; i < length; i++) {
          encapsulation->read_ulong(establish_context.authorization_token[i].the_type);
          CORBA::ULong element_length;
          encapsulation->read_ulong(element_length);
          encapsulation->read_octet_array(establish_context.authorization_token[i].the_element.get_buffer(false), 0, element_length);
          
        }
        
        // IdentityToken
        encapsulation->read_ulong(it_type);

        switch (it_type) {
        case CSI::ITTAbsent:
          {
            CORBA::Boolean absent;
            encapsulation->read_boolean(absent);
            establish_context.identity_token.absent(absent);

            break;
          }
        case CSI::ITTAnonymous:
          {
            CORBA::Boolean anonymous;
            encapsulation->read_boolean(anonymous);
            establish_context.identity_token.anonymous(anonymous);
            break;
          }
        case CSI::ITTPrincipalName:
          {
            encapsulation->read_ulong(length);
            encapsulation->read_octet_array(establish_context.identity_token.principal_name().get_buffer(false), 0, length);
            break;
          }
        case CSI::ITTX509CertChain:
          {
            encapsulation->read_ulong(length);
            encapsulation->read_octet_array(establish_context.identity_token.certificate_chain().get_buffer(false), 0, length);
            break;
          }
        case CSI::ITTDistinguishedName:
          {
            encapsulation->read_ulong(length);
            encapsulation->read_octet_array(establish_context.identity_token.dn().get_buffer(false), 0, length);
          }
          break;
        }

        // GSSToken 
        encapsulation->read_ulong(length);
        establish_context.client_authentication_token.length(length);
        encapsulation->read_octet_array(establish_context.client_authentication_token.get_buffer(false), 0, length);
        
        m_sas_context_body->establish_msg(establish_context);

      break;
      }
    case CSI::MTCompleteEstablishContext:
      {
        CSI::CompleteEstablishContext complete_establish_context;
        CORBA::ULong length;

        encapsulation->read_ulonglong(complete_establish_context.client_context_id);

        encapsulation->read_boolean(complete_establish_context.context_stateful);

        encapsulation->read_ulong(length);
        complete_establish_context.final_context_token.length(length);
        encapsulation->read_octet_array(complete_establish_context.final_context_token.get_buffer(false), 0, length);

        m_sas_context_body->complete_msg(complete_establish_context);

        break;
      }
    case CSI::MTContextError:
      {
        CSI::ContextError context_error;

        encapsulation->read_ulonglong(context_error.client_context_id);

        encapsulation->read_long(context_error.minor_status);
        encapsulation->read_long(context_error.major_status);
        CORBA::ULong length;

        encapsulation->read_ulong(length);
        context_error.error_token.length(length);
        encapsulation->read_octet_array(context_error.error_token.get_buffer(false), 0, length);

        m_sas_context_body->error_msg(context_error);

        break;
      }
    case CSI::MTMessageInContext:
      {
        CSI::MessageInContext message_in_context;

        encapsulation->read_ulonglong(message_in_context.client_context_id);
        encapsulation->read_boolean(message_in_context.discard_context);

        m_sas_context_body->in_context_msg(message_in_context);

        break;
      }
    default:
      break;
    }

    delete encapsulation;

  } catch (...) {
    delete encapsulation;
    throw;
  }
}

	
void TIDorb::core::security::sas::SASServiceContext::write(
                                TIDorb::core::cdr::CDROutputStream& output) const
{
  if (m_sas_context_body == NULL) {
    return;
  }
    

  output.write_ulong(_context_id);
  
  TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
  
  try {

    encapsulation->write_short(m_sas_context_body->_d());

    switch(m_sas_context_body->_d()) {
    case CSI::MTEstablishContext:
      {
        const CSI::EstablishContext& establish_context = m_sas_context_body->establish_msg();

        // ContextId client_context_id;
        encapsulation->write_ulonglong(establish_context.client_context_id);

        // AuthorizationToken authorization_token;
        CORBA::ULong length = establish_context.authorization_token.length();
        encapsulation->write_ulong(length);

        for (int i = 0; i < length; i++) {
          encapsulation->write_ulong(establish_context.authorization_token[i].the_type);
          CORBA::ULong element_length = 
            establish_context.authorization_token[i].the_element.length();
          encapsulation->write_ulong(element_length);
          if (element_length > 0) {
            encapsulation->write_octet_array(
                  establish_context.authorization_token[i].the_element.get_buffer(), 
                  0, element_length);
          }
        }

        // IdentityToken identity_token;
        encapsulation->write_ulong(establish_context.identity_token._d());

        switch(establish_context.identity_token._d()) {
        case CSI::ITTAbsent:
          {
            encapsulation->write_boolean(establish_context.identity_token.absent());
            break;
          }
        case CSI::ITTAnonymous:
          {
            encapsulation->write_boolean(establish_context.identity_token.anonymous());
            break;
          }
        case CSI::ITTPrincipalName:
          {
            CORBA::ULong length = 
              establish_context.identity_token.principal_name().length();
            encapsulation->write_ulong(length);
            if (length > 0) {
              encapsulation->write_octet_array(
                    establish_context.identity_token.principal_name().get_buffer(), 
                    0, length);
            }
            break;
          }
        case CSI::ITTX509CertChain:
          {
            CORBA::ULong length = 
              establish_context.identity_token.certificate_chain().length();
            encapsulation->write_ulong(length);
            if (length > 0) {
              encapsulation->write_octet_array(
                  establish_context.identity_token.certificate_chain().get_buffer(), 
                  0, length);
            }
            break;
          }
        case CSI::ITTDistinguishedName:
          {
            CORBA::ULong length = establish_context.identity_token.dn().length();
            encapsulation->write_ulong(length);
            if (length > 0) {
              encapsulation->write_octet_array(
                    establish_context.identity_token.dn().get_buffer(), 
                    0, length);
            }
            break;
          }
        }
      
        // GSSToken client_authentication_token;
        length = establish_context.client_authentication_token.length();
        encapsulation->write_ulong(length);
        if (length > 0) {
          encapsulation->write_octet_array(
                establish_context.client_authentication_token.get_buffer(), 
                0, length);
        }


        break;
      }
    case CSI::MTCompleteEstablishContext:
      {
        const CSI::CompleteEstablishContext& complete_establish_context = 
          m_sas_context_body->complete_msg();

        encapsulation->write_ulonglong(complete_establish_context.client_context_id);

        encapsulation->write_boolean(complete_establish_context.context_stateful);

        CORBA::ULong length = 
          complete_establish_context.final_context_token.length();

        encapsulation->write_ulong(length);
        if (length > 0) {
          encapsulation->write_octet_array(
                complete_establish_context.final_context_token.get_buffer(), 
                0, length);
        }

        break;
      }
    case CSI::MTContextError:
      {
        const CSI::ContextError& context_error = m_sas_context_body->error_msg();

        encapsulation->write_ulonglong(context_error.client_context_id);

        encapsulation->write_long(context_error.minor_status);
        encapsulation->write_long(context_error.major_status);
        CORBA::ULong length = context_error.error_token.length();

        encapsulation->write_ulong(length);
        if (length > 0) {
          encapsulation->write_octet_array(
                context_error.error_token.get_buffer(), 0, length);
        }
        break;
      }
    case CSI::MTMessageInContext:
      {
        const CSI::MessageInContext& message_in_context = m_sas_context_body->in_context_msg();

        encapsulation->write_ulonglong(message_in_context.client_context_id);
        encapsulation->write_boolean(message_in_context.discard_context);

        break;
      }
    default:
      break;
    }

    output.write_buffer(*(encapsulation->get_buffer())); 

    delete encapsulation;
  	
  } catch (...) {
    delete encapsulation;
    throw;
  }
  
}
	
