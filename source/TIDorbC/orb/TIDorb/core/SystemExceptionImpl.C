/////////////////////////////////////////////////////////////////////////
//
// File:        SystemExceptionImpl.C
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

#include "CORBA.h"

#include "TIDorb/core/util.h"

using TIDorb::core::util::exceptions::SystemExceptionType;
using TIDorb::core::util::SystemExceptionHolder;

namespace CORBA {
const CORBA::TypeCode_ptr _tc_BAD_CONTEXT = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::BAD_CONTEXT_id); 
const CORBA::TypeCode_ptr _tc_BAD_INV_ORDER = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::BAD_INV_ORDER_id);
const CORBA::TypeCode_ptr _tc_BAD_OPERATION = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::BAD_OPERATION_id); 
const CORBA::TypeCode_ptr _tc_BAD_PARAM = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::BAD_PARAM_id); 
const CORBA::TypeCode_ptr _tc_BAD_TYPECODE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::BAD_TYPECODE_id); 
const CORBA::TypeCode_ptr _tc_COMM_FAILURE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::COMM_FAILURE_id); 
const CORBA::TypeCode_ptr _tc_DATA_CONVERSION = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::DATA_CONVERSION_id);
const CORBA::TypeCode_ptr _tc_FREE_MEM = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::FREE_MEM_id); 
const CORBA::TypeCode_ptr _tc_IMP_LIMIT = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::IMP_LIMIT_id);
const CORBA::TypeCode_ptr _tc_INITIALIZE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INITIALIZE_id); 
const CORBA::TypeCode_ptr _tc_INTERNAL = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INTERNAL_id); 
const CORBA::TypeCode_ptr _tc_INTF_REPOS = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INTF_REPOS_id); 
const CORBA::TypeCode_ptr _tc_INV_FLAG = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INV_FLAG_id); 
const CORBA::TypeCode_ptr _tc_INV_IDENT = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INV_IDENT_id); 
const CORBA::TypeCode_ptr _tc_INV_OBJREF= 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INV_OBJREF_id); 
const CORBA::TypeCode_ptr _tc_INV_POLICY = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INV_POLICY_id); 
const CORBA::TypeCode_ptr _tc_INVALID_TRANSACTION = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::INVALID_TRANSACTION_id); 
const CORBA::TypeCode_ptr _tc_MARSHAL = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::MARSHAL_id); 
const CORBA::TypeCode_ptr _tc_NO_IMPLEMENT = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::NO_IMPLEMENT_id); 
const CORBA::TypeCode_ptr _tc_NO_MEMORY = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::NO_MEMORY_id); 
const CORBA::TypeCode_ptr _tc_NO_PERMISSION = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::NO_PERMISSION_id);
const CORBA::TypeCode_ptr _tc_NO_RESOURCES = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::NO_RESOURCES_id); 
const CORBA::TypeCode_ptr _tc_NO_RESPONSE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::NO_RESPONSE_id);
const CORBA::TypeCode_ptr _tc_OBJECT_NOT_EXIST = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::OBJECT_NOT_EXIST_id); 
const CORBA::TypeCode_ptr _tc_OBJ_ADAPTER = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::OBJ_ADAPTER_id); 
const CORBA::TypeCode_ptr _tc_PERSIST_STORE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::PERSIST_STORE_id); 
const CORBA::TypeCode_ptr _tc_TRANSACTION_REQUIRED = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::TRANSACTION_REQUIRED_id); 
const CORBA::TypeCode_ptr _tc_TRANSACTION_ROLLEDBACK = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::TRANSACTION_ROLLEDBACK_id); 
const CORBA::TypeCode_ptr _tc_TRANSIENT = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::TRANSIENT_id); 
const CORBA::TypeCode_ptr _tc_UNKNOWN = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::UNKNOWN_id); 
const CORBA::TypeCode_ptr _tc_TIMEOUT = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::TIMEOUT_id); 
const CORBA::TypeCode_ptr _tc_REBIND = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::REBIND_id); 
const CORBA::TypeCode_ptr _tc_CODESET_INCOMPATIBLE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::CODESET_INCOMPATIBLE_id); 
const CORBA::TypeCode_ptr _tc_TRANSACTION_MODE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::TRANSACTION_MODE_id); 
const CORBA::TypeCode_ptr _tc_TRANSACTION_UNAVAILABLE = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::TRANSACTION_UNAVAILABLE_id); 
const CORBA::TypeCode_ptr _tc_BAD_QOS = 
    SystemExceptionType::create( TIDorb::core::util::exceptions::BAD_QOS_id); 
}

::CORBA::Boolean operator>>=(const ::CORBA::Any& any, const ::CORBA::SystemException*& se)
{
     ::TIDorb::portable::Any& delegate=any.delegate();
     const TIDorb::portable::Streamable* _holder;
     
     if(delegate.extract_Streamable(_holder))
     {
        const SystemExceptionHolder* _hld=dynamic_cast< const SystemExceptionHolder*>(_holder);
        if(_hld){
            (CORBA::SystemException*&)se = (CORBA::SystemException*) _hld->value();
            return true;
        } // Dynamic Cast != null
        else 
          return false;
     } // has Streamable

    CORBA::TypeCode_ptr type = any.type();
    SystemExceptionHolder* _hld = new SystemExceptionHolder(type->id());
    CORBA::release(type);
    try {
        CORBA::Boolean ret = delegate.set_Streamable(_hld);
        (CORBA::SystemException*&)se = (CORBA::SystemException*) _hld->value();
        if (!ret) // if ret is true delegate consumes holder 
          delete _hld; 
        return ret;
    } catch (CORBA::BAD_OPERATION _e) {
        delete _hld;
        return false;
    }
    
  return false;
}


void operator<<=(::CORBA::Any& any, const ::CORBA::SystemException& se)
{
    any.delegate().insert_Streamable(new SystemExceptionHolder(se));    
}

void operator<<=(::CORBA::Any& any, ::CORBA::SystemException* se)
{
    if(!se)
        throw CORBA::BAD_PARAM();
        
    any.delegate().insert_Streamable(new SystemExceptionHolder(se));    
    
}


ostream& operator<<(ostream& os, const ::CORBA::SystemException* se)
{
#ifdef minor
#define __CORBA_SYSTEM_EXCEPTION_MINOR_ minor
#undef minor
#endif
  os << "CORBA::" << se->_name() << "  minor code: " << se->minor() << " completed: ";
#ifdef __CORBA_SYSTEM_EXCEPTION_MINOR__
#define minor __CORBA_SYSTEM_EXCEPTION_MINOR__
#undef __CORBA_SYSTEM_EXCEPTION_MINOR__
#endif

  switch (se->completed()) {
  case CORBA::COMPLETED_NO:
    os << "COMPLETED_NO";
    break;
   case CORBA::COMPLETED_YES:
    os << "COMPLETED_YES";
    break;
  case CORBA::COMPLETED_MAYBE:
    os << "COMPLETED_MAYBE";
    break;
  }
  
  if(se->what()) {
    os << " reason: " << se->what();
  }

  return os;
}


ostream& operator<<(ostream& os, const ::CORBA::SystemException& se)
{
  return os << &se;
}
