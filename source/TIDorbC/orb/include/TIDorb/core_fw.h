///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/core_fw.h
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
