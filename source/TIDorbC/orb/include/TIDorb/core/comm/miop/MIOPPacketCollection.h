///////////////////////////////////////////////////////////////////////////
//
// File:        MIOPPacketCollection.h
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

#ifndef _TIDorb_core_comm_miop_MIOPPacketCollection_H_
#define _TIDorb_core_comm_miop_MIOPPacketCollection_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace miop {

class MIOPPacketCollection: public TIDorb::core::util::RemovableObject{
public:
  CORBA::ULong number_chunks;
  CORBA::ULong number_chunks_inserted;
  TIDorb::core::cdr::ChunkCDR** chunks;
 
  bool canBeRemoved() {
    return true;
  }
  MIOPPacketCollection()
    : number_chunks(0), number_chunks_inserted(0), chunks(0) {}
  ~MIOPPacketCollection() {}
};

} //miop
} //comm
} //core
} //TIDorb

#endif
