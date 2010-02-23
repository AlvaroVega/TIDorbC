//////////////////////////////////////////////////////////////////////////////////
//
// File:        ZIOPMessage.h
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

#include "TIDorb/core/comm/ziop.h"

#ifndef _TIDORB_CORE_COMM_ZIOP_ZIOPMESSAGE_H_
#define _TIDORB_CORE_COMM_ZIOP_ZIOPMESSAGE_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace ziop {

class ZIOPMessage : public TIDorb::core::comm::iiop::GIOPMessage {

private:

  TIDorb::core::cdr::BufferCDR_ref    _original_message_buffer;

  TIDorb::core::cdr::CDRInputStream*  message_buffer_in;
  TIDorb::core::cdr::CDROutputStream* message_buffer_out;
  size_t _fragment_size;

  CORBA::ULong _original_length;

public:

  ZIOPMessage(const TIDorb::core::comm::iiop::GIOPMessage& giop_message);

  ZIOPMessage(const TIDorb::core::comm::iiop::GIOPHeader& header);

  ~ZIOPMessage();

  void receive_body(TIDorb::core::comm::Connection* conn,
                    const CORBA::Octet* header_bytes);

  size_t get_fragment_size(TIDorb::core::TIDORB* orb);

  bool perform_compression(TIDorb::core::TIDORB* orb, 
                           Compression::CompressorIdLevel compressor,
                           CORBA::ULong low_value,
                           ZIOP::CompressionMinRatioPolicyValue min_ratio);

  void connect_GIOPMessage(TIDorb::core::comm::Connection* conn);

private:

  void create_message_buffer_output(TIDorb::core::TIDORB* orb);




//   void* _impl();
//   const char* _typeid();

};
} //ziop
} //comm
} //core
} //TIDorb
#endif
