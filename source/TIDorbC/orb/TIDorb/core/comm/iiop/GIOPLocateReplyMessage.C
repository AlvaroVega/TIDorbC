//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPLocateReplyMessage.C
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

#include "TIDorb/core/comm/iiop/GIOPLocateReplyMessage.h"
#include "TIDorb/core/util/exceptions.h"

//MLG
void* TIDorb::core::comm::iiop::GIOPLocateReplyMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPLocateReplyMessage::_typeid()
{
	//return CORBA::string_dup("GIOPLocateReplyMessage");
	return "GIOPLocateReplyMessage";
}
//EMLG


TIDorb::core::comm::iiop::GIOPLocateReplyMessage::GIOPLocateReplyMessage
  (const TIDorb::core::comm::iiop::GIOPHeader& header)
  : GIOPFragmentedMessage(header),
    _reply_status(TIDorb::core::comm::iiop::UNKNOWN_OBJECT)
{
}




TIDorb::core::comm::iiop::GIOPLocateReplyMessage::GIOPLocateReplyMessage
  (const TIDorb::core::comm::iiop::Version& version,
   TIDorb::core::comm::iiop::RequestId id)
  : GIOPFragmentedMessage(GIOPHeader(version, TIDorb::core::comm::iiop::LocateReply), id),
    _reply_status(TIDorb::core::comm::iiop::UNKNOWN_OBJECT)
{
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::reset()
{
  _reply_status = TIDorb::core::comm::iiop::UNKNOWN_OBJECT;
  _message_completed = false;
  _headers_marshaled = false;
}

TIDorb::core::comm::iiop::LocateReplyStatusType
  TIDorb::core::comm::iiop::GIOPLocateReplyMessage::reply_status() const
{
  return _reply_status;
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::insert_unknown_object(TIDorb::core::TIDORB* orb)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::UNKNOWN_OBJECT;

  create_message_buffer_output(orb);

  _message_completed = true;
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::insert_object_here(TIDorb::core::TIDORB* orb)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::OBJECT_HERE;

  create_message_buffer_output(orb);

  _message_completed = true;
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::insert_system_exception
  (TIDorb::core::TIDORB* orb, const CORBA::SystemException& exception)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::LOC_SYSTEM_EXCEPTION;

  //MCPG -ATENCION

  create_message_buffer_output(orb);

  TIDorb::core::util::exceptions::SystemExceptionEncoder::write(*message_buffer_out, exception);

  _message_completed = true;
}




CORBA::SystemException* TIDorb::core::comm::iiop::GIOPLocateReplyMessage::extract_system_exception()
{
  if(_reply_status != TIDorb::core::comm::iiop::LOC_SYSTEM_EXCEPTION)
      throw CORBA::INTERNAL("Unexpected extract_arguments, SYSTEM_EXCEPTION is not the reply status");

//MCPG- ATENCION
  return TIDorb::core::util::exceptions::SystemExceptionEncoder::read(*message_buffer_in);
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::insert_forward
  (TIDorb::core::TIDORB* orb, CORBA::Object* obj)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::OBJECT_FORWARD;

  create_message_buffer_output(orb);

  message_buffer_out->write_Object(obj);

  _message_completed = true;
}




TIDorb::core::iop::IOR* TIDorb::core::comm::iiop::GIOPLocateReplyMessage::extract_forward()
{
  if(_reply_status != TIDorb::core::comm::iiop::OBJECT_FORWARD)
      throw CORBA::INTERNAL("Unexpected extract_arguments, OBJECT_FORWARD is not the reply status");

  TIDorb::core::iop::IOR* forward_ior = new TIDorb::core::iop::IOR();
  forward_ior->read(*message_buffer_in);
  return forward_ior;
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::insert_forward_perm
  (TIDorb::core::TIDORB* orb, CORBA::Object* obj)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::OBJECT_FORWARD_PERM;

  create_message_buffer_output(orb);

  message_buffer_out->write_Object(obj);

  _message_completed = true;

}




TIDorb::core::iop::IOR* TIDorb::core::comm::iiop::GIOPLocateReplyMessage::extract_forward_perm()
{
  if(_reply_status != TIDorb::core::comm::iiop::OBJECT_FORWARD_PERM)
      throw CORBA::INTERNAL("Unexpected extract_arguments, OBJECT_FORWARD_PERM is not the reply status");

  TIDorb::core::iop::IOR* forward_ior = new TIDorb::core::iop::IOR();
  forward_ior->read(*message_buffer_in);
  return forward_ior;
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::insert_needs_addressing_mode
  (TIDorb::core::TIDORB* orb,
   TIDorb::core::comm::iiop::AddressingDisposition disposition)
{
  reset();

  _reply_status = TIDorb::core::comm::iiop::LOC_NEEDS_ADDRESSING_MODE;

  create_message_buffer_output(orb);

  AddressingDispositionHelper::write(*message_buffer_out,disposition);

  _message_completed = true;
}




TIDorb::core::comm::iiop::AddressingDisposition
  TIDorb::core::comm::iiop::GIOPLocateReplyMessage::extract_addressing_disposition()
{
  if(_reply_status != TIDorb::core::comm::iiop::LOC_NEEDS_ADDRESSING_MODE)
      throw CORBA::INTERNAL("Unexpected extract_arguments, NEEDS_ADDRESSING_MODE is not the reply status");

  return AddressingDispositionHelper::read(*message_buffer_in);
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::create_message_buffer_output(TIDorb::core::TIDORB* orb)
{
  TIDorb::core::comm::iiop::GIOPFragmentedMessage::create_message_buffer_output(orb);

  // header
  // request_id marshaled with father create_message_buffer_output() in version 1.2

  if(_header.getVersion() < Version::VERSION_1_2) {
      message_buffer_out->write_ulong(_request_id);
  }

  // status
  message_buffer_out->write_ulong(_reply_status);
}




void TIDorb::core::comm::iiop::GIOPLocateReplyMessage::receive_body(TIDorb::core::comm::Connection* conn,
                                                                    const CORBA::Octet* header_bytes)
{
  TIDorb::core::comm::iiop::GIOPFragmentedMessage::receive_body(conn, header_bytes);

  // request_id has been readen in father's method in version 1.2
  if(_header.getVersion() < Version::VERSION_1_2) {
      message_buffer_in->read_ulong(_request_id);
  }

  // _reply_status

  //_reply_status = TIDorb::core::comm::iiop::LocateReplyStatusType::from_int(message_buffer_in->read_ulong());
  CORBA::ULong aux;
  message_buffer_in->read_ulong(aux);
  _reply_status = TIDorb::core::comm::iiop::LocateReplyStatusType_from_int(aux);
}

ostream& operator<<(ostream& os,
                    const TIDorb::core::comm::iiop::GIOPLocateReplyMessage& msg)
{
    os << (const TIDorb::core::comm::iiop::GIOPFragmentedMessage&) msg << " "
       << TIDorb::core::comm::iiop::toString(msg.reply_status()) << ends;
 
  return os;
}

