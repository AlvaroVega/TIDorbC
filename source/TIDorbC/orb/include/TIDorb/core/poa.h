//////////////////////////////////////////////////////////////////////////////////
//
// File:        poa.h
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

#ifndef _TIDORB_CORE_POA_H_
#define _TIDORB_CORE_POA_H_


#include "TIDorb/core/poa/policies.h"

#include "TIDorb/core/poa/OID.h"
#include "TIDorb/core/poa/SystemOID.h"
#include "TIDorb/core/poa/CompletionWaiter.h"
#include "TIDorb/core/poa/ActiveObjectMap.h"
#include "TIDorb/core/poa/POAPath.h"
#include "TIDorb/core/poa/POAKey.h"
#include "TIDorb/core/poa/POAImpl.h"
#include "TIDorb/core/poa/QueuedRequest.h"
#include "TIDorb/core/poa/AbstractIIOPRequest.h"
#include "TIDorb/core/poa/IIOPRequest.h"
#include "TIDorb/core/poa/IIOPLocateRequest.h"
#include "TIDorb/core/poa/AbstractLocalRequest.h"
#include "TIDorb/core/poa/LocalLocateRequest.h"
#include "TIDorb/core/poa/LocalRequest.h"
#include "TIDorb/core/poa/POAManagerConfListener.h"
#include "TIDorb/core/poa/ThreadStateListener.h"
#include "TIDorb/core/poa/QueueReaderManager.h"
#include "TIDorb/core/poa/ThreadPool.h"
#include "TIDorb/core/poa/POAManagerConf.h"
#include "TIDorb/core/poa/POAManagerImpl.h"
#include "TIDorb/core/poa/CurrentInfo.h"
#include "TIDorb/core/poa/CurrentImpl.h"
#include "TIDorb/core/poa/RequestQueue.h"
#include "TIDorb/core/poa/ServantDelegate.h"
#include "TIDorb/core/poa/ExecThread.h"

// pra@tid.es - MIOP extensions
#include "TIDorb/core/goa/GOAImpl.h"
#include "TIDorb/core/goa/AbstractMIOPRequest.h"
#include "TIDorb/core/goa/MIOPRequest.h"
// end MIOP extensions


#endif
