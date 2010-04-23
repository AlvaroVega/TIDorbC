///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/core.h
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
#include "TIDorb/core/ConstructionPolicyImpl.h"
#include "TIDorb/core/DomainManagerImpl.h"

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

#include "TIDorb/core/security.h"

#include "TIDorb/core/RequestCounter.h"

#endif
