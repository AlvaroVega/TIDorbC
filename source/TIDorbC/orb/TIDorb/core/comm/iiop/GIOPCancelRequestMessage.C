//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPCancelRequestMessage.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/GIOPCancelRequestMessage.h"

//MLG
void* TIDorb::core::comm::iiop::GIOPCancelRequestMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPCancelRequestMessage::_typeid()
{
	//return CORBA::string_dup("GIOPCancelRequestMessage");
	return "GIOPCancelRequestMessage";
}
//EMLG

TIDorb::core::comm::iiop::GIOPCancelRequestMessage::GIOPCancelRequestMessage
  (const TIDorb::core::comm::iiop::Version& version,
         TIDorb::core::comm::iiop::RequestId id)
   : GIOPMessage(GIOPHeader(version, TIDorb::core::comm::iiop::CancelRequest)),
     request_id(id)
{
}




TIDorb::core::comm::iiop::GIOPCancelRequestMessage::GIOPCancelRequestMessage
  (const TIDorb::core::comm::iiop::GIOPHeader& header)
  : GIOPMessage(header)
{
}




TIDorb::core::comm::iiop::RequestId
TIDorb::core::comm::iiop::GIOPCancelRequestMessage::getRequestId() const
{
        return request_id;
}

void TIDorb::core::comm::iiop::GIOPCancelRequestMessage::set_body(TIDorb::core::cdr::BufferCDR_ref buf,
                                                                  TIDorb::core::cdr::CDRInputStream* input)
{
  TIDorb::core::comm::iiop::GIOPMessage::set_body(buf);

  input->set_byte_order(_header.getByteOrder());
  input->read_ulong(request_id);
  delete input;
}


void TIDorb::core::comm::iiop::GIOPCancelRequestMessage::receive_body
  (TIDorb::core::comm::Connection* conn,
   const CORBA::Octet* header_buffer)
{
        TIDorb::core::comm::iiop::GIOPMessage::receive_body(conn, header_buffer);

        //TIDorb::core::cdr::CDRInputStream* id_in = new CDRInputStream(null, message_buffer);
        TIDorb::core::cdr::CDRInputStream id_in(NULL, _message_buffer);
        id_in.set_byte_order(_header.getByteOrder());

        //request_id = new RequestId(id_in->read_ulong());
        id_in.read_ulong(request_id);
}




TIDorb::core::cdr::BufferCDR*
TIDorb::core::comm::iiop::GIOPCancelRequestMessage::get_message_buffer()
{
  if (!_message_completed)
  {
    if (_header.getVersion() >= TIDorb::core::comm::iiop::Version::VERSION_1_2)
    {
      //_message_buffer =
      //        new BufferCDR(new char[TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE +
      //                                TIDorb::core::cdr::CDR::ULONG_SIZE]);

      _message_buffer =
        new TIDorb::core::cdr::BufferCDR(TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE +
                                         TIDorb::core::cdr::CDR::ULONG_SIZE);

      // write header
      //TIDorb::core::cdr::CDROutputStream* out = new CDROutputStream(null,message_buffer);
      TIDorb::core::cdr::CDROutputStream out(NULL,_message_buffer);
      _header.write(out);

      //write
      out.write_ulong(request_id);

      //MCPG- No se necesita el m?todo close en el destructor ya se cierra
      /*try {

      out.close();
      } catch(CORBA::Exception& e){ }*/

      out = NULL;
      _message_completed = true;
      _headers_marshaled = true;
    }
    else
    {
      write_headers();
      _message_completed = true;
    }
  }

  return _message_buffer;
}




CORBA::Boolean TIDorb::core::comm::iiop::GIOPCancelRequestMessage::hasBody() const
{
  return (_header.getVersion() >= TIDorb::core::comm::iiop::Version::VERSION_1_2);
}
