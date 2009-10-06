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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
