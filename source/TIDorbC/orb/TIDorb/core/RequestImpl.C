/////////////////////////////////////////////////////////////////////////
//
// File:        RequestImpl.C
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


TIDorb::core::RequestImpl::RequestImpl(TIDorb::portable::Stub* target,
                                       ::CORBA::Context_ptr ctx,
                                       const char* operation,
                                       ::CORBA::NVList_ptr arg_list,
                                       ::CORBA::NamedValue_ptr result,
                                       ::CORBA::ExceptionList_ptr ex_list,
                                       ::CORBA::ContextList_ptr ctx_list,
                                       ::CORBA::Flags req_flags)
  throw (TIDThr::SystemException): m_result("",0,0) //jagd 4
{
//MLG
//TODO cambiar que herede de monitor a que lo tenga como miembro
//y hacer que herede de la TIDORB::portable::RefCounter
  _count(1);
//EMLG
  //jagd quita dynamic_cast
  //m_target = dynamic_cast<TIDorb::portable::Stub*> (CORBA::Object::_duplicate(target));
  m_target=target;
  target->_add_ref();
//MLG it owns the list
//TODO is it necessary the dynamic_cast?
  //m_orb = dynamic_cast<TIDorb::core::TIDORB*> (CORBA::ORB::_duplicate(m_target->_orb()));
  //jagd 
  //m_orb = dynamic_cast<TIDorb::core::TIDORB*> (m_target->_orb());
  m_orb = (TIDorb::core::TIDORB*) (m_target->_orb());
  //m_context = dynamic_cast<ContextImpl*>(CORBA::Context::_duplicate(ctx));
  //jagd 
  //m_context = dynamic_cast<ContextImpl*>(ctx);
  m_context = (ContextImpl*)(ctx);
  m_operation_name = operation;
  //m_arguments = dynamic_cast<NVListImpl*>(CORBA::NVList::_duplicate(arg_list));
  //m_arguments = dynamic_cast<NVListImpl*>(arg_list);
  m_arguments = (NVListImpl*)arg_list->_impl();
  //m_result = dynamic_cast<NamedValueImpl*>(CORBA::NamedValue::_duplicate(result));
  if (result)
  {
  //jagd
  //	m_result = (NamedValueImpl*)CORBA::NamedValue::_duplicate(result)->_impl();
  	//jagd 3m_result = (NamedValueImpl*)CORBA::NamedValue::_duplicate((NamedValueImpl*)result);
  	//jagd 4 m_result = (NamedValueImpl*)result;
  	m_result = *(NamedValueImpl*)result;
        ((NamedValueImpl*)result)->m_release_out=false; //nos hemos copiado el any y no lo queremos borrar
  }
  //jagd 4 else
  //    m_result =NULL;


  //m_exceptions = dynamic_cast<ExceptionListImpl*>(CORBA::ExceptionList::_duplicate(ex_list));
  //jagd cambio por cast normal 
  //m_exceptions = dynamic_cast<ExceptionListImpl*>(ex_list);
  m_exceptions = (ExceptionListImpl*)(ex_list);
  //m_context_list = dynamic_cast<ContextListImpl*>(CORBA::ContextList::_duplicate(ctx_list));
  //jagd
  //m_context_list = dynamic_cast<ContextListImpl*>(ctx_list);
  m_context_list = (ContextListImpl*)(ctx_list);
 //EMLG 
  m_release_out_params = req_flags & CORBA::OUT_LIST_MEMORY;
  
  m_completed = CORBA::COMPLETED_NO;
  //jagd 3 m_env = new EnvironmentImpl();
  m_with_response = false;
  m_reliable_oneway = false; 
  m_request_id = 0;
 
  /* jagd 4 codigo que no se usa  
  if(m_arguments && (!m_release_out_params)) {
    CORBA::ULong length = m_arguments->count();
    for(CORBA::ULong i = 0; i < length; i++) {
      //(dynamic_cast<NamedValueImpl*>(m_arguments->item(i)))->release_out(false);
     //jagd
     // ((NamedValueImpl*)m_arguments->item(i)->_impl())->release_out(false);
     ((NamedValueImpl*)m_arguments->item(i))->release_out(false);
    }
  }
  */ 

  m_policy_context = NULL;
}

                
TIDorb::core::RequestImpl::~RequestImpl()  throw (TIDThr::SystemException)
{
  //jagd 2
  //CORBA::release(m_context);
  delete (m_context);
  //jagd
  //CORBA::string_free(m_operation_name);
  CORBA::release(m_arguments);
  //jagd 3
  //CORBA::release(m_result);
  //jagd 4 delete m_result;
  //jagd
  //CORBA::release(m_exceptions);
  delete m_exceptions;
  CORBA::release(m_context_list);
  //jagd 3 CORBA::release(m_env);
//MLG  
  //CORBA::release(m_orb);
//EMLG  
//FRAN
  CORBA::release(m_target);
//EFRAN
  
  if (m_policy_context != NULL)
    delete m_policy_context;

}
    
