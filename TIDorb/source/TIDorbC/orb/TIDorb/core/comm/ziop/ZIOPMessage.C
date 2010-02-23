//////////////////////////////////////////////////////////////////////////////////
//
// File:        ZIOPMessage.C
// Description: 
//
// Rel:         01.00
// Created:     November, 2008
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
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

#include "TIDorb/core/comm/ziop/ZIOPMessage.h"


namespace TIDorb {
namespace core {
namespace comm {
namespace ziop {

TIDorb::core::comm::ziop::ZIOPMessage::ZIOPMessage
(const TIDorb::core::comm::iiop::GIOPMessage& giop_message)
  : TIDorb::core::comm::iiop::GIOPMessage(giop_message.getHeader()),
    message_buffer_in(NULL),
    message_buffer_out(NULL),
    _fragment_size(0)
{
  _original_message_buffer = giop_message.get_message_buffer();
}

TIDorb::core::comm::ziop::ZIOPMessage::ZIOPMessage
(const TIDorb::core::comm::iiop::GIOPHeader& header)
  : TIDorb::core::comm::iiop::GIOPMessage(header),
    message_buffer_in(NULL),
    message_buffer_out(NULL),
    _fragment_size(0)
{
  _original_message_buffer = NULL;
}


TIDorb::core::comm::ziop::ZIOPMessage::~ZIOPMessage()
{
  if (message_buffer_in)
    delete message_buffer_in;
  if (message_buffer_out)
    delete message_buffer_out; 
}


size_t TIDorb::core::comm::ziop::ZIOPMessage::get_fragment_size(TIDorb::core::TIDORB* orb)
{
  return (_fragment_size == 0) ? orb->conf().fragment_size : _fragment_size;
}


void TIDorb::core::comm::ziop::ZIOPMessage::create_message_buffer_output
  (TIDorb::core::TIDORB* orb)
{
  if (_message_buffer.is_null())
    _message_buffer = new TIDorb::core::cdr::BufferCDR(get_fragment_size(orb));
  else
    _message_buffer->recycle();

  delete message_buffer_out;
  message_buffer_out = new TIDorb::core::cdr::CDROutputStream(orb, _message_buffer);

  message_buffer_out->set_version(_header.getVersion());
  message_buffer_out->set_message(true);

}



bool TIDorb::core::comm::ziop::ZIOPMessage::perform_compression(
                                        TIDorb::core::TIDORB* orb, 
                                        Compression::CompressorIdLevel compressor,
                                        CORBA::ULong low_value,
                                        ZIOP::CompressionMinRatioPolicyValue min_ratio)
{

  create_message_buffer_output(orb);

  _header.setCompressed(true);
  
  Compression::Compressor_var compressor_ptr = 
    orb->getCompressionManager()->get_compressor(compressor.compressor_id, 
                                                 compressor.compression_level);
  
  
  // Write 'body_buffer_out' bufferCDR into 'source' OctetSeq
  CORBA::OctetSeq source;
  CORBA::ULong source_length = 0;
  TIDorb::core::cdr::BufferCDR_ref buffer = _original_message_buffer;
  size_t num_chunks = buffer->get_num_available_chunks();
  TIDorb::core::cdr::ChunkCDR* chunk = NULL;
  for(size_t i = 0; i < num_chunks; i++) {
    chunk = buffer->get_chunk(i);
    CORBA::Long current_length = source_length;
    source_length += chunk->get_available_bytes();
    source.length(source_length+1);
    memcpy(&(source[current_length]), chunk->get_buffer(), chunk->get_available_bytes());
  }

  // Check (source_length > CompressorLowValue) to apply or not compression
  if (source.length() < low_value)
    return false;

  // Apply compression to source
  CORBA::OctetSeq compressed;
  compressor_ptr->compress(source, compressed);
  
  // Write compressed OctetSeq into message_buffer_out 
  message_buffer_out->write_ushort(compressor.compressor_id);
  message_buffer_out->write_ulong(source_length);

  // Check min ratio
  //if ((100 - (((float)compressed.length() / (float)source.length())*100)) < min_ratio) 
  if ( ((float)compressed.length() / (float)source.length()) < min_ratio) 
    return false;

  message_buffer_out->write_octet_array(compressed.get_buffer(), 0 , compressed.length());

  _message_completed = true;

  if (orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Compressing GIOP message ";
    msg << " from " << source.length() << " bytes "; 
    msg << " to " << compressed.length() << " bytes ";
    msg << "using compressor " << compressor.compressor_id;
    orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
  }

  _header.setSize(_message_buffer->get_available_bytes() - 
                  TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);
  _headers_marshaled = false;
  
  return true;
}


void TIDorb::core::comm::ziop::ZIOPMessage::receive_body
  (TIDorb::core::comm::Connection* conn,
   const CORBA::Octet* header_bytes)
{
  TIDorb::core::comm::iiop::GIOPMessage::receive_body(conn, header_bytes);

  delete message_buffer_in;
  message_buffer_in = new TIDorb::core::cdr::CDRInputStream(conn->orb(), _message_buffer);
  message_buffer_in->set_byte_order(_header.getByteOrder());

  message_buffer_in->set_version(_header.getVersion());
  message_buffer_in->set_message(true);

  _message_completed = !(_header.hasMoreFragments());

  CORBA::OctetSeq compressed;
  CORBA::ULong compressed_length;
  compressed_length = _header.getSize() - 8; // CompressionData without data buffer
  compressed.length(compressed_length);

  // Read CompressionData
  CORBA::ULong original_length;
  message_buffer_in->read_ushort(_compressor.compressor_id);
  message_buffer_in->read_ulong(original_length);
  message_buffer_in->read_octet_array(compressed.get_buffer(), (CORBA::ULong)0, 
                                      compressed_length);
    
  Compression::Compressor_var compressor_ptr = 
    conn->_orb->getCompressionManager()->get_compressor(_compressor.compressor_id, 0);
  
  CORBA::OctetSeq uncompressed;
  compressor_ptr->decompress(compressed, uncompressed);
 

  if (conn->_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Uncompressing ZIOP message ";
    msg << " from " << compressed.length() << " bytes "; 
    msg << " to " << uncompressed.length() << " bytes ";
    msg << "using compressor " << _compressor.compressor_id;
    conn->_orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
  }


  // From uncompressed to CDRInputStream
  TIDorb::core::cdr::ChunkCDR* chunk = 
    new TIDorb::core::cdr::ChunkCDR(uncompressed.length());

  memcpy((unsigned char*) chunk->get_buffer(), &(uncompressed[0]), uncompressed.length());

  chunk->set_available_bytes(uncompressed.length()); 
  _original_message_buffer = new TIDorb::core::cdr::BufferCDR(chunk);

}


void TIDorb::core::comm::ziop::ZIOPMessage::connect_GIOPMessage(
                                        TIDorb::core::comm::Connection* conn)
{
  TIDorb::core::TIDORB* _orb = conn->orb();

  switch(_header.getMsgType()) {
  case TIDorb::core::comm::iiop::Request:
    {
      TIDorb::core::comm::iiop::GIOPRequestMessage* message;
      message = new TIDorb::core::comm::iiop::GIOPRequestMessage(_header);

      TIDorb::core::cdr::CDRInputStream* original_buffer_in = 
        new TIDorb::core::cdr::CDRInputStream(_orb, _original_message_buffer);

      original_buffer_in->fix_starting();

      message->set_body(_original_message_buffer, original_buffer_in);

      message->set_compressor(_compressor);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk received - HEXDUMP ";
        msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                         message->get_message_buffer()->get_chunk(0)->get_length());
      }
      
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg <<  conn->toString() << ": " << *message << " has been received!";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      conn->manage_message(message);
      break;
    }  
  case TIDorb::core::comm::iiop::Reply:
    {
      TIDorb::core::comm::iiop::GIOPReplyMessage* message;
      message = new TIDorb::core::comm::iiop::GIOPReplyMessage(_header);    

      TIDorb::core::cdr::CDRInputStream* original_buffer_in = 
        new TIDorb::core::cdr::CDRInputStream(_orb, _original_message_buffer);

      original_buffer_in->fix_starting();

      message->set_body(_original_message_buffer, original_buffer_in);

      // Skip GIOP Reply header
      switch (_header.getVersion().getMinor()) {
      case 0:
      case 1:
        message->unmarshal_reply_header_1_1();
        break;
      case 2:
        message->unmarshal_reply_header_1_2();
        break;
      }

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk received - HEXDUMP ";
        msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                         message->get_message_buffer()->get_chunk(0)->get_length());
      }
      
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg <<  conn->toString() << ": " << *message << " has been received!";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      conn->manage_message(message);
      break;
    }
  case TIDorb::core::comm::iiop::CancelRequest:
    {
      TIDorb::core::comm::iiop::GIOPCancelRequestMessage* message;
      message = new TIDorb::core::comm::iiop::GIOPCancelRequestMessage(_header);

      TIDorb::core::cdr::CDRInputStream* original_buffer_in = 
        new TIDorb::core::cdr::CDRInputStream(_orb, _original_message_buffer);

      original_buffer_in->fix_starting();

      message->set_body(_original_message_buffer, original_buffer_in);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk received - HEXDUMP ";
        msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                         message->get_message_buffer()->get_chunk(0)->get_length());
      }
      
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg <<  conn->toString() << ": " << *message << " has been received!";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      conn->manage_message(message);
      break;
    }
  case TIDorb::core::comm::iiop::LocateRequest:
    {
      TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message;
      message = new TIDorb::core::comm::iiop::GIOPLocateRequestMessage(_header);    

      TIDorb::core::cdr::CDRInputStream* original_buffer_in = 
        new TIDorb::core::cdr::CDRInputStream(_orb, _original_message_buffer);

      original_buffer_in->fix_starting();

      message->set_body(_original_message_buffer, original_buffer_in);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk received - HEXDUMP ";
        msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                         message->get_message_buffer()->get_chunk(0)->get_length());
      }
      
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg <<  conn->toString() << ": " << *message << " has been received!";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      conn->manage_message(message);
      break;
    }
  case TIDorb::core::comm::iiop::LocateReply:
    {
      TIDorb::core::comm::iiop::GIOPLocateReplyMessage* message;
      message = new TIDorb::core::comm::iiop::GIOPLocateReplyMessage(_header);

      TIDorb::core::cdr::CDRInputStream* original_buffer_in = 
        new TIDorb::core::cdr::CDRInputStream(_orb, _original_message_buffer);

      original_buffer_in->fix_starting();

      message->set_body(_original_message_buffer, original_buffer_in);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk received - HEXDUMP ";
        msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                         message->get_message_buffer()->get_chunk(0)->get_length());
      }
      
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg <<  conn->toString() << ": " << *message << " has been received!";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      conn->manage_message(message);
      break;
    }
  case TIDorb::core::comm::iiop::Fragment:
    {
      TIDorb::core::comm::iiop::GIOPFragmentMessage* message;
      message = new TIDorb::core::comm::iiop::GIOPFragmentMessage(_header);    

      TIDorb::core::cdr::CDRInputStream* original_buffer_in = 
        new TIDorb::core::cdr::CDRInputStream(_orb, _original_message_buffer);

      original_buffer_in->fix_starting();

      message->set_body(_original_message_buffer, original_buffer_in);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk received - HEXDUMP ";
        msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                         message->get_message_buffer()->get_chunk(0)->get_length());
      }
      
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg <<  conn->toString() << ": " << *message << " has been received!";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      conn->manage_message(message);
      break;
    }
  case TIDorb::core::comm::iiop::CloseConnection:
    {
      conn->close_by_pair();
      break;
    }
  case TIDorb::core::comm::iiop::MessageError:
    {
      conn->close_by_error(CORBA::COMM_FAILURE("Connection closed due to pair message error."));
      break;
    }
  }
}


}
}
}
} // namespace TIDorb::core::comm::ziop
