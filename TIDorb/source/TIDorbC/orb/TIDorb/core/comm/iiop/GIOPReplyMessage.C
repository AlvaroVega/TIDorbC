//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPReplyMessage.C
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

#include "TIDorb/core/comm/iiop/GIOPReplyMessage.h"
#include "TIDorb/core/util/exceptions.h"

void* TIDorb::core::comm::iiop::GIOPReplyMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPReplyMessage::_typeid()
{
	//return CORBA::string_dup("GIOPReplyMessage");
	return "GIOPReplyMessage";
}


TIDorb::core::comm::iiop::GIOPReplyMessage::GIOPReplyMessage
  (const TIDorb::core::comm::iiop::Version& version)
  : GIOPFragmentedMessage(GIOPHeader(version, TIDorb::core::comm::iiop::Reply))
{
  _reply_status = TIDorb::core::comm::iiop::NO_EXCEPTION;
  _service_context_list = NULL;
  _service_context_list_owner = false;
}




TIDorb::core::comm::iiop::GIOPReplyMessage::GIOPReplyMessage
  (const TIDorb::core::comm::iiop::GIOPHeader& header)
  : GIOPFragmentedMessage(header)
{
  _reply_status = TIDorb::core::comm::iiop::NO_EXCEPTION;
  _service_context_list = NULL;
  _service_context_list_owner = false;
}




TIDorb::core::comm::iiop::GIOPReplyMessage::GIOPReplyMessage
 (const TIDorb::core::comm::iiop::Version& version,
  TIDorb::core::comm::iiop::RequestId id)
 : GIOPFragmentedMessage(GIOPHeader(version, TIDorb::core::comm::iiop::Reply), id)
{
  _reply_status = TIDorb::core::comm::iiop::NO_EXCEPTION;
  _service_context_list = NULL;
  _service_context_list_owner = false;
}




