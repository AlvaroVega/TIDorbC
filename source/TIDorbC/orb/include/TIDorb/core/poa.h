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
