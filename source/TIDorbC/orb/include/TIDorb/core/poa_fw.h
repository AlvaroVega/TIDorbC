//////////////////////////////////////////////////////////////////////////////////
//
// File:        poa_fw.h
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

#ifndef _TIDORB_CORE_POA_FW_H_
#define _TIDORB_CORE_POA_FW_H_

#include "TIDThr.h"

// Forward Declarations

namespace TIDorb {
namespace core {
namespace poa {
  
  class POAKey;
  typedef TIDThr::HandleT<POAKey> POAKey_ref;
  
  class OID;
  class SystemOID;
  class CompletionWaiter;
  class ActiveObjectMap;
  class POAImpl;
  class QueuedRequest;
  class AbstractIIOPRequest;
  class IIOPRequest;
  class IIOPLocateRequest;
  class AbstractLocalRequest;
  class LocalLocateRequest;
  class LocalRequest;
  class POAManagerConfListener;
  class QueueReaderManager;
  class ThreadStateListener;
  class ThreaPool;
  class POAManagerConf;
  class POAManagerImpl;
  class CurrentImpl;
  class RequestQueue;
  class ServantDelegate;
  class ExecThread;
  typedef TIDThr::HandleT<ExecThread> ExecThread_ref;
}
}
}

#include "TIDorb/core/poa/policies_fw.h"


// pra@tid.es - MIOP extensions
namespace TIDorb {
namespace core {
namespace goa {
  class GOAImpl;
  class AbstractMIOPRequest;
  class MIOPRequest;
}
}
}
// end MIOP extensions


#endif
