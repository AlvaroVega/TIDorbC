//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPRequestMessage.C
// Description: 
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

#include "TIDorb/core/comm/iiop/GIOPRequestMessage.h"

//MLG
void* TIDorb::core::comm::iiop::GIOPRequestMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPRequestMessage::_typeid()
{
	//return CORBA::string_dup("GIOPRequestMessage");
	return "GIOPRequestMessage";
}
//EMLG


TIDorb::core::comm::iiop::GIOPRequestMessage::GIOPRequestMessage
  (const TIDorb::core::comm::iiop::GIOPHeader& header)
  : GIOPFragmentedMessage(header)
{
  service_context_list = NULL;
  service_context_list_owner = false;
}

TIDorb::core::comm::iiop::GIOPRequestMessage::GIOPRequestMessage
  (const TIDorb::core::comm::iiop::Version& ver)
  : GIOPFragmentedMessage(GIOPHeader(ver, TIDorb::core::comm::iiop::Request))
{
  service_context_list = NULL;
  service_context_list_owner = false;
}

TIDorb::core::comm::iiop::GIOPRequestMessage::GIOPRequestMessage
  (const TIDorb::core::comm::iiop::Version& ver,
   TIDorb::core::comm::iiop::RequestId id,
   size_t fragment_size)
   : GIOPFragmentedMessage(GIOPHeader(ver, TIDorb::core::comm::iiop::Request), 
                           id, fragment_size)
{
  service_context_list = NULL;
  service_context_list_owner = false;
}

TIDorb::core::comm::iiop::GIOPRequestMessage::~GIOPRequestMessage()
{
  if (service_context_list_owner && service_context_list)
    delete service_context_list;
}




void TIDorb::core::comm::iiop::GIOPRequestMessage::insert_request_header_1_0
  (const TIDorb::core::RequestImpl* request,
   const TIDorb::core::iop::IOR& ior)
{
  // service context
  TIDorb::core::comm::iiop::ServiceContextList::write(service_context_list, *message_buffer_out);

  // request_id
  message_buffer_out->write_ulong(_request_id);

  // with response?
  message_buffer_out->write_boolean(request->with_response());

  // object key

  // pra@tid.es - MIOP extensions
  // Pre-GIOP 1.2 support: 'MIOP' octets followed by encapsulated UIPMC profile
  ior.object_key()->write(*message_buffer_out);
  // end MIOP extensions

  // operation name
  message_buffer_out->write_string(request->operation());

  // principal: not implemented
  message_buffer_out->write_string("");
}




void TIDorb::core::comm::iiop::GIOPRequestMessage::insert_request_header_1_1
  (const TIDorb::core::RequestImpl* request,
   const TIDorb::core::iop::IOR& ior)
{
  // service context
  TIDorb::core::comm::iiop::ServiceContextList::write(service_context_list, *message_buffer_out);

  // request_id
  message_buffer_out->write_ulong(_request_id);

  // with response?
  message_buffer_out->write_boolean(request->with_response());

  // reserved[3]
  message_buffer_out->write_octet((CORBA::Octet) 0);
  message_buffer_out->write_octet((CORBA::Octet) 0);
  message_buffer_out->write_octet((CORBA::Octet) 0);

  // object key

  // pra@tid.es - MIOP extensions
  // Pre-GIOP 1.2 support: 'MIOP' octets followed by encapsulated UIPMC profile
  ior.object_key()->write(*message_buffer_out);
  // end MIOP extensions

  // operation name
  message_buffer_out->write_string(request->operation());

  // principal: not implemented
  message_buffer_out->write_string("");
}




void TIDorb::core::comm::iiop::GIOPRequestMessage::insert_request_header_1_2
  (const TIDorb::core::RequestImpl* request,
   const TIDorb::core::iop::IOR& ior,
   TIDorb::core::comm::iiop::AddressingDisposition disposition)
{
  // request_id marshaled in father create_message_buffer_out
  // message_buffer_out.write_ulong(request.get_id());

  // with response?
  unsigned char response_octet;

  if (request->with_response() || request->reliable_oneway())
    response_octet = (CORBA::Octet) 0x03;
  else
    response_octet = (CORBA::Octet) 0x00;

  message_buffer_out->write_octet(response_octet);

  // reserved[3]
  message_buffer_out->write_octet((CORBA::Octet) 0);
  message_buffer_out->write_octet((CORBA::Octet) 0);
  message_buffer_out->write_octet((CORBA::Octet) 0);


  switch(disposition) { 	
    case TIDorb::core::comm::iiop::KeyAddr:
        TargetAddress::write_object_key_address(*message_buffer_out, ior);
        break; 
    case TIDorb::core::comm::iiop::ProfileAddr:
        TargetAddress::write_profile_address(*message_buffer_out, ior);
        break;
    case TIDorb::core::comm::iiop::ReferenceAddr:
        TargetAddress::write_ior_address(*message_buffer_out, ior);      
        break;
    default:
    {
      throw CORBA::INTERNAL();
    }
//EMLG    
  }

  // operation name
  message_buffer_out->write_string(request->operation());

  // service context list
  TIDorb::core::comm::iiop::ServiceContextList::write(service_context_list, *message_buffer_out);

  // parameters alignment for Version 1.2
  message_buffer_out->fix_next_aligned_position(TIDorb::core::cdr::CDR::LONGLONG_SIZE);
}




