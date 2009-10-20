//////////////////////////////////////////////////////////////////////////////////
//
// File:        IIOPCommLayer.C
// Description: 
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"


#include <exception>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#undef ERROR



TIDorb::core::comm::LocalCommLayer::LocalCommLayer(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
  : CommunicationLayer(orb)
{
  m_max_response_blocked_time = conf.max_blocked_time;
  m_qos_enabled = conf.qos_enabled;
}



bool 
TIDorb::core::comm::LocalCommLayer::object_exists(TIDorb::core::iop::IOR* ior,
                                                  const TIDorb::core::PolicyContext& policy_context)
  throw(TIDorb::core::ForwardRequest, CORBA::SystemException)
{
  try {

    CORBA::ULongLong timeout = m_max_response_blocked_time;
    
    if (m_qos_enabled) {
      if (&policy_context != NULL) {
        timeout =  
          TIDorb::core::messaging::QoS::checkRequestTime(_orb, policy_context);
      }
    }

    TIDorb::core::poa::POAKey* poaKey = ior->object_key()->get_key();

    // Creating LocalLocateRequest
    TIDorb::core::poa::POAImpl* pPOAImpl = 
      dynamic_cast<TIDorb::core::poa::POAImpl *>(_orb->init_POA());
    TIDorb::core::poa::LocalLocateRequest_ref localLocateRequest =
      new TIDorb::core::poa::LocalLocateRequest(poaKey, pPOAImpl);

    localLocateRequest->setSerial(_orb->getRequestCounter()->next());

    // Check if the the thread that invoked the request is an ExecThread

    TIDThr::Thread* this_thread = TIDThr::Thread::currentThread();

    TIDorb::core::poa::POAManagerImpl* poaManager =
      (TIDorb::core::poa::POAManagerImpl*)(pPOAImpl->the_POAManager());
    

    TIDorb::core::poa::ExecThread* exec_thread = 
      (TIDorb::core::poa::ExecThread*)(this_thread);

    if(exec_thread) {
      ((TIDorb::core::poa::ExecThread*)this_thread)->processLocalRequest(
                                                                localLocateRequest,
                                                                poaManager);
    } else { // application invocation
      // Invoking poaManager

      localLocateRequest->_add_ref(); /* new reference in POAManager request queue */

      poaManager->put(localLocateRequest);
      localLocateRequest->wait_for_completion(timeout);
    }

    // Invoking poaManager

    if(!localLocateRequest->is_completed())
      throw CORBA::NO_RESPONSE();

    if (localLocateRequest->is_forwarded())
      {
        TIDorb::core::iop::IOR* forwarded_ior;
        CORBA::Object* forward = localLocateRequest->get_forward();
        if(!(dynamic_cast<CORBA::ValueBase*>(forward)))
          throw CORBA::NO_IMPLEMENT();
 
        TIDorb::core::ObjectImpl* pAux = 
          dynamic_cast<TIDorb::core::ObjectImpl*>(forward); 

        if(pAux)
          throw CORBA::INV_OBJREF();

        TIDorb::portable::ObjectDelegate* forward_delegate =
          pAux->_get_delegate();


        TIDorb::core::ObjectDelegateImpl* pForward = 
          (TIDorb::core::ObjectDelegateImpl*)(forward_delegate); 

        if(!(pForward)) {
          forwarded_ior = pForward->getReference();
        }  else { // write ior from anothers's ORB Object
          forwarded_ior = new TIDorb::core::iop::IOR();
          forwarded_ior->fromString(_orb, _orb->object_to_string(forward));
        }

        throw TIDorb::core::ForwardRequest(forwarded_ior);
      }
    
    if (localLocateRequest->has_exception()) 
      localLocateRequest->exception()->_raise();

    // Return results

    return localLocateRequest->found;

  } catch (const CORBA::INTERNAL& e) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, 
                        "INTERNAL Exception in local location invocation", e);
    }
    throw;
  } catch (const CORBA::OBJECT_NOT_EXIST& e) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_DEBUG, 
                        "OBCJECT_NOT_EXIST exception in local location invocation", e);
    }
    return false;
  } catch (const CORBA::SystemException& se) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in local location invocation", se);
    }
    throw;
  } catch (const exception& e) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in local location invocation", e);
    }

    throw CORBA::INTERNAL(e.what());
  }
}


void TIDorb::core::comm::LocalCommLayer::request(TIDorb::core::RequestImpl* request,
                                                 TIDorb::core::iop::IOR* ior)
  throw(TIDorb::core::ForwardRequest,CORBA::SystemException)
{
  try {

    request->with_response(true);
    send_request(request, ior); // throws ForwardRequest

  } catch (const PortableServer::ForwardRequest) {
    throw;
  } catch (const CORBA::SystemException& se) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in local invocation", se);
    }
    throw;
  } catch (const TIDThr::SystemException& threx) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected TIDThr exception: " << threx.what();
      msg << " at LocalCommLayer::request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (const exception& e) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in local invocation", e);
    }
    throw CORBA::INTERNAL(e.what());
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception raised.";
      msg << " at LocalCommLayer::request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }
}


void TIDorb::core::comm::LocalCommLayer::oneway_request(TIDorb::core::RequestImpl* request,
                                                        TIDorb::core::iop::IOR* ior)
{
  try {

    request->with_response(false);
    send_request(request, ior);

  } catch (const PortableServer::ForwardRequest& se)  {
    // unreachable
  } catch (const CORBA::SystemException& se) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in local oneway invocaton", se);
    }
    throw;
  } catch (const TIDThr::SystemException& threx) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected TIDThr exception: " << threx.what();
      msg << " at LocalCommLayer::oneway_request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (const exception& e) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in local oneway invocaton", e);
    }
    throw CORBA::INTERNAL(e.what());
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception raised.";
      msg << " at LocalCommLayer::oneway_request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }
}


