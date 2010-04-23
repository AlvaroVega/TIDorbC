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


// MIOP extensions
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
