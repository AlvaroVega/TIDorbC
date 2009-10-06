//////////////////////////////////////////////////////////////////////////////////
//
// File:        comm_fw.h
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

#ifndef _TIDORB_CORE_COMM_FW_H_
#define _TIDORB_CORE_COMM_FW_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace comm {
  class CommunicationLayer; 
  typedef TIDThr::HandleT<CommunicationLayer> CommunicationLayer_ref; 
  class IIOPCommLayer;  
  typedef TIDThr::HandleT<IIOPCommLayer> IIOPCommLayer_ref;
  class LocalCommLayer;
  typedef TIDThr::HandleT<LocalCommLayer> LocalCommLayer_ref;
  class CommunicationManager;
  typedef TIDThr::HandleT<CommunicationManager> CommunicationManager_ref;
  class ServerListener;
  typedef TIDThr::HandleT<ServerListener> ServerListener_ref;
  class FragmentedMessageHolder;
  class ReliableOnewayThread;
  
  class Connection;
  //PRA
  //typedef TIDThr::HandleT<Connection> Connection_ref;
  class Connection_ref;
  //EPRA
  class ConnectionThread;
  class ConnectionState;
  class OpeningLock;
  typedef TIDThr::HandleT<OpeningLock> OpeningLock_ref;
  class ConnectionManager;
  typedef TIDThr::HandleT<ConnectionManager> ConnectionManager_ref;

  // pra@tid.es - MIOP extensions
  class TCPConnection;
  typedef TIDThr::HandleT<TCPConnection> TCPConnection_ref;
  class UDPConnection;
  typedef TIDThr::HandleT<UDPConnection> UDPConnection_ref;
  class MulticastConnection;
  typedef TIDThr::HandleT<MulticastConnection> MulticastConnection_ref;
  class MIOPCommLayer;
  typedef TIDThr::HandleT<MIOPCommLayer> MIOPCommLayer_ref;
  // end MIOP extensions

  // pra@tid.es - FT extensions
  class HeartbeatThread;
  // end FT extensions

}
}
}

#include "TIDorb/core/comm/iiop_fw.h"

// pra@tid.es - MIOP extensions
#include "TIDorb/core/comm/miop_fw.h"
// end MIOP extensions

// pra@tid.es - FT extensions
#include "TIDorb/core/comm/FT_fw.h"
// end FT extensions


#include "TIDorb/core/comm/ziop_fw.h"

#endif
