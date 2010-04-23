/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectImpl.C
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

#include "TIDorb/core.h"

CORBA::RepositoryIdSeq_ptr TIDorb::core::ObjectImpl::__init_ids()
{
  CORBA::RepositoryIdSeq_ptr ids = new CORBA::RepositoryIdSeq();
  ids->length(0);
  return ids;
}


const CORBA::RepositoryIdSeq_ptr TIDorb::core::ObjectImpl::__ids =
  TIDorb::core::ObjectImpl::__init_ids();


const CORBA::RepositoryIdSeq_ptr TIDorb::core::ObjectImpl::_ids()
{
  return __ids;
}


CORBA::Object_ptr CORBA::Object::_duplicate(CORBA::Object_ptr ref)
{

  if(!ref)
    return ref;

  try {
    TIDorb::portable::Stub* stub = dynamic_cast<TIDorb::portable::Stub*> (ref);

    if(stub) {
      stub->_add_ref();
      return stub;
    }

    CORBA::LocalObject* local = dynamic_cast<CORBA::LocalObject*> (ref);

    if(local) {
      local->_add_ref();
      return local;
    }

  } catch (...) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  return NULL;
}


CORBA::Object_ptr CORBA::Object::_nil()
{
  return NULL;
}


void CORBA::release(CORBA::Object_ptr ref)
{
  if(!ref)
    return;

  try {
    TIDorb::portable::Stub* stub = dynamic_cast<TIDorb::portable::Stub*> (ref);

    if(stub) {
      stub->_remove_ref();
      return;
    }

    CORBA::LocalObject* local = dynamic_cast<CORBA::LocalObject*> (ref);

    if(local) {
      local->_remove_ref();
    }

  } catch (...) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}


CORBA::Boolean CORBA::is_nil(CORBA::Object_ptr ref)
{
  return !ref;
}


TIDorb::core::ObjectImpl*
TIDorb::core::ObjectImpl::fromIOR(TIDorb::core::TIDORB* orb, 
                                  TIDorb::core::iop::IOR* ior)
{
  ObjectImpl* obj = new ObjectImpl();

  obj->m_delegate = orb->getCommunicationManager()->createDelegate(ior);

  return obj;
}


CORBA::Policy_ptr TIDorb::core::ObjectImpl::_get_policy(CORBA::PolicyType policy_type)
{
  ObjectDelegateImpl* delegate = (ObjectDelegateImpl*) (m_delegate);
  
  if(delegate) {
    TIDorb::core::PolicyContext* policy_context = NULL;
    policy_context = delegate->getPolicyContext(); 
    
    if (policy_context != NULL){
      return policy_context->getPolicy(policy_type);
    }
  } 
  else
    throw CORBA::INTERNAL("ObjectDelegate not found");
}

CORBA::DomainManagerList* TIDorb::core::ObjectImpl::_get_domain_managers()
{
  ObjectDelegateImpl* delegate = (ObjectDelegateImpl*) (m_delegate);
  
  if(delegate)
    return delegate->getDomainManagerList();   
  else
    throw CORBA::INTERNAL("ObjectDelegate not found");
}



CORBA::Object_ptr 
TIDorb::core::ObjectImpl::_set_policy_overrides(const CORBA::PolicyList& policies,
                                                CORBA::SetOverrideType set_or_add)
{
  ObjectDelegateImpl* delegate = (ObjectDelegateImpl*) (m_delegate);

  if(delegate) {

    // Make a deep copy of current object reference
    ObjectImpl* obj_copy = new ObjectImpl;
    ObjectDelegateImpl* delegate_copy = delegate->copy();
    obj_copy->_set_delegate(delegate_copy);

    TIDorb::core::PolicyContext* policy_context = NULL;
    policy_context = delegate_copy->getPolicyContext();

    ///delegate_copy->_remove_ref(); /// ?????

    if (policy_context != NULL){

      policy_context->setPolicies(policies, set_or_add);
      // _set_policy_overrides must leave current object reference with the same 
      // policies and return a new object reference with the new policies

      return obj_copy;
    }
  }
  else
    throw CORBA::INTERNAL("ObjectDelegate not found");
}



CORBA::PolicyList_ptr 
TIDorb::core::ObjectImpl::_get_policy_overrides(const CORBA::PolicyTypeSeq& types)
{
  ObjectDelegateImpl* delegate = (ObjectDelegateImpl*) (m_delegate);

  if(delegate) {
    TIDorb::core::PolicyContext* policy_context = NULL;
    policy_context = delegate->getPolicyContext(); 

    if (policy_context != NULL){
      return policy_context->getPolicies(types);
    }
  }
  else
    throw CORBA::INTERNAL("ObjectDelegate not found");
}


