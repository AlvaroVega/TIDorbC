/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        
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

#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_NAMES_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_NAMES_H_

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {

  extern const char* BAD_CONTEXT_id; 
  extern const char* BAD_INV_ORDER_id;
  extern const char* BAD_OPERATION_id;
  extern const char* BAD_PARAM_id;
  extern const char* BAD_TYPECODE_id;
  extern const char* COMM_FAILURE_id;
  extern const char* DATA_CONVERSION_id;
  extern const char* FREE_MEM_id; 
  extern const char* IMP_LIMIT_id;
  extern const char* INITIALIZE_id; 
  extern const char* INTERNAL_id; 
  extern const char* INTF_REPOS_id;
  extern const char* INV_FLAG_id; 
  extern const char* INV_IDENT_id;
  extern const char* INV_OBJREF_id;
  extern const char* INV_POLICY_id;
  extern const char* INVALID_TRANSACTION_id;
  extern const char* MARSHAL_id; 
  extern const char* NO_IMPLEMENT_id;
  extern const char* NO_MEMORY_id; 
  extern const char* NO_PERMISSION_id;
  extern const char* NO_RESOURCES_id;
  extern const char* NO_RESPONSE_id; 
  extern const char* OBJECT_NOT_EXIST_id;
  extern const char* OBJ_ADAPTER_id;
  extern const char* PERSIST_STORE_id;
  extern const char* TRANSACTION_REQUIRED_id;
  extern const char* TRANSACTION_ROLLEDBACK_id;
  extern const char* TRANSIENT_id;
  extern const char* UNKNOWN_id;
  extern const char* TIMEOUT_id;
  extern  const char* BAD_QOS_id;
  extern  const char* TRANSACTION_MODE_id;
  extern  const char* TRANSACTION_UNAVAILABLE_id;
  extern  const char* REBIND_id;
  extern  const char* CODESET_INCOMPATIBLE_id;



} // exceptions
} // util
} // core
} // TIDorb

#endif