::CORBA::Object_ptr TIDorb::core::RequestImpl::target() const
{
  return CORBA::Object::_duplicate(m_target);
}

const char* TIDorb::core::RequestImpl::operation() const
{
  return m_operation_name;
}

::CORBA::NVList_ptr TIDorb::core::RequestImpl::arguments()
{
  return CORBA::NVList::_duplicate(m_arguments);
}

::CORBA::NamedValue_ptr TIDorb::core::RequestImpl::result()
{
  //jagd 3 return CORBA::NamedValue::_duplicate(m_result);
  //jagd 4return m_result;
  return &m_result;
}

::CORBA::Environment_ptr TIDorb::core::RequestImpl::env()
{
//FRAN
  //return CORBA::Environment::_duplicate(m_env);
  return &m_env;
//EFRAN
}

::CORBA::ExceptionList_ptr TIDorb::core::RequestImpl::exceptions()
{
//FRAN
//  return CORBA::ExceptionList::_duplicate(m_exceptions);
  return m_exceptions;
//EFRAN
}

::CORBA::ContextList_ptr TIDorb::core::RequestImpl::contexts()
{
  return CORBA::ContextList::_duplicate(m_context_list);  
}

void TIDorb::core::RequestImpl::ctx(::CORBA::Context_ptr context)
{
  // jagd 2
  //if(!CORBA::is_nil(m_context))
  //if((m_context))
  // jagd 2
    //CORBA::release(m_context);
    delete m_context;
  //jagd
  //m_context = dynamic_cast<ContextImpl*> (CORBA::Context::_duplicate(context));
  //jagd 2 
  //m_context = (ContextImpl*) (CORBA::Context::_duplicate(context));
  m_context = (ContextImpl*) ((context));
}

::CORBA::Context_ptr TIDorb::core::RequestImpl::ctx() const
{
  //jagd 2
  //return CORBA::Context::_duplicate(m_context);  
  return (m_context);  
}

::CORBA::Any& TIDorb::core::RequestImpl::add_in_arg()
{
  return add_arg("", CORBA::ARG_IN);
}

::CORBA::Any& TIDorb::core::RequestImpl::add_in_arg(const char* name)
{
  return add_arg(name, CORBA::ARG_IN);
}

::CORBA::Any& TIDorb::core::RequestImpl::add_inout_arg()
{
  return add_arg("", CORBA::ARG_INOUT);
}

::CORBA::Any& TIDorb::core::RequestImpl::add_inout_arg(const char* name)
{
  return add_arg(name, CORBA::ARG_INOUT);
}

::CORBA::Any& TIDorb::core::RequestImpl::add_out_arg()
{
  return add_arg("", CORBA::ARG_OUT);
}

::CORBA::Any& TIDorb::core::RequestImpl::add_out_arg(const char* name)
{
  return add_arg(name, CORBA::ARG_OUT);
}

::CORBA::Any& TIDorb::core::RequestImpl::add_arg(const char* name, CORBA::Flags flags)
{
  if(!m_arguments)
    //jagd
    //m_arguments = new NVListImpl(dynamic_cast< TIDORB* > (m_target->_orb()));
    m_arguments = new NVListImpl(m_orb);
  
  //jagd 3 CORBA::NamedValue_var nv = m_arguments->add_item(name, flags);
  CORBA::NamedValue * nv = m_arguments->add_item(name, flags);
 
  /* jagd 4 codigo que no se usa 
  if((flags == CORBA::ARG_OUT) && (!m_release_out_params))
    //(dynamic_cast<NamedValueImpl*>((CORBA::NamedValue_ptr) nv))->release_out(false);
    //jagd ((NamedValueImpl*)((CORBA::NamedValue_ptr) nv)->_impl())->release_out(false);
    ((NamedValueImpl*)nv)->release_out(false);
  */ 
  return *(nv->value());

}

