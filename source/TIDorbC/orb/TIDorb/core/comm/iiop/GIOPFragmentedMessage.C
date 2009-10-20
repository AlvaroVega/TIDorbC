//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPFragmentedMessage.C
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

#include "TIDorb/core/comm/iiop/GIOPFragmentedMessage.h"



TIDorb::core::comm::iiop::GIOPFragmentedMessage::GIOPFragmentedMessage()
  : GIOPMessage(),
    _request_id(0),
    message_buffer_in(NULL),
    message_buffer_out(NULL),
    body_buffer_in(NULL),
    body_buffer_out(NULL),
    _fragment_size(0)
{
}


TIDorb::core::comm::iiop::GIOPFragmentedMessage::GIOPFragmentedMessage
  (const GIOPHeader& header)
    : GIOPMessage(header),
    _request_id(0),
    message_buffer_in(NULL),
    message_buffer_out(NULL),
    body_buffer_in(NULL),
    body_buffer_out(NULL),
    _fragment_size(0)
{
}


TIDorb::core::comm::iiop::GIOPFragmentedMessage::GIOPFragmentedMessage
  (TIDorb::core::comm::iiop::RequestId request_id, size_t fragment_size)
    : GIOPMessage(),
      _request_id(request_id),
      message_buffer_in(NULL),
      message_buffer_out(NULL),
      body_buffer_in(NULL),
      body_buffer_out(NULL),
      _fragment_size(fragment_size)
{
}

TIDorb::core::comm::iiop::GIOPFragmentedMessage::GIOPFragmentedMessage
  (const GIOPHeader& header, 
   TIDorb::core::comm::iiop::RequestId request_id, 
   size_t fragment_size)
    : GIOPMessage(header),
      _request_id(request_id),
      message_buffer_in(NULL),
      message_buffer_out(NULL),
      body_buffer_in(NULL),
      body_buffer_out(NULL),
      _fragment_size(fragment_size)
{
}

//
// MIOP extensions
//

size_t TIDorb::core::comm::iiop::GIOPFragmentedMessage::get_fragment_size(TIDorb::core::TIDORB* orb)
{
  return (_fragment_size == 0) ? orb->conf().fragment_size : _fragment_size;
}

//
// end MIOP extensions
//




TIDorb::core::comm::iiop::GIOPFragmentedMessage::~GIOPFragmentedMessage()
{

 if (message_buffer_in)
  	delete message_buffer_in;
 if (message_buffer_out)
        delete message_buffer_out; 
 if (body_buffer_in)
        delete body_buffer_in; 
 if (body_buffer_out)
        delete body_buffer_out; 

}




void TIDorb::core::comm::iiop::GIOPFragmentedMessage::create_message_buffer_output
  (TIDorb::core::TIDORB* orb)
{
  if (_message_buffer.is_null())
    _message_buffer = new TIDorb::core::cdr::BufferCDR(get_fragment_size(orb));
  else
    _message_buffer->recycle();

  //message_buffer_out = new CDROutputStream(orb, _message_buffer);

  delete message_buffer_out;
  message_buffer_out = new TIDorb::core::cdr::CDROutputStream(orb, _message_buffer);

  message_buffer_out->set_version(_header.getVersion());
  message_buffer_out->set_message(true);

  // set message skips also the GIOP HEADER
  // message_buffer_out.skip(GIOPHeader.HEADER_SIZE);

  // writes the request id
  if(_header.getVersion() >= Version::VERSION_1_2) {
    message_buffer_out->write_ulong(_request_id);
  }
}


void TIDorb::core::comm::iiop::GIOPFragmentedMessage::create_body_buffer_output
  (TIDorb::core::TIDORB* orb)
{
  if (_body_buffer.is_null())
    _body_buffer = new TIDorb::core::cdr::BufferCDR(get_fragment_size(orb));
  else
    _body_buffer->recycle();

  delete body_buffer_out;
  body_buffer_out = new TIDorb::core::cdr::CDROutputStream(orb, _body_buffer);

}