TIDorb::core::ServerRequestImpl*
TIDorb::core::comm::iiop::GIOPRequestMessage::extract_request_1_0()
{
  // service_context_list
  service_context_list = TIDorb::core::comm::iiop::ServiceContextList::read(*message_buffer_in);
  service_context_list_owner = true;

  // request_id
  message_buffer_in->read_ulong(_request_id);

  // with response?
  //CORBA::Boolean with_response = message_buffer_in->read_boolean();
  CORBA::Boolean with_response;
  message_buffer_in->read_boolean(with_response);

  // object key
  TIDorb::core::comm::iiop::TargetAddress* target = new TargetAddress();
  TIDorb::core::comm::iiop::ObjectKey*     key    = new ObjectKey();
  key->read(*message_buffer_in);
  target->object_key(key); // consumed

  // operation name
  //char* operation = message_buffer_in->read_string();
  char* operation;
  message_buffer_in->read_string(operation);

  // principal: not implemented
  message_buffer_in->skip_string();

  message_buffer_in->fix_starting();
  TIDorb::core::ServerRequestImpl* request =
     new ServerRequestImpl(TIDorb::core::comm::iiop::Version::VERSION_1_0,
                           _request_id,
                           operation, // consumed
                           message_buffer_in->copy(),
                           target); // consumed


  request->with_response(with_response);

  return request;
}




TIDorb::core::ServerRequestImpl*
TIDorb::core::comm::iiop::GIOPRequestMessage::extract_request_1_1()
{
  // service_context_list
  service_context_list = TIDorb::core::comm::iiop::ServiceContextList::read(*message_buffer_in);
  service_context_list_owner = true;

  // request_id
  message_buffer_in->read_ulong(_request_id);

  // with response?
  CORBA::Boolean with_response;
  message_buffer_in->read_boolean(with_response);

  // reserved octets
  message_buffer_in->skip_octet_array(3);

  // object key
  TIDorb::core::comm::iiop::TargetAddress* target = new TargetAddress();
  TIDorb::core::comm::iiop::ObjectKey*     key    = new ObjectKey();
  key->read(*message_buffer_in);
  target->object_key(key); // consumed

  // operation name
  //char* operation = message_buffer_in->read_string();
  char* operation;
  message_buffer_in->read_string(operation);

  // principal: not implemented
  message_buffer_in->skip_string();

  message_buffer_in->fix_starting();
  TIDorb::core::ServerRequestImpl* request =
     new ServerRequestImpl(TIDorb::core::comm::iiop::Version::VERSION_1_1,
                           _request_id,
                           operation, // consumed
                           message_buffer_in->copy(),
                           target); // consumed

  request->with_response(with_response);

  return request;
}




TIDorb::core::ServerRequestImpl*
TIDorb::core::comm::iiop::GIOPRequestMessage::extract_request_1_2()
{
  // request_id extracted in father receive_message_buffer
  //request_id = new RequestId(message_buffer_in.read_ulong());

  // with response?
  //unsigned char* response_octet = message_buffer_in->read_octet();
  CORBA::Octet response_octet;
  message_buffer_in->read_octet(response_octet);

  // TODO: comprobar CORBA 2.6
  // 0x01 devuelve respuesta, pero con cuerpo vac?o
  // 0x03 devuelve respuesta con cuerpo
  CORBA::Boolean with_response = (response_octet & 0x01) != 0;

  // reserved octets
  message_buffer_in->skip_octet_array(3);

  // object key
  TIDorb::core::comm::iiop::TargetAddress* target = new TargetAddress();
  target->read(*message_buffer_in);

  // operation name
  //char* operation = message_buffer_in->read_string();
  char* operation;
  message_buffer_in->read_string(operation);

  // service_context_list
  service_context_list = TIDorb::core::comm::iiop::ServiceContextList::read(*message_buffer_in);
  service_context_list_owner = true;

  try {
    message_buffer_in->go_next_aligned_position(TIDorb::core::cdr::CDR::LONGLONG_SIZE);
  } catch (CORBA::MARSHAL) {
  }

  message_buffer_in->fix_starting();
  TIDorb::core::ServerRequestImpl* request =
     new ServerRequestImpl(TIDorb::core::comm::iiop::Version::VERSION_1_2,
                           _request_id,
                           operation, // consumed
                           message_buffer_in->copy(),
                           target, // consumed
                           this->get_compressor());

  request->with_response(with_response);

  return request;
}




