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
