/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ObjectDelegateImpl.h
// Description: Object Delegate Implementation.
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core.h"

#ifndef _TIDorb_core_RequestImpl_H_
#define _TIDorb_core_RequestImpl_H_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {
  
//typedef CORBA::ULong RequestId;  

class RequestImpl: public CORBA::Request,
                   public TIDThr::RefCounter
  {
  public:
    RequestImpl(){}; 
 
    RequestImpl(TIDorb::portable::Stub* target,
                ::CORBA::Context_ptr ctx,
                const char *operation,
                ::CORBA::NVList_ptr arg_list,
                ::CORBA::NamedValue_ptr result,
                ::CORBA::ExceptionList_ptr ex_list,
                ::CORBA::ContextList_ptr ctx_list,
                ::CORBA::Flags req_flags) 
      throw (TIDThr::SystemException);
                
    ~RequestImpl()  throw (TIDThr::SystemException);
    
    ::CORBA::Object_ptr target() const;
    const char *operation() const;
    ::CORBA::NVList_ptr arguments();
    ::CORBA::NamedValue_ptr result();
    ::CORBA::Environment_ptr env();
    ::CORBA::ExceptionList_ptr exceptions();
    ::CORBA::ContextList_ptr contexts();
    void ctx(::CORBA::Context_ptr);
    ::CORBA::Context_ptr ctx() const;
    ::CORBA::Any& add_in_arg();
    ::CORBA::Any& add_in_arg(const char* name);
    ::CORBA::Any& add_inout_arg();
    ::CORBA::Any& add_inout_arg(const char* name);
    ::CORBA::Any& add_out_arg();
    ::CORBA::Any& add_out_arg(const char* name);
    void set_return_type(::CORBA::TypeCode_ptr tc);
    ::CORBA::Any& return_value();
    void invoke();
    void send_oneway();
    void send_deferred();
    void get_response();
    ::CORBA::Boolean poll_response();
    
    void set_completed(CORBA::CompletionStatus status);    
    CORBA::CompletionStatus get_completed() const;  
    
    void set_user_exception(const CORBA::Any& exc);   
    void set_system_exception(CORBA::SystemException* exc);
    
    void read_result(TIDorb::portable::InputStream& input); 

    LocalServerRequest* server_request();
  
    TIDorb::core::comm::iiop::RequestId get_id() const;
    void set_id(TIDorb::core::comm::iiop::RequestId id);
    void reliable_oneway(bool value);
    bool reliable_oneway() const;
    void with_response(bool value);
    bool with_response() const;
    
    TIDORB* orb() const;

    void write_params_in(TIDorb::core::cdr::CDROutputStream& output) const;
    
    TIDorb::core::PolicyContext* getPolicyContext();

  protected:
  
    ::CORBA::Any& add_arg(const char* name, CORBA::Flags flags);
    
    	/**
  	 * operation name.
  	 */
        //jagd
  	//char* m_operation_name;
  	const char* m_operation_name;
  	 
    /**
     * Request identifier.
     */
    TIDorb::core::comm::iiop::RequestId m_request_id;
  	
    /**
     * Needs response.
     */
    bool m_with_response;
  	
    /**
     * Reliable oneway request that needs response.
     */
    bool m_reliable_oneway;
  	
    /**
     * FREE or not the Any values of Out params = request_flags & CORBA::OUT_LIST_MEMORY
     */
    bool m_release_out_params;
  	
    /**
     * Completion status needed in exception throwing. Initialy, the status is <code>COMPLETED_NO</code>
     */
    CORBA::CompletionStatus m_completed;
  
    TIDorb::portable::Stub* m_target;
    NVListImpl*        m_arguments;
    //jagd 4 NamedValueImpl*    m_result;
    NamedValueImpl     m_result;
    //jagd 3 EnvironmentImpl*   m_env;
    EnvironmentImpl    m_env;
    ExceptionListImpl* m_exceptions;
    ContextListImpl*   m_context_list;
    ContextImpl*       m_context;
    TIDORB*            m_orb;
 
    TIDorb::core::PolicyContext*  m_policy_context;


  public:
    virtual void* _impl();
    virtual const char* _typeid();

};

}
}

#endif
