//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPFragmentMessage.C
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

#include "TIDorb/core/comm/iiop/GIOPFragmentMessage.h"

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {
    
void* GIOPFragmentMessage::_impl()
{
	return this;
}

const char* GIOPFragmentMessage::_typeid()
{
	//return CORBA::string_dup("GIOPFragmentMessage");
	return "GIOPFragmentMessage";
}

CORBA::ULong
GIOPFragmentMessage::FRAGMENT_HEADER_SIZE_1_1 =
  GIOPHeader::HEADER_SIZE;


CORBA::ULong
GIOPFragmentMessage::FRAGMENT_HEADER_SIZE_1_2 =
  GIOPHeader::HEADER_SIZE + TIDorb::core::cdr::CDR::ULONG_SIZE;




GIOPFragmentMessage::GIOPFragmentMessage
  (const Version& version,
   RequestId id)
  : GIOPMessage(GIOPHeader(version, Fragment))
{
  _request_id = id;
}




GIOPFragmentMessage::GIOPFragmentMessage(const GIOPHeader& header)
 : GIOPMessage(header),
   _request_id(0)
{
  
}




GIOPFragmentMessage::~GIOPFragmentMessage()
{

}




RequestId GIOPFragmentMessage::getRequestId() const
{
  return _request_id;
}




bool GIOPFragmentMessage::hasBody() const
{
  return true;
}


void TIDorb::core::comm::iiop::GIOPFragmentMessage::set_body(TIDorb::core::cdr::BufferCDR_ref buf,
                                                             TIDorb::core::cdr::CDRInputStream* input)
{
  TIDorb::core::comm::iiop::GIOPMessage::set_body(buf);

  if(_header.getVersion()>= Version::VERSION_1_2) {
    input->set_byte_order(_header.getByteOrder());
    input->skip(TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);
    input->read_ulong(_request_id);
    delete input;
  }
}

void GIOPFragmentMessage::receive_body
  (TIDorb::core::comm::Connection* conn,
   const CORBA::Octet* header_bytes)
{
  GIOPMessage::receive_body(conn, header_bytes);

  if(_header.getVersion()>= Version::VERSION_1_2) {
    //TIDorb::core::cdr::CDRInputStream* id_in = new CDRInputStream(NULL, _message_buffer);
    TIDorb::core::cdr::CDRInputStream id_in(NULL, _message_buffer);
    id_in.set_byte_order(_header.getByteOrder());
    id_in.skip(GIOPHeader::HEADER_SIZE);
    id_in.read_ulong(_request_id);
  }
}




void GIOPFragmentMessage::write_header
  (TIDorb::core::cdr::CDROutputStream& out,
   GIOPHeader* header,
   RequestId request_id)
{
  header->write(out);

  if(header->getVersion() >= Version::VERSION_1_2) {
    out.write_ulong(request_id);
    out.alignment(TIDorb::core::cdr::CDR::LONGLONG_SIZE); // force alingment to 8
  }
}




void GIOPFragmentMessage::send(TIDorb::core::comm::Connection* conn)
{
  throw CORBA::INTERNAL("Fragment message cannot be sent as itself");
}




void GIOPFragmentMessage::skip_fragment_header_1_2(TIDorb::core::cdr::CDRInputStream& in)
{
  in.skip_ulong();
}



void GIOPFragmentMessage::skip_fragment_header_1_2(TIDorb::core::cdr::CDROutputStream& out)
{
  out.skip(TIDorb::core::cdr::CDR::LONG_SIZE);
}

}
}
}
} // namespace TIDorb::core::comm::iiop

ostream& operator<<(ostream& os,
                    const TIDorb::core::comm::iiop::GIOPFragmentMessage& msg)
{
    os << msg.getHeader() << " id(" << msg.getRequestId()  << ")" << ends;
    return os;
}
