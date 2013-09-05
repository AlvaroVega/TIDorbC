//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPMessage.C
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

#include "TIDorb/core/comm/iiop/GIOPMessage.h"

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

void* GIOPMessage::_impl()
{
	return this;
}

const char* GIOPMessage::_typeid()
{
	//return CORBA::string_dup("GIOPMessage");
	return "GIOPMessage";
}

GIOPMessage::GIOPMessage()
  : _header(),
    _message_buffer(NULL), 
    _body_buffer(NULL), 
    _message_completed(false),
    _headers_marshaled(false),
    _compressor(Compression::CompressorIdLevel(0,0))
{
}

GIOPMessage::GIOPMessage(const GIOPHeader& header)
  : _header(header), 
    _message_buffer(NULL), 
    _body_buffer(NULL), 
    _headers_marshaled(false),
    _compressor(Compression::CompressorIdLevel(0,0))
{
    
	switch(_header.getMsgType()) {
        	case MessageError:
                	_message_completed = true;
        	case CloseConnection:
                	_message_completed = true;
        	default:
                	_message_completed = false;
    }
}


GIOPMessage::~GIOPMessage(){}

void GIOPMessage::write_headers()
{
  if(hasBody())
  {
    if(!_message_completed)
    throw CORBA::INTERNAL("Uncompleted Message: no body");

    if (!_headers_marshaled)
    {
      TIDorb::core::cdr::CDROutputStream out(NULL, _message_buffer);

      out.set_version(_header.getVersion());

      // write message size = buffer size - 12 octets from header

      _header.setSize(_message_buffer->get_available_bytes() - GIOPHeader::HEADER_SIZE);

      _header.write(out);

      _headers_marshaled = true;
    }
  }
  else
  {
    if (!_headers_marshaled) //header message
    {
      _message_buffer = new TIDorb::core::cdr::BufferCDR(GIOPHeader::HEADER_SIZE);

      TIDorb::core::cdr::CDROutputStream out(NULL,_message_buffer);

      _header.write(out);
      _headers_marshaled = true;

    }
  }
}

const GIOPHeader& GIOPMessage::getHeader() const
{
        return _header;
}

TIDorb::core::cdr::BufferCDR* GIOPMessage::get_message_buffer() const
{
	if(!_message_completed)
		return NULL;
      
	if(!_headers_marshaled) {
		((GIOPMessage*) this)->write_headers();
	}

	return _message_buffer;
}

void GIOPMessage::set_message_buffer(TIDorb::core::cdr::BufferCDR* cdr)
{
        _message_buffer = cdr;
}

void GIOPMessage::set_message_completed(CORBA::Boolean value)
{
        _message_completed = value;
}

bool GIOPMessage::hasBody() const
{
        switch(_header.getMsgType()) {
                case MessageError:
                        return false;
                case CloseConnection:
                        return false;
                default:
                        return true;
        }
}

void GIOPMessage::set_body(TIDorb::core::cdr::BufferCDR_ref buf)
{
  _message_buffer = buf;
  _message_completed = true;
  _headers_marshaled = true;
}

void GIOPMessage::receive_body(TIDorb::core::comm::Connection* conn,
                               const CORBA::Octet* header_bytes)
{
    TIDorb::core::cdr::ChunkCDR* chunk = conn->receive_chunk(_header, header_bytes);
    _message_buffer = new TIDorb::core::cdr::BufferCDR(chunk);
    _message_completed = true;
    _headers_marshaled = true;

}

Compression::CompressorIdLevel GIOPMessage::get_compressor() const
{
  return _compressor;
}

void GIOPMessage::set_compressor(Compression::CompressorIdLevel compressor)
{
  _compressor = compressor;
}


}
}
}
} // namespace TIDorb::core::comm::iiop

ostream& operator<<(ostream& os, 
                    const TIDorb::core::comm::iiop::GIOPMessage& msg)
{
    os << msg.getHeader() << endl;
    return os;
}
