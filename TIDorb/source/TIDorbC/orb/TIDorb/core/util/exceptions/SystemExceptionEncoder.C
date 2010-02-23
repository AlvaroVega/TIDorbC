/////////////////////////////////////////////////////////////////////////
//
// File:        SystemExceptionEncoder.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/util/exceptions.h"

const TIDorb::core::util::exceptions::SystemExceptionCreatorTable*
  TIDorb::core::util::exceptions::SystemExceptionEncoder::st_creators_table =
    TIDorb::core::util::exceptions::SystemExceptionEncoder::init_creators();

TIDorb::core::util::exceptions::SystemExceptionCreatorTable*
TIDorb::core::util::exceptions::SystemExceptionEncoder::init_creators()
{

  SystemExceptionCreatorTable* creators_table = new SystemExceptionCreatorTable();

  (*creators_table)[BAD_CONTEXT_id] = new BAD_CONTEXT_Creator();
  (*creators_table)[BAD_INV_ORDER_id] = new BAD_INV_ORDER_Creator();
  (*creators_table)[BAD_OPERATION_id] = new BAD_OPERATION_Creator();
  (*creators_table)[BAD_PARAM_id] = new BAD_PARAM_Creator();
  (*creators_table)[BAD_TYPECODE_id] = new BAD_TYPECODE_Creator();
  (*creators_table)[COMM_FAILURE_id] = new COMM_FAILURE_Creator();
  (*creators_table)[DATA_CONVERSION_id] = new DATA_CONVERSION_Creator();
  (*creators_table)[FREE_MEM_id] = new FREE_MEM_Creator();
  (*creators_table)[IMP_LIMIT_id] = new IMP_LIMIT_Creator();
  (*creators_table)[INITIALIZE_id] = new INITIALIZE_Creator();
  (*creators_table)[INTERNAL_id] = new INTERNAL_Creator();
  (*creators_table)[INTF_REPOS_id] = new INTF_REPOS_Creator();
  (*creators_table)[INV_FLAG_id] = new INV_FLAG_Creator();
  (*creators_table)[INV_IDENT_id] = new INV_IDENT_Creator();
  (*creators_table)[INV_OBJREF_id] = new INV_OBJREF_Creator();
  (*creators_table)[INV_POLICY_id] = new INV_POLICY_Creator();
  (*creators_table)[INVALID_TRANSACTION_id] = new INVALID_TRANSACTION_Creator();
  (*creators_table)[MARSHAL_id] = new MARSHAL_Creator();
  (*creators_table)[NO_IMPLEMENT_id] = new NO_IMPLEMENT_Creator();
  (*creators_table)[NO_MEMORY_id] = new NO_MEMORY_Creator();
  (*creators_table)[NO_PERMISSION_id] = new NO_PERMISSION_Creator();
  (*creators_table)[NO_RESOURCES_id] = new NO_RESOURCES_Creator();
  (*creators_table)[NO_RESPONSE_id] = new NO_RESPONSE_Creator();
  (*creators_table)[OBJECT_NOT_EXIST_id] = new OBJECT_NOT_EXIST_Creator();
  (*creators_table)[OBJ_ADAPTER_id] = new OBJ_ADAPTER_Creator();
  (*creators_table)[PERSIST_STORE_id] = new PERSIST_STORE_Creator();
  (*creators_table)[TRANSACTION_REQUIRED_id] = new TRANSACTION_REQUIRED_Creator();
  (*creators_table)[TRANSACTION_ROLLEDBACK_id] = new TRANSACTION_ROLLEDBACK_Creator();
  (*creators_table)[TRANSIENT_id] = new TRANSIENT_Creator();
  (*creators_table)[UNKNOWN_id] = new UNKNOWN_Creator();
  (*creators_table)[TIMEOUT_id] = new TIMEOUT_Creator();

  (*creators_table)[BAD_QOS_id] = new BAD_QOS_Creator();
  (*creators_table)[TRANSACTION_MODE_id] = new TRANSACTION_MODE_Creator();
  (*creators_table)[TRANSACTION_UNAVAILABLE_id] = new TRANSACTION_UNAVAILABLE_Creator();
  (*creators_table)[REBIND_id] = new REBIND_Creator();
  (*creators_table)[CODESET_INCOMPATIBLE_id] = new CODESET_INCOMPATIBLE_Creator();

  return creators_table;
}


void TIDorb::core::util::exceptions::SystemExceptionEncoder::write(TIDorb::portable::OutputStream& out,
const CORBA::SystemException& sys)
{
  out.write_string(sys._rep_id());
  out.write_ulong(sys.minor());
  out.write_ulong(sys.completed());
}


CORBA::SystemException*
TIDorb::core::util::exceptions::SystemExceptionEncoder::read(TIDorb::portable::InputStream& in)
{
  char * name;
  CORBA::ULong minor;
  CORBA::ULong completed;

  in.read_string(name);
  try {
    in.read_ulong(minor);
    in.read_ulong(completed);
  } catch (...) {
    CORBA::string_free(name);
    throw;
  }

  CORBA::CompletionStatus completion = (CORBA::CompletionStatus) completed;

  SystemExceptionCreatorTable::const_iterator p = st_creators_table->find(name);

  CORBA::string_free(name);

  if(p == st_creators_table->end())
  throw CORBA::MARSHAL(); //"Invalid System Exception Name");

  return  ((*p).second)->create(minor, completion);

}

CORBA::SystemException*
TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(const CORBA::SystemException& se)
{
  SystemExceptionCreatorTable::const_iterator p = st_creators_table->find(se._rep_id());

  if(p == st_creators_table->end())
  throw CORBA::INTERNAL("Invalid System Exception Name");

  return  ((*p).second)->copy(se);

}

CORBA::SystemException*
TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(const char* rep_id)
{
  SystemExceptionCreatorTable::const_iterator p = st_creators_table->find(rep_id);

  if(p == st_creators_table->end())
    throw CORBA::INTERNAL("Invalid System Exception Name");

  return  ((*p).second)->create(0, CORBA::COMPLETED_NO);

}
