///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/core_fw.h
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

#ifndef _TIDORB_CORE_FW_H
#define _TIDORB_CORE_FW_H

#include "TIDorb/config.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {

  class TIDORB;
  typedef TIDThr::HandleT<TIDORB> TIDORB_ref;
  
  class ORBServices;
  class ObjectImpl;
  class ObjectDelegateImpl;
  class RequestImpl;
  class ServerRequestImpl;  
  class LocalServerRequest;
  class ConfORB;
  class AnyImpl;
  class SingletonORB;
  class ContextListImpl;
  class ContextImpl;
  class EnvironmentImpl;
  class NamedValueImpl;
  class NVListImpl;
  class ExceptionListImpl;
  class ORBComponent;
  class ObjectDelegate;
  class ObjectDelegateImpl;

  class ProcessingState;
  class ShutdownThread;
  typedef TIDThr::HandleT<ShutdownThread> ShutdownThread_ref;

  class ForwardRequest;

  class Codec;
  class CodecFactory;
  class BidirectionalPolicyImpl;
  
  class PolicyFactory;
  class PolicyContext;
  class PolicyManagerImpl;
  class PolicyCurrentImpl;
  class PolicyContextManagerImpl;

  // FT extensions
  class HeartbeatPolicyImpl;
  class HeartbeatEnabledPolicyImpl;

  class RequestCounter;

}
}

#include "TIDorb/core/cdr_fw.h"
#include "TIDorb/core/iop_fw.h"
#include "TIDorb/core/comm_fw.h"
#include "TIDorb/core/typecode_fw.h"
#include "TIDorb/core/messaging_fw.h"
#include "TIDorb/core/compression_fw.h"
#include "TIDorb/core/ziop_fw.h"
#include "TIDorb/core/poa_fw.h"



#endif