void TIDorb::core::RequestImpl::set_return_type(::CORBA::TypeCode_ptr tc)
{
  //jagd 4 if(m_result)
  //jagd 3  CORBA::release(m_result);
  //  delete (m_result);
  
  CORBA::Any* any = m_target->_orb()->create_any();
  // Fix bug [#392] Any::type(tc) reset any value
  //any->type(tc);
  any->delegate().set_type(tc);
 //MLG _duplicate needed because NamedValueImpl should inherit from TIDorb::portable::RefCounter 
  //m_result =  dynamic_cast<NamedValueImpl*>(CORBA::NamedValue::_duplicate(new NamedValueImpl(CORBA::string_dup(""), any, CORBA::ARG_OUT)));
  //jagd 
  //m_result =  (NamedValueImpl*)CORBA::NamedValue::_duplicate(new NamedValueImpl(CORBA::string_dup(""), any, CORBA::ARG_OUT))->_impl();
  //jagd 3 m_result =  new NamedValueImpl(CORBA::string_dup(""), any, CORBA::ARG_OUT);
  // jagd 4 m_result =  new NamedValueImpl("", any, CORBA::ARG_OUT);
  m_result.m_value= any;
  m_result.m_flags= CORBA::ARG_OUT;
  //jagd 3 
  //m_result->_add_ref(); 
//EMLG
  
}

::CORBA::Any& TIDorb::core::RequestImpl::return_value()
{
  //jagd 4if(m_result)
  //  return *(m_result->value());
  if(m_result.m_value)
    return *(m_result.m_value);
  
  throw CORBA::BAD_OPERATION();
  
}

void TIDorb::core::RequestImpl::invoke()
{
  try {
    m_with_response = true;
  
    //jagd	
    //ObjectDelegateImpl* delegate = dynamic_cast <ObjectDelegateImpl* > (m_target->_get_delegate());
    ObjectDelegateImpl* delegate = (ObjectDelegateImpl*) (m_target->_get_delegate());
      
    delegate->invoke(this); 
    
  } catch (const CORBA::SystemException& se) {
    set_system_exception(::TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(se));
  } catch (const CORBA::Exception& ex) {
    set_system_exception(new CORBA::UNKNOWN());

    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "RequestImpl::invoke(): Unexpected exception: " << ex._name(); 
      m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  }

  //jagd 3 CORBA::Exception* e = m_env->exception();
  CORBA::Exception* e = m_env.exception();
  
  //jagd 
  //CORBA::SystemException* se = CORBA::SystemException::_downcast(e);
  //TODO jagd CORBA::SystemException* se = (CORBA::SystemException*)(e);
  
  if (e)
  {
     CORBA::SystemException* se =dynamic_cast< CORBA::SystemException* >(e); 
     if(se)
       se->_raise();
  }
}


void TIDorb::core::RequestImpl::send_oneway()
{
//   throw CORBA::NO_IMPLEMENT(); 
//   // it is not the same as invoke the request with with_response setted to false

  try {
  
    m_with_response = false;
  
    //jagd	
    //ObjectDelegateImpl* delegate = dynamic_cast <ObjectDelegateImpl* > (m_target->_get_delegate());
    ObjectDelegateImpl* delegate = (ObjectDelegateImpl*)  (m_target->_get_delegate());
      
    delegate->oneway_request(this);
  		
  } catch (CORBA::SystemException& se) {
    se.completed(m_completed);
    se._raise();
  }  	

}

void TIDorb::core::RequestImpl::send_deferred()
{
  throw CORBA::NO_IMPLEMENT();
}

void TIDorb::core::RequestImpl::get_response()
{
  throw CORBA::NO_IMPLEMENT();
}

::CORBA::Boolean TIDorb::core::RequestImpl::poll_response()
{
  throw CORBA::NO_IMPLEMENT();
}


