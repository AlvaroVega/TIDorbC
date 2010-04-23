/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectDelegateImpl.C
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

TIDorb::core::ObjectDelegateImpl::ObjectDelegateImpl
  (TIDorb::core::TIDORB* orb,
   TIDorb::core::iop::IOR* ref,
   TIDorb::core::comm::CommunicationLayer* layer)
 throw (TIDThr::SystemException)
    : m_orb(orb), m_reference(ref), m_comm_layer(layer), policyContext(NULL), 
      m_domain_manager_list(NULL)
{
}  


TIDorb::core::ObjectDelegateImpl::ObjectDelegateImpl
  (TIDorb::core::TIDORB* orb,
   TIDorb::core::iop::IOR* ref,
   TIDorb::core::comm::CommunicationLayer* layer,
   TIDorb::core::PolicyContext* policies)
 throw (TIDThr::SystemException)
    : m_orb(orb), m_reference(ref), m_comm_layer(layer), policyContext(policies),
      m_domain_manager_list(NULL)
{
}  


TIDorb::core::ObjectDelegateImpl::~ObjectDelegateImpl() throw (TIDThr::SystemException)
{    
  if (policyContext != NULL)
    delete policyContext;
}

    
TIDorb::portable::ORB* TIDorb::core::ObjectDelegateImpl::orb() 
{
  return m_orb;
}

TIDorb::core::iop::IOR* TIDorb::core::ObjectDelegateImpl::getReference()
{
  return m_reference;
}

::CORBA::Boolean TIDorb::core::ObjectDelegateImpl::is_local() 
{
  if(!m_orb)
    throw CORBA::BAD_OPERATION("ORB Singleton", 0, CORBA::COMPLETED_NO);

  if(m_forwarded_delegate.in())  
    return m_forwarded_delegate->is_local();
  else
    return m_comm_layer->is_local_layer();
}

::CORBA::Boolean TIDorb::core::ObjectDelegateImpl::non_existent()
{
  if(!m_orb)
    throw CORBA::BAD_OPERATION("ORB Singleton", 0, CORBA::COMPLETED_NO);

  if(m_forwarded_delegate.in())  
    return m_forwarded_delegate->non_existent();
  else {
    TIDorb::core::PolicyContext* request_policy_context = createRequestPolicyContext();
    CORBA::Boolean non_existent = 
      ! m_comm_layer->object_exists(m_reference, *request_policy_context);
    delete request_policy_context;
  }
}

 //TIDORB 

void TIDorb::core::ObjectDelegateImpl::oneway_request(TIDorb::core::RequestImpl* request) throw(CORBA::SystemException)
{
  if (! m_forwarded_delegate.in())  
    m_comm_layer->oneway_request(request,m_reference);
  else
    m_forwarded_delegate->oneway_request(request);


}


void TIDorb::core::ObjectDelegateImpl::invoke(TIDorb::core::RequestImpl* request)
{
  try {
    if (! m_forwarded_delegate.in())  {
       m_comm_layer->request(request,m_reference);
    } else {// there is forwards      
        try {
          m_forwarded_delegate->invoke(request); 
        } catch (const CORBA::COMM_FAILURE& cf) {          
          m_forwarded_delegate = 0;
          if(cf.completed() == CORBA::COMPLETED_NO) {
            m_comm_layer->request(request, m_reference);
          } else {
            throw cf;
          }
        } catch (const CORBA::OBJECT_NOT_EXIST& one) {
          m_forwarded_delegate = 0;
          m_comm_layer->request(request, m_reference);
        } 
      }
    
  } catch (const ForwardRequest& fe) {
    
    m_forwarded_delegate = 
      m_orb->getCommunicationManager()->createDelegate(fe.m_reference);
    
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "DelegateImpl::invoke(): " << m_reference->toString()
          << " forwarded to " << fe.m_reference->toString() << flush; 
      m_orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
    invoke(request);
  }

}

TIDorb::core::PolicyContext* TIDorb::core::ObjectDelegateImpl::getPolicyContext()
{
  if ( policyContext == NULL ){
    if (m_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "DelegateImpl::getPolicyContext(): new PolicyContext created" << flush; 
      m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }
    policyContext = new TIDorb::core::PolicyContext();
  }
  return policyContext;
}


/**
 * Creates the PolicyContext for a request using reference + thread + ORB
 * PolicyManagers
 */
TIDorb::core::PolicyContext* TIDorb::core::ObjectDelegateImpl::createRequestPolicyContext()
{

  // generates effective Request context
  TIDorb::core::PolicyContext* context = this->getPolicyContext();
  TIDorb::core::PolicyContext* context_aux = context->duplicate();
  TIDThr::Thread* currentThread = TIDThr::Thread::currentThread();

  // thread_context has as father ORB Context
  TIDorb::core::PolicyContext* thread_context = 
    m_orb->getPolicyContextManager()->getThreadContext(currentThread);
  
  context_aux->setFatherContext(thread_context);
  return context_aux;

}


/**
 * Creates the PolicyContext for a request using reference + thread + ORB
 * PolicyManagers
 */
TIDorb::core::ObjectDelegateImpl* TIDorb::core::ObjectDelegateImpl::copy()
{

  // generates effective Request context
  TIDorb::core::PolicyContext* context = this->getPolicyContext();
  TIDorb::core::PolicyContext* context_aux = context->duplicate();


  TIDThr::Thread* currentThread = TIDThr::Thread::currentThread();

  // thread_context has as father ORB Context
  TIDorb::core::PolicyContext* thread_context = 
    m_orb->getPolicyContextManager()->getThreadContext(currentThread);
  
  context_aux->setFatherContext(thread_context);

  ObjectDelegateImpl* copy_delegate = 
    new ObjectDelegateImpl(m_orb, m_reference, m_comm_layer, context_aux);

  return copy_delegate;

}



CORBA::DomainManagerList* 
TIDorb::core::ObjectDelegateImpl::getDomainManagerList()
{

  if (m_domain_manager_list == NULL) {
    m_domain_manager_list = new CORBA::DomainManagerList();
    m_domain_manager_list->length(1);
    (*m_domain_manager_list)[0] = m_orb->getDefaultDomainManager();
  }
  return m_domain_manager_list;

}
