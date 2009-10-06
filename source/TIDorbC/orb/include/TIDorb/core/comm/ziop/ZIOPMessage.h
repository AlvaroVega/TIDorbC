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
// Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