void TIDorb::core::RequestImpl::set_completed(CORBA::CompletionStatus status)
{
  m_completed = status;
}

CORBA::CompletionStatus TIDorb::core::RequestImpl::get_completed() const
{
  return m_completed;
}   

void TIDorb::core::RequestImpl::set_user_exception(const CORBA::Any& exc) 
{
  //jagd 3 m_env->exception(new CORBA::UnknownUserException(exc));
  m_env.exception(new CORBA::UnknownUserException(exc));
}

void TIDorb::core::RequestImpl::set_system_exception(CORBA::SystemException* exc)
{
  //jagd 3 m_env->exception(exc);
  m_env.exception(exc);
}

void TIDorb::core::RequestImpl::read_result(TIDorb::portable::InputStream& input) 
{
  //jagd 4 if(m_result) {  
  //  CORBA::TypeCode_var tc = m_result->value()->type(); 		
  //  m_result->value()->delegate().read_value(input,tc);
  //}
  if(m_result.m_value) {  
    CORBA::TypeCode_var tc = m_result.m_value->type(); 		
    m_result.m_value->delegate().read_value(input,tc);
  }
}


TIDorb::core::LocalServerRequest* TIDorb::core::RequestImpl::server_request()
{
  return new TIDorb::core::LocalServerRequest(this);
}

TIDorb::core::TIDORB* TIDorb::core::RequestImpl::orb() const
{
  return m_orb;
}

TIDorb::core::comm::iiop::RequestId TIDorb::core::RequestImpl::get_id() const
{
  return m_request_id;
}

void TIDorb::core::RequestImpl::set_id(TIDorb::core::comm::iiop::RequestId id)
{
  m_request_id = id;
}

void TIDorb::core::RequestImpl::reliable_oneway(bool value)
{
  m_reliable_oneway = value;
}

bool TIDorb::core::RequestImpl::reliable_oneway() const
{
  return m_reliable_oneway;
}  

void TIDorb::core::RequestImpl::with_response(bool value)
{
  m_with_response = value;
}

bool TIDorb::core::RequestImpl::with_response() const
{
  return m_with_response;
} 

void TIDorb::core::RequestImpl::write_params_in(TIDorb::core::cdr::CDROutputStream& output) const
{
  if(m_arguments)
    NVListImpl::write_params_in(m_arguments, output);
}

void* TIDorb::core::RequestImpl::_impl()
{
  return this;
}

const char* TIDorb::core::RequestImpl::_typeid()
{
  //return CORBA::string_dup("RequestImpl");
  return "RequestImpl";
}



TIDorb::core::PolicyContext* TIDorb::core::RequestImpl::getPolicyContext()
{
  if (m_policy_context == NULL) {
    
    ObjectDelegateImpl* delegate = (ObjectDelegateImpl*) (m_target->_get_delegate());
//     ObjectDelegateImpl* delegate = 
//       dynamic_cast <ObjectDelegateImpl* > (m_target->_get_delegate());
    
    m_policy_context = delegate->createRequestPolicyContext();
  }

 return m_policy_context;
}



CORBA::Request_ptr CORBA::Request::_duplicate(CORBA::Request_ptr req)
{
  try {
    
    //TIDorb::core::RequestImpl* req_impl = dynamic_cast<TIDorb::core::RequestImpl*> (req);
    TIDorb::core::RequestImpl* req_impl = NULL;
      //jagd
    	req_impl = (TIDorb::core::RequestImpl*)req;
    
    if(req_impl)
      req_impl->_add_ref();
    
    return req;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
}

CORBA::Request_ptr CORBA::Request::_nil()
{
  return 0;
}

void CORBA::release(CORBA::Request_ptr req)
{
  try {
    
    //TIDorb::core::RequestImpl* req_impl= dynamic_cast<TIDorb::core::RequestImpl*> (req);
    TIDorb::core::RequestImpl* req_impl = NULL;
    //jagd
    	req_impl = (TIDorb::core::RequestImpl*)req;
    
    if(req_impl)
      req_impl->_remove_ref();
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
}

CORBA::Boolean CORBA::is_nil(CORBA::Request_ptr req)
{
  return !req;
}