TIDorb::core::comm::iiop::GIOPReplyMessage::~GIOPReplyMessage()
{
  if (_service_context_list_owner && _service_context_list)
    delete _service_context_list;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::reset()
{
  _reply_status = TIDorb::core::comm::iiop::NO_EXCEPTION;
  _message_completed = false;
  _headers_marshaled = false;
  //_service_context_list = NULL;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_arguments(TIDorb::core::TIDORB* orb,
  TIDorb::core::ServerRequestImpl* request)
{
  reset();
  _request_id = request->get_id();
  _reply_status = TIDorb::core::comm::iiop::NO_EXCEPTION;

  create_message_buffer_output(orb);

  const CORBA::Any result = request->get_result(); // tk_null si no hay resultado
  result.delegate().write_value(*message_buffer_out);

  CORBA::NVList* list = request->get_parameters();

  if(list != NULL)
      TIDorb::core::NVListImpl::write_params_out(list, *message_buffer_out);
      
  //jagd el get_parameters hace duplicate interno y hay que liberar
  CORBA::release(list);
  
  _message_completed = true;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_system_exception
  (TIDorb::core::TIDORB* orb, const CORBA::SystemException& exception)
{
  reset();
  _reply_status = TIDorb::core::comm::iiop::SYSTEM_EXCEPTION;

  create_message_buffer_output(orb);
  TIDorb::core::util::exceptions::SystemExceptionEncoder::write(*message_buffer_out, exception);

  _message_completed = true;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_user_exception
  (TIDorb::core::TIDORB* orb,
   const CORBA::Any& exception)
{
  reset();
  _reply_status = TIDorb::core::comm::iiop::USER_EXCEPTION;

  create_message_buffer_output(orb);

  //exception.write_value(message_buffer_out);
  exception.delegate().write_value(*message_buffer_out);

  _message_completed = true;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::marshal_reply_header_1_1()
{
  // Service Context
  TIDorb::core::comm::iiop::ServiceContextList::write(_service_context_list, *message_buffer_out);

  // request_id
  message_buffer_out->write_ulong(_request_id);

  // reply_status
  message_buffer_out->write_ulong(_reply_status);
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::marshal_reply_header_1_2()
{
  // request_id marshaled with father create_body()
  // message_buffer_out.write_ulong(request_id);
  // reply_status
  message_buffer_out->write_ulong(_reply_status);

  // Service Context
  TIDorb::core::comm::iiop::ServiceContextList::write(_service_context_list, *message_buffer_out);

  // alignment to 8 in version 1.2
  message_buffer_out->fix_next_aligned_position(TIDorb::core::cdr::CDR::LONGLONG_SIZE);
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::unmarshal_reply_header_1_1()
{
  // Service Context
  _service_context_list = TIDorb::core::comm::iiop::ServiceContextList::read(*message_buffer_in);
  _service_context_list_owner = true; /// Check this!!!!!!!!!!!!

  // request_id
  message_buffer_in->read_ulong(_request_id);

  // reply_status
  //_reply_status = TIDorb::core::comm::iiop::ReplyStatusType::from_int(message_buffer_in->read_ulong());
  CORBA::ULong aux;
  message_buffer_in->read_ulong(aux);
  _reply_status = TIDorb::core::comm::iiop::ReplyStatusType_from_int(aux);

  if(_reply_status > TIDorb::core::comm::iiop::LOCATION_FORWARD)
    throw CORBA::MARSHAL("Version 1.0 or 1.1: Invalid ReplyStatus value.");
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::unmarshal_reply_header_1_2()
{
  // request_id unmarshaled in father's receive_body()

  // message_buffer_in.read_ulong(_request_id);

  // reply_status
  CORBA::ULong aux;
  message_buffer_in->read_ulong(aux);
  //_reply_status = TIDorb::core::comm::iiop::ReplyStatusType::from_int(message_buffer_in->read_ulong());
  _reply_status = TIDorb::core::comm::iiop::ReplyStatusType_from_int(aux);

  // Service Context
  _service_context_list = TIDorb::core::comm::iiop::ServiceContextList::read(*message_buffer_in);
  _service_context_list_owner = true; /// Check this!!!!!!!!!!!!
  // VERSION 1.2 alignment to 8
  try {
    //jagd OJO 
    // message_buffer_in->go_next_aligned_position(TIDorb::core::cdr::CDR::LONGLONG_SIZE);
      message_buffer_in->go_next_aligned_position_sin_exception(TIDorb::core::cdr::CDR::LONGLONG_SIZE);
  } catch (const CORBA::MARSHAL){
  }
}

TIDorb::core::comm::iiop::ReplyStatusType TIDorb::core::comm::iiop::GIOPReplyMessage::reply_status() const
{
  return _reply_status;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::set_service_context_list
(TIDorb::core::comm::iiop::ServiceContextList* list, bool owner)
{
  _service_context_list = list;
  _service_context_list_owner = owner;
}




const TIDorb::core::comm::iiop::ServiceContextList*
  TIDorb::core::comm::iiop::GIOPReplyMessage::get_service_context_list() const
{
  return _service_context_list;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_result_request
  (TIDorb::core::TIDORB* orb, TIDorb::core::ServerRequestImpl* request)
{
  switch(request->get_return_status()) {
    case TIDorb::core::ServerRequestStatus::NO_EXCEPTION:
      insert_arguments(orb, request);
      return;
    case TIDorb::core::ServerRequestStatus::USER_EXCEPTION:
      insert_user_exception(orb, request->get_exception());
      return;
    case TIDorb::core::ServerRequestStatus::SYSTEM_EXCEPTION:
      insert_system_exception(orb, request->get_system_exception());
      return;
    case TIDorb::core::ServerRequestStatus::LOCATION_FORWARD:
      insert_forward(orb, request->get_forward());
      return;
  }
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::extract_arguments(TIDorb::core::RequestImpl* request)
{
  if(_reply_status != TIDorb::core::comm::iiop::NO_EXCEPTION)
    throw CORBA::INTERNAL("Unexpected extract_arguments, NO_EXCEPTION is not the reply status");

  request->read_result(*message_buffer_in);

  CORBA::NVList_var list = request->arguments();

  if((list))
    TIDorb::core::NVListImpl::read_params_out(list, *message_buffer_in);
}




CORBA::Any*
TIDorb::core::comm::iiop::GIOPReplyMessage::extract_user_exception(CORBA::ExceptionList* list)
{
  if(_reply_status != TIDorb::core::comm::iiop::USER_EXCEPTION)
    throw CORBA::INTERNAL("Unexpected extract_arguments, USER_EXCEPTION is not the reply status");

  // lectura adelantada del repository_id de la excepcion
  message_buffer_in->fix_starting();

  char* name;
  message_buffer_in->read_string(name);
  message_buffer_in->rewind();

  // busqueda del typecode y lectura de sus componentes
  if(list != NULL) {
    CORBA::ULong count = list->count();
    try {
      for (CORBA::ULong i = 0; i < count; i++) {
        if(strcmp(name,list->item(i)->id())==0) {
          CORBA::Any* any = message_buffer_in->orb()->create_any();

          //any->read_value(message_buffer_in, list->item(i));
          any->delegate().read_value(*message_buffer_in, list->item(i));

          CORBA::string_free(name);
          return any;
        }
      }
    } catch (CORBA::TypeCode::Bounds) {  //??????????????????
    } catch (CORBA::TypeCode::BadKind) { //??????????????????
    }
  }

  CORBA::string_free(name);
  throw CORBA::UNKNOWN("Unknown user exception");
}




CORBA::SystemException*
TIDorb::core::comm::iiop::GIOPReplyMessage::extract_system_exception()
{
  if(_reply_status != TIDorb::core::comm::iiop::SYSTEM_EXCEPTION)
    throw CORBA::INTERNAL(); //"Unexpected extract_arguments, SYSTEM_EXCEPTION is not the reply status");

  return TIDorb::core::util::exceptions::SystemExceptionEncoder::read(*message_buffer_in);
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_forward(TIDorb::core::TIDORB* orb, CORBA::Object_ptr obj)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::LOCATION_FORWARD;

  create_message_buffer_output(orb);

  message_buffer_out->write_Object(obj);

  _message_completed = true;
}




TIDorb::core::iop::IOR* TIDorb::core::comm::iiop::GIOPReplyMessage::extract_forward()
{
  if(_reply_status != TIDorb::core::comm::iiop::LOCATION_FORWARD)
    throw CORBA::INTERNAL("Unexpected extract_arguments, LOCAL_FORWARD is not the reply status");

  TIDorb::core::iop::IOR* ior = new TIDorb::core::iop::IOR();
  ior->read(*message_buffer_in);
  return ior;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_forward_perm(TIDorb::core::TIDORB* orb, CORBA::Object_ptr obj)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::LOCATION_FORWARD_PERM;

  create_message_buffer_output(orb);

  message_buffer_out->write_Object(obj);

  _message_completed = true;
}




TIDorb::core::iop::IOR* TIDorb::core::comm::iiop::GIOPReplyMessage::extract_forward_perm()
{
  if(_reply_status != TIDorb::core::comm::iiop::LOCATION_FORWARD_PERM)
    throw CORBA::INTERNAL("Unexpected extract_arguments, LOCAL_FORWARD_PERM is not the reply status");

  TIDorb::core::iop::IOR* ior = new TIDorb::core::iop::IOR();
  ior->read(*message_buffer_in);
  return ior;
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::insert_needs_addressing_mode
 (TIDorb::core::TIDORB* orb,
  TIDorb::core::comm::iiop::AddressingDisposition disposition)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::NEEDS_ADDRESSING_MODE;

  create_message_buffer_output(orb);

  AddressingDispositionHelper::write(*message_buffer_out,disposition);

  _message_completed = true;
}




TIDorb::core::comm::iiop::AddressingDisposition
  TIDorb::core::comm::iiop::GIOPReplyMessage::extract_addressing_disposition()
{
  if(_reply_status != TIDorb::core::comm::iiop::NEEDS_ADDRESSING_MODE)
    throw CORBA::INTERNAL("Unexpected extract_arguments, NEEDS_ADDRESSING_MODE is not the reply status");

  return AddressingDispositionHelper::read(*message_buffer_in);
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::create_message_buffer_output(TIDorb::core::TIDORB* orb)
{
  TIDorb::core::comm::iiop::GIOPFragmentedMessage::create_message_buffer_output(orb);

  switch (_header.getVersion().getMinor()) {
    case 0:
    case 1:
        marshal_reply_header_1_1();
        break;
    case 2:
        marshal_reply_header_1_2();
        break;
  }
}




void TIDorb::core::comm::iiop::GIOPReplyMessage::receive_body
  (TIDorb::core::comm::Connection* conn,
   const CORBA::Octet* header_bytes)
{
  TIDorb::core::comm::iiop::GIOPFragmentedMessage::receive_body(conn, header_bytes);

  switch (_header.getVersion().getMinor()) {
    case 0:
    case 1:
        unmarshal_reply_header_1_1();
        break;
    case 2:
        unmarshal_reply_header_1_2();
        break;
  }

  message_buffer_in->fix_starting();
}

ostream& operator<<(ostream& os,
                    const TIDorb::core::comm::iiop::GIOPReplyMessage& msg)
{
    os << (const TIDorb::core::comm::iiop::GIOPFragmentedMessage&) msg << " "
       << TIDorb::core::comm::iiop::toString(msg.reply_status()) << ends;

    return os;
}

