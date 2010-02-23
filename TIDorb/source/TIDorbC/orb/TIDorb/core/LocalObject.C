/////////////////////////////////////////////////////////////////////////
//
// File:        LocalObject.C
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
#include <string.h>

//MLG
void* CORBA::LocalObject::_impl()
{
	return this;
}

const char* CORBA::LocalObject::_typeid()
{
	//return CORBA::string_dup("LocalObject");
	return "LocalObject";
}
//EMLG

CORBA::LocalObject::LocalObject()
{
}


CORBA::LocalObject::~LocalObject()
{
}


void CORBA::LocalObject::_add_ref()
{
  try{
    TIDThr::RefCounter* counter = dynamic_cast<TIDThr::RefCounter*> (this);
    if(counter) { 
      counter->_add_ref(); 
      return;
    }
  } catch (...) { 
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}


void CORBA::LocalObject::_remove_ref()
{
  try{
    TIDThr::RefCounter* counter = dynamic_cast<TIDThr::RefCounter*> (this);
    if(counter) { 
      counter->_remove_ref(); 
      return;
    }
  } catch (...) { 
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}


::CORBA::InterfaceDef_ptr CORBA::LocalObject::_get_interface()
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


::CORBA::Boolean CORBA::LocalObject::_is_a(const char* logical_type_id)
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


::CORBA::Boolean CORBA::LocalObject::_non_existent()
{
  return false;
}


::CORBA::Boolean CORBA::LocalObject::_is_equivalent(::CORBA::Object_ptr other_object)
{
  return (this == other_object);
}


::CORBA::ULong CORBA::LocalObject::_hash(::CORBA::ULong maximum)
{
  uintptr_t ptr = (uintptr_t) this;
  return ((CORBA::ULong) ptr) % maximum;
}


void CORBA::LocalObject::_create_request(::CORBA::Context_ptr ctx,
                                         const char *operation,
                                         ::CORBA::NVList_ptr arg_list,
                                         ::CORBA::NamedValue_ptr result,
                                         ::CORBA::Request_out request,
                                         ::CORBA::Flags req_flags)
{
  throw CORBA::NO_IMPLEMENT(4, CORBA::COMPLETED_NO);
}


void CORBA::LocalObject::_create_request(::CORBA::Context_ptr ctx,
                                         const char *operation,
                                         ::CORBA::NVList_ptr arg_list,
                                         ::CORBA::NamedValue_ptr result,
                                         ::CORBA::ExceptionList_ptr ex_list,
                                         ::CORBA::ContextList_ptr ctx_list,
                                         ::CORBA::Request_out request,
                                         ::CORBA::Flags req_flags)
{
  throw CORBA::NO_IMPLEMENT(4, CORBA::COMPLETED_NO);
}


::CORBA::Request_ptr CORBA::LocalObject::_request(const char* operation)
{
  throw CORBA::NO_IMPLEMENT(4, CORBA::COMPLETED_NO);
}


::CORBA::Policy_ptr CORBA::LocalObject::_get_policy(::CORBA::PolicyType policy_type)
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


::CORBA::DomainManagerList* CORBA::LocalObject::_get_domain_managers()
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


::CORBA::Object_ptr
CORBA::LocalObject::_set_policy_overrides(const ::CORBA::PolicyList& policies,
                                          ::CORBA::SetOverrideType set_or_add)
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}



CORBA::Policy_ptr
CORBA::LocalObject::_get_client_policy(CORBA::PolicyType policy_type)
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


CORBA::PolicyList_ptr
CORBA::LocalObject::_get_policy_overrides(const CORBA::PolicyTypeSeq& types)
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


CORBA::Boolean
CORBA::LocalObject::_validate_connection(CORBA::PolicyList_out inconsistent_policies)
{
  throw CORBA::NO_IMPLEMENT(3, CORBA::COMPLETED_NO);
}


::CORBA::LocalObject_ptr
CORBA::LocalObject::_narrow(::CORBA::Object_ptr obj)
{
  //TODO jagd
  CORBA::LocalObject* local = dynamic_cast< CORBA::LocalObject* > (obj);

  if(!local)
    return NULL;

  local->_add_ref();

  return local;
}


::CORBA::LocalObject_ptr
CORBA::LocalObject::_duplicate(CORBA::LocalObject_ptr local)
{
  if(!local)
    return NULL;

  local->_add_ref();

  return local;
}


::CORBA::LocalObject_ptr CORBA::LocalObject::_nil()
{
  return NULL;
}
