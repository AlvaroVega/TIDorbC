/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        exceptions.h
// Description: 
//
// Rel:         01.00
// Created:     
// Author:      
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
///////////////////////////////////////////////////////////////////////////////////*/


#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_H_

#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::bad_cast;
#endif

#include "TIDorb/core/util/exceptions/SystemExceptionNames.h"
#include "TIDorb/core/util/exceptions/SystemExceptionCreator.h"
#include "TIDorb/core/util/exceptions/SystemExceptionEncoder.h"
#include "TIDorb/core/util/exceptions/SystemExceptionType.h"

#include "TIDorb/core/util/exceptions/BAD_CONTEXT_Creator.h"
#include "TIDorb/core/util/exceptions/BAD_INV_ORDER_Creator.h"
#include "TIDorb/core/util/exceptions/BAD_OPERATION_Creator.h"
#include "TIDorb/core/util/exceptions/BAD_PARAM_Creator.h"
#include "TIDorb/core/util/exceptions/BAD_TYPECODE_Creator.h"
#include "TIDorb/core/util/exceptions/COMM_FAILURE_Creator.h"
#include "TIDorb/core/util/exceptions/DATA_CONVERSION_Creator.h"
#include "TIDorb/core/util/exceptions/FREE_MEM_Creator.h"
#include "TIDorb/core/util/exceptions/IMP_LIMIT_Creator.h"
#include "TIDorb/core/util/exceptions/INITIALIZE_Creator.h"
#include "TIDorb/core/util/exceptions/INTERNAL_Creator.h"
#include "TIDorb/core/util/exceptions/INTF_REPOS_Creator.h"
#include "TIDorb/core/util/exceptions/INV_FLAG_Creator.h"
#include "TIDorb/core/util/exceptions/INV_IDENT_Creator.h"
#include "TIDorb/core/util/exceptions/INV_OBJREF_Creator.h"
#include "TIDorb/core/util/exceptions/INV_POLICY_Creator.h"
#include "TIDorb/core/util/exceptions/INVALID_TRANSACTION_Creator.h"
#include "TIDorb/core/util/exceptions/MARSHAL_Creator.h"
#include "TIDorb/core/util/exceptions/NO_IMPLEMENT_Creator.h"
#include "TIDorb/core/util/exceptions/NO_MEMORY_Creator.h"
#include "TIDorb/core/util/exceptions/NO_PERMISSION_Creator.h"
#include "TIDorb/core/util/exceptions/NO_RESOURCES_Creator.h"
#include "TIDorb/core/util/exceptions/NO_RESPONSE_Creator.h"
#include "TIDorb/core/util/exceptions/OBJ_ADAPTER_Creator.h"
#include "TIDorb/core/util/exceptions/OBJECT_NOT_EXIST_Creator.h"
#include "TIDorb/core/util/exceptions/PERSIST_STORE_Creator.h"
#include "TIDorb/core/util/exceptions/TRANSACTION_REQUIRED_Creator.h"
#include "TIDorb/core/util/exceptions/TRANSACTION_ROLLEDBACK_Creator.h"
#include "TIDorb/core/util/exceptions/TRANSIENT_Creator.h"
#include "TIDorb/core/util/exceptions/UNKNOWN_Creator.h"
#include "TIDorb/core/util/exceptions/TIMEOUT_Creator.h"
#include "TIDorb/core/util/exceptions/BAD_QOS_Creator.h"
#include "TIDorb/core/util/exceptions/TRANSACTION_MODE_Creator.h"
#include "TIDorb/core/util/exceptions/TRANSACTION_UNAVAILABLE_Creator.h"
#include "TIDorb/core/util/exceptions/REBIND_Creator.h"
#include "TIDorb/core/util/exceptions/CODESET_INCOMPATIBLE_Creator.h"

#endif
