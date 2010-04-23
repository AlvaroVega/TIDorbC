//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServiceId.C
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

#include "TIDorb/core/comm/iiop/ServiceId.h"

const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::TransactionService = 0;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::CodeSets = 1;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::ChainBypassCheck = 2;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::ChainBypassInfo = 3;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::LogicalThreadId = 4;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::BI_DIR_IIOP = 5;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::SendingContextRunTime = 6;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::INVOCATION_POLICIES = 7;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::FORWARDED_ENTITY = 8;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::UnknownExceptionInfo = 9;

const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::SecurityAttributeService = 10;