void TIDorb::core::comm::LocalCommLayer::send_request(TIDorb::core::RequestImpl* request,
                                                      TIDorb::core::iop::IOR* ior)
  throw(TIDorb::core::ForwardRequest, CORBA::SystemException)
{

  CORBA::ULongLong timeout = m_max_response_blocked_time;
    
  if (m_qos_enabled) {
    
    TIDorb::core::PolicyContext* policy_context = request->getPolicyContext();
    
    if (policy_context != NULL) {
      timeout =  
        TIDorb::core::messaging::QoS::checkRequestTime(_orb, policy_context);
    }
  }
  
  
  TIDorb::core::poa::POAKey* poaKey = ior->object_key()->get_key();

  // Creating ServerRequest
  TIDorb::core::LocalServerRequest* serverRequest = 
    new TIDorb::core::LocalServerRequest(request);

  // Creating LocalRequest
  TIDorb::core::poa::POAImpl * pPOAImpl =  
    dynamic_cast<TIDorb::core::poa::POAImpl *>(_orb->init_POA());
  TIDorb::core::poa::LocalRequest_ref localRequest = 
    new TIDorb::core::poa::LocalRequest(poaKey, pPOAImpl, serverRequest);

  localRequest->setSerial(_orb->getRequestCounter()->next());

  if(!request->with_response()) {
    // Invoking poaManager
    TIDorb::core::poa::POAManagerImpl* poaManager = 
      (TIDorb::core::poa::POAManagerImpl*)(pPOAImpl->the_POAManager());
    localRequest->_add_ref(); /* new reference in POAManager request queue */
    poaManager->put(localRequest);
  } else {
    // Check if the the thread that invoked the request is an ExecThread

    TIDThr::Thread* this_thread = TIDThr::Thread::currentThread();
 
    TIDorb::core::poa::POAManagerImpl* poaManager = 
      (TIDorb::core::poa::POAManagerImpl*)(pPOAImpl->the_POAManager());

    TIDorb::core::poa::ExecThread* exec_thread = (TIDorb::core::poa::ExecThread*)(this_thread);


    // Check if exec_thread belongs to POAManager thread pool of request
    bool thread_in_manager = false;
    if (exec_thread) {
      TIDorb::core::poa::POAImpl* current_poa = pPOAImpl; // localRequest->get_current_POA();
      if (localRequest->isFinalPOA()){

        PortableServer::POAManager_ptr aux_nextPOAManager = 
          current_poa->the_POAManager();
        
        TIDorb::core::poa::POAManagerImpl* nextPOAManager = 
          (TIDorb::core::poa::POAManagerImpl*)(aux_nextPOAManager);
        
        if (exec_thread->belongsToPOAManager(nextPOAManager)) {
          thread_in_manager = true;
        }

      } else {

        while (!localRequest->isFinalPOA()) {
          current_poa = 
            current_poa->find_POA((const char*)localRequest->get_current_child_POA_name(), 
                                   true);
          
          localRequest->next_child_POA(current_poa);
          PortableServer::POAManager_ptr aux_nextPOAManager = 
            current_poa->the_POAManager();
          
          TIDorb::core::poa::POAManagerImpl* nextPOAManager = 
            (TIDorb::core::poa::POAManagerImpl*)(aux_nextPOAManager);
          
          if (exec_thread->belongsToPOAManager(nextPOAManager)) {
            thread_in_manager = true;
            break;
          }
          poaManager = nextPOAManager;
        } // while
      }
    }

    if(exec_thread && thread_in_manager) {
      ((TIDorb::core::poa::ExecThread*)this_thread)->processLocalRequest(
                                                                 localRequest, 
                                                                 poaManager);
    } else { // application invocation
      // Invoking poaManager

      localRequest->_add_ref(); /* new reference in POAManager request queue */

      poaManager->put(localRequest);
      localRequest->wait_for_completion(timeout);
    }

    if(!localRequest->is_completed()) {
      throw CORBA::NO_RESPONSE();
    }

    if(serverRequest->is_forwarded()) {

      TIDorb::core::iop::IOR* forwarded_ior;

      CORBA::Object_var forward = serverRequest->get_forward();

      if(dynamic_cast<CORBA::ValueBase*>((CORBA::Object_ptr) forward))
        throw CORBA::NO_IMPLEMENT();


      TIDorb::portable::Stub* forward_stub =
        dynamic_cast<TIDorb::portable::Stub*>((CORBA::Object_ptr) forward);

      if(!forward_stub)
        throw CORBA::INTERNAL();

      TIDorb::core::ObjectDelegateImpl* obj_delegate =
        (TIDorb::core::ObjectDelegateImpl*)(forward_stub->_get_delegate());

      if(obj_delegate) {
        forwarded_ior =  obj_delegate->getReference();
      }  else {
        throw CORBA::INTERNAL();
      }

      throw TIDorb::core::ForwardRequest(forwarded_ior);
    }

  }
}


/**
  * The layer can use this IOR to stablish a remote connection
  */
bool TIDorb::core::comm::LocalCommLayer::accepts(const TIDorb::core::iop::IOR& ior)
{
    return _orb->getCommunicationManager()->getExternalLayer()->is_local(ior);  
}
    
    
/**
 * Creates a Delegate for a CORBA::Object
 */
TIDorb::core::ObjectDelegateImpl* 
    TIDorb::core::comm::LocalCommLayer::createDelegate(TIDorb::core::iop::IOR* ior)
{
    if(accepts(*ior)) {
        return new TIDorb::core::ObjectDelegateImpl(_orb, ior, this);
    } else {
        throw CORBA::INV_OBJREF();
    }
}
