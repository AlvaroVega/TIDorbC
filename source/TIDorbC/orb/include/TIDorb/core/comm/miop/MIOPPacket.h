///////////////////////////////////////////////////////////////////////////
//
// File:        MIOPPacket.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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