TIDorb::core::comm::iiop::RequestId
TIDorb::core::comm::iiop::GIOPFragmentedMessage::getRequestId() const
{
  return _request_id;
}


void TIDorb::core::comm::iiop::GIOPFragmentedMessage::set_body(TIDorb::core::cdr::BufferCDR_ref buf,
                                                               TIDorb::core::cdr::CDRInputStream* input)
{
  TIDorb::core::comm::iiop::GIOPMessage::set_body(buf);

  delete message_buffer_in;
  message_buffer_in = input;
  message_buffer_in->set_byte_order(_header.getByteOrder());
  
  //MCPG
  message_buffer_in->set_version(_header.getVersion());
  message_buffer_in->set_message(true);

  // set message skips also the GIOP HEADER
  //  message_buffer_in->skip(TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);
  // lectura adelantada del request_id para comprobar despues los fragmentos
  if(_header.getVersion() == TIDorb::core::comm::iiop::Version::VERSION_1_2) {
    message_buffer_in->read_ulong(_request_id);
    //    cerr << "GIOPFragmentedMessage::set_body _request_id is " << _request_id << endl; // must be 0 the first time
  }
  
  _message_completed = !(_header.hasMoreFragments());
}

void TIDorb::core::comm::iiop::GIOPFragmentedMessage::receive_body
  (TIDorb::core::comm::Connection* conn,
   const CORBA::Octet* header_bytes)
{
  TIDorb::core::comm::iiop::GIOPMessage::receive_body(conn, header_bytes);
  //MCPG

  delete message_buffer_in;
  message_buffer_in = new TIDorb::core::cdr::CDRInputStream(conn->orb(), _message_buffer);
  message_buffer_in->set_byte_order(_header.getByteOrder());

  //MCPG
  message_buffer_in->set_version(_header.getVersion());
  message_buffer_in->set_message(true);

  // set message skips also the GIOP HEADER
  // message_buffer_in.skip(GIOPHeader.HEADER_SIZE);
  // lectura adelantada del request_id para comprobar despues los fragmentos
  if(_header.getVersion() == TIDorb::core::comm::iiop::Version::VERSION_1_2) {
    message_buffer_in->read_ulong(_request_id);
  }

  _message_completed = !(_header.hasMoreFragments());
}




void TIDorb::core::comm::iiop::GIOPFragmentedMessage::write_headers()
{

  if (!_headers_marshaled) {
//MLG
    TIDorb::core::cdr::CDROutputStream out(NULL, _message_buffer);
    
    out.set_version(_header.getVersion());
    
    // write message size = buffer size - 12 octets from header

    _header.setSize(_message_buffer->get_available_bytes() - TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);

    _header.setMoreFragments(false);

    _header.write(out);

    _headers_marshaled = true;
//EMLG
  }
}


void TIDorb::core::comm::iiop::GIOPFragmentedMessage::add_fragment(TIDorb::core::comm::iiop::GIOPFragmentMessage* fragment)
{
  if(_message_completed)
    throw CORBA::MARSHAL();//"Unexpected Fragment");

  if(_message_buffer.is_null())
    throw CORBA::INTERNAL();//"Unexpected Fragment");

  if(! _message_buffer.is_null())
    _message_buffer->add_chunk(fragment->get_message_buffer()->get_chunk(0));

  _message_completed = !(fragment->getHeader().hasMoreFragments());
}

ostream& operator<<(ostream& os,
                    const TIDorb::core::comm::iiop::GIOPFragmentedMessage& msg)
{
      os << msg.getHeader() 
         << " id(" << msg.getRequestId()  << ")" << ends;

  return os;
}

void* TIDorb::core::comm::iiop::GIOPFragmentedMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPFragmentedMessage::_typeid()
{
	//return CORBA::string_dup("GIOPFragmentedMessage");
	return "GIOPFragmentedMessage";
}
