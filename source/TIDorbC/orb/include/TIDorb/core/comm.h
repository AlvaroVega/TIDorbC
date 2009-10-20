//////////////////////////////////////////////////////////////////////////////////
//
// File:        comm.h
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

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_COMM_H_
#define _TIDORB_CORE_COMM_H_

#include "TIDorb/core/comm/RECOVERABLE_COMM_FAILURE.h"

#include "TIDorb/core/comm/iiop.h"

#include "TIDorb/core/comm/CommunicationLayer.h"
#include "TIDorb/core/comm/LocalCommLayer.h"
#include "TIDorb/core/comm/IIOPCommLayer.h"
#include "TIDorb/core/comm/CommunicationManager.h"
#include "TIDorb/core/comm/OpeningLock.h"
#include "TIDorb/core/comm/ServerListener.h"
#include "TIDorb/core/comm/ReliableOnewayThread.h"
#include "TIDorb/core/comm/ConnectionState.h"
#include "TIDorb/core/comm/Connection.h"
#include "TIDorb/core/comm/ConnectionManager.h"
#include "TIDorb/core/comm/ConnectionThread.h"

// pra@tid.es - MIOP extensions
#include "TIDorb/core/comm/miop.h"
#include "TIDorb/core/comm/TCPConnection.h"
#include "TIDorb/core/comm/UDPConnection.h"
#include "TIDorb/core/comm/MulticastConnection.h"
#include "TIDorb/core/comm/MIOPCommLayer.h"
// end MIOP extensions

// pra@tid.es - FT extensions
#include "TIDorb/core/comm/FT.h"
#include "TIDorb/core/comm/HeartbeatThread.h"
// end FT extensions

#include "TIDorb/core/comm/ziop.h"

#endif
