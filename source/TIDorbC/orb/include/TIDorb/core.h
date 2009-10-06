///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/core.h
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

#ifndef _TIDORB_CORE_H
#define _TIDORB_CORE_H

#include "TIDorb/config.h"
#include "TIDorb/util.h"

#include "CORBA.h"
#include "PortableServer.h"
#include "BiDirPolicy.h"

#include "TIDorb/core_fw.h"

#include "TIDorb/core/util.h"

#include "TIDorb/core/BidirectionalPolicyImpl.h"

#include "TIDorb/core/PolicyFactory.h"
#include "TIDorb/core/PolicyContext.h"
#include "TIDorb/core/PolicyManagerImpl.h"
#include "TIDorb/core/PolicyCurrentImpl.h"
#include "TIDorb/core/PolicyContextManagerImpl.h"

#include "TIDorb/core/ConfORB.h"
#include "TIDorb/core/ORBServices.h"
#include "TIDorb/core/ProcessingState.h"
#include "TIDorb/core/TIDORB.h"
#include "TIDorb/core/ORBComponent.h"

#include "TIDorb/core/cdr.h"
#include "TIDorb/core/iop.h"
#include "TIDorb/core/typecode.h"

#include "TIDorb/core/ForwardRequest.h"

#include "TIDorb/core/comm.h"

#include "TIDorb/core/poa.h"

#include "TIDorb/core/ShutdownThread.h"
#include "TIDorb/core/AnyImpl.h"
#include "TIDorb/core/ContextListImpl.h"
#include "TIDorb/core/ContextImpl.h"
#include "TIDorb/core/EnvironmentImpl.h"
#include "TIDorb/core/NamedValueImpl.h"
#include "TIDorb/core/NVListImpl.h"
#include "TIDorb/core/ExceptionListImpl.h"
#include "TIDorb/core/RequestImpl.h"
#include "TIDorb/core/ServerRequestStatus.h"
#include "TIDorb/core/ServerRequestImpl.h"
#include "TIDorb/core/LocalServerRequest.h"
#include "TIDorb/core/ObjectImpl.h"
#include "TIDorb/core/ObjectDelegateImpl.h"

#include "TIDorb/core/CodecImpl.h"
#include "TIDorb/core/CodecFactoryImpl.h"

// FT extensions
#include "TIDorb/core/HeartbeatPolicyImpl.h"
#include "TIDorb/core/HeartbeatEnabledPolicyImpl.h"

// QoS extensions
#include "TIDorb/core/messaging.h"

// ZIOP Compression
#include "TIDorb/core/ziop.h"
#include "TIDorb/core/compression.h"

#include "TIDorb/core/RequestCounter.h"

#endif