void TIDorb::core::comm::iiop::GIOPRequestMessage::set_service_context_list(
                                             TIDorb::core::comm::iiop::ServiceContextList* list,
                                             bool owner)
{
  service_context_list = list;
  service_context_list_owner = owner;
}




const TIDorb::core::comm::iiop::ServiceContextList*
  TIDorb::core::comm::iiop::GIOPRequestMessage::get_service_context_list() const
{
  return service_context_list;
}




TIDorb::core::ServerRequestImpl*
TIDorb::core::comm::iiop::GIOPRequestMessage::extract_request()
{
  if(!_message_completed)
    throw CORBA::INTERNAL("Uncompleted message: no message_buffer.");

  switch (_header.getVersion().getMinor()) {
    case 0:
        return extract_request_1_0();
    case 1:
        return extract_request_1_1();
    case 2:
        return extract_request_1_2();
    default:
        return NULL;
  }
}




void TIDorb::core::comm::iiop::GIOPRequestMessage::insert_request
  (const TIDorb::core::RequestImpl* request,
   const TIDorb::core::iop::IOR& ior,
   TIDorb::core::comm::iiop::AddressingDisposition disposition)
{
  _request_id = request->get_id();

  create_message_buffer_output(request->orb());

  switch (_header.getVersion().getMinor()) {
    case 0:
      insert_request_header_1_0(request, ior);
      break;
    case 1:
      insert_request_header_1_1(request, ior);
      break;
    case 2:
      insert_request_header_1_2(request, ior, disposition);
      break;
  }

  // in parameters
  request->write_params_in(*message_buffer_out);

  // context
  CORBA::Context_var ctx = request->ctx();
  CORBA::ContextList_var ctx_list =((TIDorb::core::RequestImpl*) request)->contexts();
  message_buffer_out->write_Context(ctx, ctx_list);

  _message_completed = true;
}




//
// pra@tid.es - FT extensions
//

void TIDorb::core::comm::iiop::GIOPRequestMessage::insert_heartbeat_request
  (TIDorb::core::TIDORB* orb, TIDorb::core::comm::iiop::RequestId id)
{
  _request_id = id;

  create_message_buffer_output(orb);

  // service context
  TIDorb::core::comm::iiop::ServiceContextList::write(service_context_list, *message_buffer_out);

  // request_id
  message_buffer_out->write_ulong(_request_id);

  // with response?
  message_buffer_out->write_boolean(true);

  // object key
  TIDorb::core::poa::POAKey poa_key;
  poa_key._add_ref();
  TIDorb::core::comm::iiop::ObjectKey object_key(&poa_key);
  object_key.write(*message_buffer_out);

  // operation name
  message_buffer_out->write_string(TIDorb::core::comm::FT::FT::OPERATION_NAME);

  // principal: not implemented
  message_buffer_out->write_string("");

  // in parameters
  // context

  _message_completed = true;
}


TIDorb::core::PolicyContext* 
TIDorb::core::comm::iiop::GIOPRequestMessage::getRequestInvocationPolicies()
{

  if(service_context_list != NULL) {

    for(size_t i = 0; i < service_context_list->components.size(); i++) {

      if (service_context_list->components[i]->_context_id == IOP::INVOCATION_POLICIES ){

        const TIDorb::core::comm::iiop::ServiceContext* service_context =
          service_context_list->components[i];

        TIDorb::core::comm::iiop::InvocationPoliciesContext* context =
          (TIDorb::core::comm::iiop::InvocationPoliciesContext*)service_context;

        TIDorb::core::PolicyContext* policies = context->getPolicies();
        if (policies) {
          // We must duplicate policies due to pointer will not be accesible when
          // this call end
          return policies->duplicate();
        } else
          return NULL;

      }
      
    }
  }

  return NULL;
}
