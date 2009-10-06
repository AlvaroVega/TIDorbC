///////////////////////////////////////////////////////////////////////////
//
// File:        MIOPPacket.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"

#ifndef _TIDorb_core_comm_miop_MIOPPacket_H_
#define _TIDorb_core_comm_miop_MIOPPacket_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace miop {


class MIOPPacket {

  protected:

  CORBA::Octet                       _flags;
  CORBA::ULong                       _number_of_packets;
  TIDorb::core::cdr::BufferCDR*      _buffer_cdr;
  TIDorb::core::cdr::CDROutputStream _output;

  public:

  static CORBA::ULong HEADER_SIZE;


  MIOPPacket(const TIDorb::core::comm::miop::Version& version,
             const TIDorb::core::comm::miop::UniqueId unique_id,
             TIDorb::core::cdr::BufferCDR* buffer,
             size_t num_packets);

  size_t preparePacket(unsigned char* buffer, size_t buflen,
                       size_t packet_number, size_t packet_length);

  unsigned char* getPacketBuffer();

};

}// miop
}// comm
}// core
}// TIDorb
#endif
