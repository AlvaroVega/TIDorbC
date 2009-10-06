/////////////////////////////////////////////////////////////////////////
//
// File:        Stub.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

using TIDorb::core::ObjectDelegateImpl;

TIDorb::portable::Stub::Stub()
  : m_delegate(0)
{
//MLG hereda de TIDorb::portable::RefCounter y empieza en 1
  //_count(1);
//EMLG
}


TIDorb::portable::Stub::~Stub()
{
  try {
    if(m_delegate)
    m_delegate->_remove_ref();

  } catch (...) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}


::CORBA::InterfaceDef_ptr TIDorb::portable::Stub::_get_interface()
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return NULL;
}


::CORBA::Boolean TIDorb::portable::Stub::_is_a(const char* logical_type_id)
{
  if(!logical_type_id)
    throw  CORBA::BAD_PARAM("Null string reference", 0, CORBA::COMPLETED_NO);

  if(strcmp(logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return true;

  ObjectDelegateImpl* delegate = dynamic_cast <ObjectDelegateImpl*> (m_delegate);

  if(strcmp(logical_type_id, delegate->getReference()->get_type_id()) == 0)
    return true;

  // search in known interface

//MLG
  CORBA::RepositoryIdSeq* interf_ids = _ids();
//EMLG
  if (interf_ids != NULL) {
    CORBA::ULong size = interf_ids->length();

    for(CORBA::ULong i = 0; i < size; i++) {
      if(strcmp(logical_type_id, (*interf_ids)[i]) == 0) {
        return true;
      }
    }
  }

  // Create the request:
  CORBA::Request_var request = _request("_is_a");

  CORBA::Any& pin_any = request->add_in_arg();
  pin_any <<= logical_type_id;

//FRAN
  request->set_return_type(CORBA::_tc_boolean);
//EFRAN
  // Perform the invocation:

  request->invoke();

  CORBA::Exception* _exception = request->env()->exception();

  if (_exception != NULL)
  {
    CORBA::SystemException_ptr systemException = CORBA::SystemException::_downcast(_exception);

    if (systemException != NULL)
    {
      systemException->_raise();
    }

    throw ::CORBA::INTERNAL();
  }

  // Extrae el resultado y los argumentos de salida:

  CORBA::Boolean result;
  request->return_value() >>= CORBA::Any::to_boolean(result);
  return result;
}


::CORBA::Boolean TIDorb::portable::Stub::_non_existent()
{
  return m_delegate->non_existent();
}


::CORBA::Boolean TIDorb::portable::Stub::_is_equivalent(::CORBA::Object_ptr other_object)
{
  if(((CORBA::Object_ptr) this) == other_object)
    return true;

  ObjectDelegateImpl* delegate = dynamic_cast <ObjectDelegateImpl*> (m_delegate);

  Stub* other_stub = dynamic_cast <Stub*> (other_object);

  if(other_stub == NULL)
    return false;

  ObjectDelegateImpl* other_delegate =
    dynamic_cast < ObjectDelegateImpl* > (other_stub->_get_delegate());

  return delegate->getReference()->equivalent(*(other_delegate->getReference()));
}


::CORBA::ULong TIDorb::portable::Stub::_hash(::CORBA::ULong maximum)
{
  ObjectDelegateImpl* delegate = dynamic_cast <ObjectDelegateImpl*> (m_delegate);

  return delegate->getReference()->hashCode();
}


void TIDorb::portable::Stub::_create_request(::CORBA::Context_ptr ctx,
                                             const char *operation,
                                             ::CORBA::NVList_ptr arg_list,
                                             ::CORBA::NamedValue_ptr result,
                                             ::CORBA::Request_out request,
                                             ::CORBA::Flags req_flags)
{
  TIDorb::portable::ORB* orb = m_delegate->orb();
  CORBA::ExceptionList_ptr ex_list = CORBA::ExceptionList::_nil();
  orb->create_exception_list(ex_list);
  ::CORBA::ContextList_ptr ctx_list = CORBA::ContextList::_nil();
  orb->create_context_list(ctx_list);

  request = new TIDorb::core::RequestImpl(this, ctx, operation, arg_list, result,
  ex_list, ctx_list, req_flags);
}


void TIDorb::portable::Stub::_create_request(::CORBA::Context_ptr ctx,
                                             const char *operation,
                                             ::CORBA::NVList_ptr arg_list,
                                             ::CORBA::NamedValue_ptr result,
                                             ::CORBA::ExceptionList_ptr ex_list,
                                             ::CORBA::ContextList_ptr ctx_list,
                                             ::CORBA::Request_out request,
                                             ::CORBA::Flags req_flags)
{
  request = new TIDorb::core::RequestImpl(this, ctx, operation, arg_list, result,
  ex_list, ctx_list, req_flags);
}


::CORBA::Request_ptr TIDorb::portable::Stub::_request(const char* operation)
{
  TIDorb::portable::ORB* orb = m_delegate->orb();
  CORBA::NVList_ptr arg_list = CORBA::NVList::_nil();
  orb->create_list(0, arg_list);
  CORBA::ExceptionList_ptr ex_list = CORBA::ExceptionList::_nil();
  orb->create_exception_list(ex_list);
  ::CORBA::ContextList_ptr ctx_list = CORBA::ContextList::_nil();
  orb->create_context_list(ctx_list);

  return new TIDorb::core::RequestImpl(this, CORBA::Context::_nil(), operation, arg_list,
  CORBA::NamedValue::_nil(), ex_list, ctx_list, 0);
}


::CORBA::Policy_ptr TIDorb::portable::Stub::_get_policy(::CORBA::PolicyType policy_type)
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return NULL;
}


::CORBA::DomainManagerList* TIDorb::portable::Stub::_get_domain_managers()
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return NULL;
}


::CORBA::Object_ptr TIDorb::portable::Stub::_set_policy_overrides(const CORBA::PolicyList& policies,
CORBA::SetOverrideType set_or_add)
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return NULL;
}


CORBA::Policy_ptr TIDorb::portable::Stub::_get_client_policy(CORBA::PolicyType policy_type)
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return NULL;
}


CORBA::PolicyList_ptr TIDorb::portable::Stub::_get_policy_overrides(const CORBA::PolicyTypeSeq& types)
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return NULL;
}


CORBA::Boolean TIDorb::portable::Stub::_validate_connection(CORBA::PolicyList_out inconsistent_policies)
{
  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
  // never reached !!
  return false;
}


TIDorb::portable::ORB* TIDorb::portable::Stub::_orb()
{
  return m_delegate->orb();
}


::CORBA::Boolean TIDorb::portable::Stub::_is_local()
{
  return m_delegate->is_local();
}


void TIDorb::portable::Stub::_set_delegate(TIDorb::portable::ObjectDelegate* d)
{
  if(d) {

    if(m_delegate)
    m_delegate->_remove_ref();

    m_delegate = d;

    try {
      m_delegate->_add_ref();
    } catch (...) {
      throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
    }
  }
}


TIDorb::portable::ObjectDelegate* TIDorb::portable::Stub::_get_delegate()
{
  return m_delegate;
}
