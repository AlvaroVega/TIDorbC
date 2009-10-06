/////////////////////////////////////////////////////////////////////////
//
// File:        ExecThread.C
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

#include "TIDorb/core/poa/ExecThread.h"




TIDorb::core::poa::ExecThread::ExecThread(TIDorb::core::poa::POAManagerImpl* poaManager,
                                          TIDThr::ThreadGroup* group):
  TIDThr::Thread(group, NULL, "")
{
//   sched_param 		param;
//   pthread_attr_t	tatrb;
	
// #if !defined(hp1020ansi) && !defined(hp1020ansidebug)
//   pthread_attr_init(&tatrb);
//   pthread_attr_getschedparam(&tatrb,&param);
//   if ((param.sched_priority=sched_get_priority_max(sched_getscheduler(getpid())))==-1)
//   {
//   	fprintf(stderr,"Error estableciendo prioridad, estableciendo maxima manualmente\n");
//     fflush(stderr);
//     param.sched_priority=1;
//   }
//   pthread_attr_setschedparam(&tatrb,&param);
// //FRAN
//   pthread_attr_destroy(&tatrb);
// //EFRAN
// #else
//   pthread_attr_create(&tatrb);
//   pthread_attr_setprio(&tatrb,sched_get_priority_max(sched_getscheduler(getpid())));
// #endif

  _poaManager = poaManager;
  _queue = _poaManager->getRequestQueue();
  _firstTime = true;
  _deactivated = false;
  _threadStateListener = NULL;
  _thread_name = NULL;
  _max_blocked_time = _poaManager->orb->conf().max_blocked_time;
  _qos_enabled = _poaManager->orb->conf().qos_enabled;
}




TIDorb::core::poa::ExecThread::~ExecThread() throw()
{
  _poaManager->_remove_ref();
  CORBA::string_free(_thread_name);
}




void TIDorb::core::poa::ExecThread::setThreadStateListener(TIDorb::core::poa::ThreadStateListener* l)
{
  _threadStateListener = l;
}




CORBA::Boolean TIDorb::core::poa::ExecThread::checkState(TIDorb::core::poa::QueuedRequest* request)
{
  CORBA::Boolean executeRequest = false;

  if (_poaManager->orb->trace != NULL){
    TIDorb::util::StringBuffer  msg;
    msg << toString() << ": Checking state of POAManager";
    _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  {
    TIDThr::Synchronized synchro(_poaManager->stateMutex);

    CORBA::Boolean exit = false;
    while (!exit) {
      PortableServer::POAManager::State state = _poaManager->get_state();
      if ((state == PortableServer::POAManager::ACTIVE) && !request->getMustDiscard()) {
        exit = true;
        executeRequest = true;
      } else if (state == PortableServer::POAManager::HOLDING) {
        try {
          // wait until state changes
          _poaManager->stateMutex.wait();
        } catch(const TIDThr::InterruptedException& ie) {}
        exit = false;
      } else if (state == PortableServer::POAManager::INACTIVE) {
        // MISSING: define strategy for deactivated POAManagers
        request->returnError(CORBA::TRANSIENT(), _poaManager->orb->trace);
        exit = true;
        executeRequest = false;
      } else if ((state == PortableServer::POAManager::DISCARDING) || request->getMustDiscard()) {
        request->returnError(CORBA::TRANSIENT(),  _poaManager->orb->trace);
        exit = true;
        executeRequest = false;
      }
    }
  }
  return executeRequest;
}



//FRAN
void TIDorb::core::poa::ExecThread::processRequest(TIDorb::core::poa::QueuedRequest*& thisRequest)
//EFRAN
{
  if (_poaManager->orb->trace != NULL) {
    TIDorb::util::StringBuffer  msg;
    msg << toString() << ": Executing request" << thisRequest->toString();
    _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  try {
    if (checkState(thisRequest)) {
      // find poa
      TIDorb::core::poa::POAImpl* poa = NULL;

      try {
        poa = findPOA(thisRequest);
      } catch (const PortableServer::POA::AdapterNonExistent &ane) {
        if (_poaManager->orb->trace != NULL) {
          TIDorb::util::StringBuffer  msg;
          msg << toString() << "POA " << thisRequest->get_current_child_POA_name() << " not found";
          _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(),ane);
        }
        throw CORBA::OBJECT_NOT_EXIST();
      }

      if (poa != NULL) {
        execute(thisRequest,poa);
      }
//FRAN
      else {
        thisRequest = NULL;
      }
//EFRAN

    }
  } catch (const CORBA::SystemException &e) {
    thisRequest->returnError(e, _poaManager->orb->trace);

  } catch(const exception& thw) {
    CORBA::INTERNAL e(thw.what(), 0, thisRequest->getStatus());
    thisRequest->returnError(e, _poaManager->orb->trace);
  }

  _firstTime = false;
}




TIDorb::core::poa::POAImpl* TIDorb::core::poa::ExecThread::findPOA(TIDorb::core::poa::QueuedRequest* request)
  throw (PortableServer::POA::AdapterNonExistent, CORBA::SystemException)
{
  TIDorb::core::poa::POAImpl* current_poa = request->get_current_POA();

  while (!request->isFinalPOA()) {
    
    //jagd
    //PortableServer::POA_ptr aux_poa = current_poa->find_POA((const char*)request->get_current_child_POA_name(), true);
    //current_poa= dynamic_cast<TIDorb::core::poa::POAImpl*>(aux_poa);
    current_poa= current_poa->find_POA((const char*)request->get_current_child_POA_name(), true);

    request->next_child_POA(current_poa);
    PortableServer::POAManager_ptr aux_nextPOAManager = current_poa->the_POAManager();
    //jagd 
    //TIDorb::core::poa::POAManagerImpl* nextPOAManager = dynamic_cast<TIDorb::core::poa::POAManagerImpl*>(aux_nextPOAManager);
    TIDorb::core::poa::POAManagerImpl* nextPOAManager = (TIDorb::core::poa::POAManagerImpl*)(aux_nextPOAManager);

    if (nextPOAManager != _poaManager) {
      // bypass request to nextPOAManager
      if (_poaManager->orb->trace != NULL){
	TIDorb::util::StringBuffer  msg;
        msg << toString() << " bypassing request " << request->toString() << " to " << current_poa->toString();
        _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
      nextPOAManager->put(request);
      return NULL;
    }
  }

  return current_poa;
}




CORBA::Boolean TIDorb::core::poa::ExecThread::checkForeingState
  (TIDorb::core::poa::AbstractLocalRequest* thisRequest,
   TIDorb::core::poa::POAManagerImpl* current_manager)
{
  if (_poaManager->orb->trace != NULL){
    TIDorb::util::StringBuffer  msg;
    msg << toString() << " Checking foreign POAManager state";
    _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  PortableServer::POAManager::State state = current_manager->get_state();

  // chech current_manager state
  if (state == PortableServer::POAManager::HOLDING) {
    if (_poaManager->orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << " Waiting in a foreign POAManager because it is HOLDING";
      _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    CORBA::ULongLong timeout = _max_blocked_time;
   
    if (_qos_enabled) {

      TIDorb::core::PolicyContext* policyContext = NULL;
      policyContext = thisRequest->getPolicyContext();
      
      if (policyContext != NULL) {
        timeout = TIDorb::core::messaging::QoS::checkRequestTime(_poaManager->orb, 
                                                                 *policyContext);
        delete policyContext;
      }
    }


    {
      TIDThr::Synchronized synchro(current_manager->stateMutex);
      
      try {
        current_manager->stateMutex.wait(timeout);
      } catch (const TIDThr::InterruptedException& ie) {
      }
      
    }

    state = current_manager->get_state();

    current_manager->put(thisRequest);
    if (state == PortableServer::POAManager::HOLDING) {
      if (_poaManager->orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << toString() << " NO_RESPONSE because foreign POAManager is HOLDING";
        _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
      thisRequest->returnError(CORBA::NO_RESPONSE(), _poaManager->orb->trace);
      return false;
    }
  }

  if (state == PortableServer::POAManager::INACTIVE) {  
    thisRequest->returnError(CORBA::TRANSIENT(), _poaManager->orb->trace);
    return false;
  }

  if ((state == PortableServer::POAManager::DISCARDING) || thisRequest->getMustDiscard()) {
    thisRequest->returnError(CORBA::TRANSIENT(), _poaManager->orb->trace);
    return false;
  }

  return true;
}




void TIDorb::core::poa::ExecThread::execute(TIDorb::core::poa::QueuedRequest* request, TIDorb::core::poa::POAImpl* poa)
{
  // check if the reference is no a transient value, and is out of its poa context
  CORBA::ULongLong poa_id = request->getPOAKey()->getPOAId();

  if((poa_id != 0) && (poa_id != poa->the_id())) {
    request->returnError(CORBA::OBJECT_NOT_EXIST(), _poaManager->orb->trace);
    return;
  }

  const TIDorb::core::poa::OID& oid = request->getOID();


  // execute request
  //jagd 2
  //CurrentInfo* current = new CurrentInfo(poa, request->getOID());
  //jagd 3
  //const TIDorb::core::poa::OID & _oid=request->getOID();
  CurrentInfo  current(poa, oid);
  //CurrentInfo  current(poa, _oid);
  _currentInfoStack.push(&current);

  try
  {
    //jagd se cambia el poco estilizado bloque if-else-if
    //por el patron double dispatching 
    /* 
    const char* tipobase = request->_typeid();

    if (tipobase==NULL)
      throw CORBA::INTERNAL();

    if (tipobase[0]=='I') // IIOP request
    {
      if (tipobase[4]=='R')
        execute((TIDorb::core::poa::IIOPRequest*) request->_impl(), poa);
      else
        execute((TIDorb::core::poa::IIOPLocateRequest*) request->_impl(), poa);
    }
    else if (tipobase[0]=='L') // Local request
    {
      if (tipobase[5]=='R')
        execute((TIDorb::core::poa::LocalRequest*) request->_impl(), poa);
      else
        execute((TIDorb::core::poa::LocalLocateRequest*) request->_impl(), poa);
    }
    else if (tipobase[0]=='M') // MIOP request
    {
      execute((TIDorb::core::goa::MIOPRequest*) request->_impl(), poa);
    }
    else
    {
      throw CORBA::INTERNAL();
    }
    //EPRA
   */ 
    //jagd la funcion dispatch es la encargada de seleccionar el objeto y dentro se hace el segundo 
    // dispatch por polimorfismo
    request->dispatch(this,poa);
   
    request->setStatus(CORBA::COMPLETED_YES);

  } catch (const CORBA::SystemException &e) {
    request->returnError(e, _poaManager->orb->trace);
  } catch(const exception& thw) {
    request->returnError(CORBA::INTERNAL(thw.what(), 0, request->getStatus()), _poaManager->orb->trace);
  } catch (...) {
  }

  //jagd 2
  //CurrentInfo* info = _currentInfoStack.top();
  _currentInfoStack.pop();
 
  //jagd 2 
  //if(current->is_request_in_POA()) {
  if(current.is_request_in_POA()) {
    try {
      poa->removeUser(oid);
    } catch (const CORBA::Exception &e) {}
    //PRA
    //request->destroy();
    //EPRA
  }

//MLG: This will delete current and info that points to the same
//     It should be here because current is consulted at is_request_in_POA
  //jagd 2 
  //delete info;
//EMLG  
}




void TIDorb::core::poa::ExecThread::execute(TIDorb::core::poa::IIOPRequest* request, TIDorb::core::poa::POAImpl* poa)
{
  // get serverRequest (with params for invocation)
  TIDorb::core::ServerRequestImpl* serverRequest = request->get_request();


  if (_qos_enabled) {
    // QoS validation
    TIDorb::core::PolicyContext* policyContext;
    policyContext = request->getPolicyContext();
    
    if (policyContext != NULL){
      if(!TIDorb::core::messaging::QoS::validateServerRequestEndTimePolicy(
                                                             *policyContext)) {
      
        if (_poaManager->orb->trace != NULL){
          TIDorb::util::StringBuffer msg;
          msg << toString() << " discarding " << request->toString();
          msg << " due to Messaging::RequestEndTimePolicy: TIMEOUT" << flush;
          _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        throw CORBA::TIMEOUT();
      }
    }
  }

  TIDorb::core::comm::Connection* conn = request->get_connection();

  if (!conn->is_open() && serverRequest->with_response()) {
    if (_poaManager->orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << toString() << " discarding " << serverRequest->operation();
      msg << " due to connection has been closed" << flush;
      _poaManager->orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
    return;
  }

  // get servant
//MLG  
  PortableServer::ServantLocator::Cookie cookie = NULL;
//EMLG  
  CORBA::Boolean servantLocatorUsed = false;

  PortableServer::ServantBase* servant = NULL;

  const TIDorb::core::poa::OID& oid = request->getOID();
 
  try {
//MLG
    servant = poa->find_servant(oid, serverRequest->operation(), &cookie, &servantLocatorUsed);
//EMLG

    poa->addUser(oid);

    CurrentInfo* current_info =_currentInfoStack.top();
    current_info->set_request_in_POA(true);
    current_info->setServant(servant);

  } catch (const PortableServer::POA::ObjectNotActive& e) {
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "", e);
    }
    throw CORBA::OBJECT_NOT_EXIST(e._name(),1,request->getStatus());
  } catch (const PortableServer::POA::ObjectAlreadyActive& e) {
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "", e);
    }
    throw CORBA::OBJ_ADAPTER(e._name(),0,request->getStatus());
  } catch (const PortableServer::ForwardRequest& e) {
    serverRequest->set_forward(e.forward_reference);

    if (serverRequest->with_response()) {
      conn->send_reply(serverRequest);
    }
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "Forwarded", e);
    }
    return;
  }

  // set servant in current object
  //PRA
  //CurrentInfo* current = _currentInfoStack.top();
  //current->setServant(servant);
  //EPRA

  request->setStatus(CORBA::COMPLETED_MAYBE);

  //if (serverRequest->operation()->equals("_is_a")) {
  if (strcmp(serverRequest->operation(), "_is_a")==0) {
    invoke_is_a(poa, servant, serverRequest);
  } else {
    PortableServer::DynamicImplementation* dynServant = dynamic_cast<PortableServer::DynamicImplementation*>(servant);
    //if (servant instanceof DynamicImplementation) {
    // invoke servant using DII
    //CORBA::DynamicImplementation* dynServant = (CORBA::DynamicImplementation*)servant;
    if (dynServant) {
      if (poa->isSingleThread()) {
        {
          TIDThr::Synchronized synchro(*(poa->get_single_thread_recursive_mutex()));
          do_invoke(dynServant, serverRequest);
        }
      } else {
        do_invoke(dynServant, serverRequest);
      }
    } else {
      throw CORBA::NO_IMPLEMENT("Stream-based invocation not implemented", 0, CORBA::COMPLETED_NO);
    }
  }

  request->setStatus(CORBA::COMPLETED_YES);
  // return results (if not oneway)
  if (serverRequest->with_response()) {
    conn->send_reply(serverRequest, poa->getPolicyContext());
  }

  // Call postinvoke if necessary
  if (servantLocatorUsed) {
    poa->call_postinvoke(*oid.toByteArray(), serverRequest->operation(), cookie, servant);
  }
}




void TIDorb::core::poa::ExecThread::execute(TIDorb::core::goa::MIOPRequest* request, TIDorb::core::poa::POAImpl* poa)
{
  // get serverRequest (with params for invocation)
  TIDorb::core::ServerRequestImpl* serverRequest = request->get_request();

  TIDorb::core::comm::Connection* conn = request->get_connection();

  if (!conn->is_open() && serverRequest->with_response()) {
    if (_poaManager->orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << toString() << " discarding " << serverRequest->operation();
      msg << " due to connection has been closed" << flush;
      _poaManager->orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
    return;
  }

  // get servant
//MLG  
  PortableServer::ServantLocator::Cookie cookie = NULL;
//EMLG
  CORBA::Boolean servantLocatorUsed = false;

  PortableServer::ServantBase* servant = NULL;

  const TIDorb::core::poa::OID& oid = request->getOID();

  try {
//MLG
    servant = poa->find_servant(oid, serverRequest->operation(), &cookie, &servantLocatorUsed);
//EMLG

    poa->addUser(oid);

    CurrentInfo* current_info =_currentInfoStack.top();
    current_info->set_request_in_POA(true);
    current_info->setServant(servant);

  } catch (const PortableServer::POA::ObjectNotActive& e) {
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "", e);
    }
    throw CORBA::OBJECT_NOT_EXIST(e._name(),1,request->getStatus());
  } catch (const PortableServer::POA::ObjectAlreadyActive& e) {
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "", e);
    }
    throw CORBA::OBJ_ADAPTER(e._name(),0,request->getStatus());
  } catch (const PortableServer::ForwardRequest& e) {
    serverRequest->set_forward(e.forward_reference);

    if (serverRequest->with_response()) {
      conn->send_reply(serverRequest);
    }
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "Forwarded", e);
    }
    return;
  }

  // set servant in current object
  //PRA
  //CurrentInfo* current = _currentInfoStack.top();
  //current->setServant(servant);
  //EPRA

  request->setStatus(CORBA::COMPLETED_MAYBE);

  //if (serverRequest->operation()->equals("_is_a")) {
  if (strcmp(serverRequest->operation(), "_is_a")==0) {
    invoke_is_a(poa, servant, serverRequest);
  } else {
    PortableServer::DynamicImplementation* dynServant = dynamic_cast<PortableServer::DynamicImplementation*>(servant);
    //if (servant instanceof DynamicImplementation) {
    // invoke servant using DII
    //CORBA::DynamicImplementation* dynServant = (CORBA::DynamicImplementation*)servant;
    if (dynServant) {
      if (poa->isSingleThread()) {
        {
          TIDThr::Synchronized synchro(*(poa->get_single_thread_recursive_mutex()));
          do_invoke(dynServant, serverRequest);
        }
      } else {
        do_invoke(dynServant, serverRequest);
      }
    } else {
      throw CORBA::NO_IMPLEMENT("Stream-based invocation not implemented", 0, CORBA::COMPLETED_NO);
    }
  }

  request->setStatus(CORBA::COMPLETED_YES);
  // return results (if not oneway)
  if (serverRequest->with_response()) {
    conn->send_reply(serverRequest);
  }

  // Call postinvoke if necessary
  if (servantLocatorUsed) {
    poa->call_postinvoke(*oid.toByteArray(), serverRequest->operation(), cookie, servant);
  }
}




void TIDorb::core::poa::ExecThread::execute(TIDorb::core::poa::IIOPLocateRequest* request, TIDorb::core::poa::POAImpl* poa)
{
  TIDorb::core::comm::Connection* conn = request->get_connection();

  if (!conn->is_open()) {
    return;
  }

  const TIDorb::core::comm::iiop::Version& version = request->get_version();

  TIDorb::core::comm::iiop::RequestId id = request->get_id();

  CORBA::Boolean servantLocatorUsed = false;

  const TIDorb::core::poa::OID& oid = request->getOID();

  try {
    // get servant
    PortableServer::ServantBase* servant = poa->find_servant(oid, NULL, NULL, &servantLocatorUsed);
    if (servant != NULL) {
      conn->send_locate_reply(version, id, true);
    } else {
      throw CORBA::INTERNAL("find_servant() returning null!!", 0, CORBA::COMPLETED_NO);
    }
  } catch (const CORBA::SystemException& e){
    conn->send_locate_reply(version, id, e);
  } catch (const PortableServer::ForwardRequest& e) {
    conn->send_locate_reply(version, id, e.forward_reference);
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "Forwarded", e);
    }
    return;
  } catch (const CORBA::Exception& e) {
    conn->send_locate_reply(version, id, false);
  }
}




void TIDorb::core::poa::ExecThread::execute(TIDorb::core::poa::LocalRequest* localRequest, TIDorb::core::poa::POAImpl* poa)
{
  // get serverRequest (with params for invocation)
  TIDorb::core::LocalServerRequest* serverRequest = localRequest->get_request();


  if (_qos_enabled) {
    // QoS validation
    TIDorb::core::PolicyContext* policyContext; 
    policyContext = localRequest->getPolicyContext();

    if (policyContext != NULL){
      if(!TIDorb::core::messaging::QoS::validateServerRequestEndTimePolicy(*policyContext))  {
        
        if (_poaManager->orb->trace != NULL){
          TIDorb::util::StringBuffer msg;
          msg << toString() << " discarding " << localRequest->toString();
          msg << " due to Messaging::RequestEndTimePolicy: TIMEOUT" << flush;
          _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        throw CORBA::TIMEOUT();
      }
    }
  }


  // get servant
//MLG
  PortableServer::ServantLocator::Cookie cookie = NULL;
//EMLG
  CORBA::Boolean servantLocatorUsed = false;

  PortableServer::ServantBase* servant = NULL;

  const TIDorb::core::poa::OID& oid = localRequest->getOID();

  try {

    servant = poa->find_servant(oid, serverRequest->operation(),
                                &cookie, &servantLocatorUsed);

    poa->addUser(oid);

    CurrentInfo* current_info = _currentInfoStack.top();
    current_info->set_request_in_POA(true);
    current_info->setServant(servant);

  } catch (const PortableServer::POA::ObjectNotActive& e) {
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "", e);
    }
    throw CORBA::OBJECT_NOT_EXIST(e._name(),1,localRequest->getStatus());
  } catch (const PortableServer::POA::ObjectAlreadyActive& e) {
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "", e);
    }
    throw CORBA::OBJ_ADAPTER(e._name(),0,localRequest->getStatus());
  } catch (const PortableServer::ForwardRequest& e) {
    serverRequest->set_forward(e.forward_reference);
    if (_poaManager->orb->trace != NULL){
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, "Forwarded", e);
    }
    localRequest->set_completed();
    return;
  }

  // set servant in current object
  //PRA
  //CurrentInfo* current = _currentInfoStack.top();
  //current->setServant(servant);
  //EPRA

  localRequest->setStatus(CORBA::COMPLETED_MAYBE);

  if (strcmp(serverRequest->operation(), "_is_a")==0) {
    invoke_is_a(poa, servant, serverRequest);
  } else {
    // invoke servant using DII

    PortableServer::DynamicImplementation* dynServant =
    dynamic_cast < PortableServer::DynamicImplementation* > (servant);

    if (dynServant) {
      if (poa->isSingleThread()) {
        TIDThr::Synchronized synchro(*(poa->get_single_thread_recursive_mutex()));
        do_invoke(dynServant, serverRequest);
      } else {
        do_invoke(dynServant, serverRequest);
      }
      // fix the out parameters in the request

      serverRequest->fix_out_arguments();

    } else {
      throw CORBA::NO_IMPLEMENT("Stream-based invocation not implemented", 0, CORBA::COMPLETED_NO);
    }
  }

  localRequest->setStatus(CORBA::COMPLETED_YES);

  // Call postinvoke if necessary
  if (servantLocatorUsed) {
    poa->call_postinvoke(*(oid.toByteArray()), serverRequest->operation(), 
                         cookie, servant);
  }

  localRequest->set_completed();
}




void TIDorb::core::poa::ExecThread::execute(TIDorb::core::poa::LocalLocateRequest* request, TIDorb::core::poa::POAImpl* poa)
{
  // get servant
  CORBA::Boolean servantLocatorUsed = false;

  PortableServer::ServantBase* servant = NULL;

  try {
    servant = poa->find_servant(request->getOID(), NULL, NULL, &servantLocatorUsed);
  } catch(const CORBA::SystemException& e) {
    throw;
  } catch (const PortableServer::ForwardRequest& fr) {
    request->set_forward(fr.forward_reference);
    
    if (_poaManager->orb->trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << toString() << " Forwarded";
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }

    request->set_completed();
    return;
  } catch(const CORBA::Exception& e) {
    request->found = false;
  }

  if (servant != NULL) {
    request->found = true;
  } else {
    throw CORBA::INTERNAL("find_servant() returning null!!", 0, CORBA::COMPLETED_NO);
  }

  request->set_completed();
}




void TIDorb::core::poa::ExecThread::invoke_is_a(TIDorb::core::poa::POAImpl* poa, PortableServer::ServantBase* servant, CORBA::ServerRequest* request)
{
  try {
    /* Bug en DSI
    TIDorb::portable::ServantDelegate* aux_deleg = servant->get_delegate();
    TIDorb::core::poa::ServantDelegate* deleg = dynamic_cast<TIDorb::core::poa::ServantDelegate*>(aux_deleg);
    */

    TIDorb::core::TIDORB* orb = poa->orb;

    CORBA::NVList_var params;
    orb->create_list(1, params);
    
    //jagd 3 CORBA::NamedValue_var s = params->add_item("s", CORBA::ARG_IN);
    CORBA::NamedValue * s = params->add_item("s", CORBA::ARG_IN);
//FRAN
    // Fix bug [#392] Any::type(tc) reset any value
    //s->value()->type(CORBA::_tc_string);
    s->value()->delegate().set_type(CORBA::_tc_string);
//EFRAN
    request->arguments(params);

    const char* itf;
    (*(s->value())) >>= itf;
    
    /* Bug en DSI
    CORBA::Boolean result = deleg->is_a(servant, itf);
    */

    CORBA::Boolean result = servant->_is_a(itf);

    CORBA::Any resultAny;
    resultAny <<= CORBA::Any::from_boolean(result);
    
    request->set_result(resultAny);

  } catch (const CORBA::SystemException& se) {
    throw;

  } catch (...) {

    if (_poaManager->orb->trace != NULL){
      TIDorb::util::StringBuffer  msg;
      msg << toString() << "Exception in servant invoke _is_a method, UNKNOWN thrown: ";
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(), CORBA::UNKNOWN());
    }
    throw CORBA::UNKNOWN();
  }
}




void TIDorb::core::poa::ExecThread::do_invoke(PortableServer::DynamicImplementation* servant, CORBA::ServerRequest* request)
{
  try {
    servant->invoke(request);
  } catch (const CORBA::SystemException& se) {
    throw;
  } catch (...) {
    if (_poaManager->orb->trace != NULL){
       TIDorb::util::StringBuffer  msg;
      msg << toString() << "Exception in servant invoke method, UNKNOWN thrown: ";
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(), CORBA::UNKNOWN());
    }
    throw CORBA::UNKNOWN();
  }
}




void TIDorb::core::poa::ExecThread::run()
{
  TIDorb::core::poa::QueuedRequest* thisRequest = NULL;

  while (!_deactivated) {
 
    try {
      if (!_firstTime){
        _threadStateListener->setInactive(this);
      }

      if (_poaManager->orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << toString() << ": Getting requests..." << ends;
        _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
//MLG
      thisRequest = _queue->get();
//EMLG      

      _threadStateListener->setActive(this, _firstTime);

      if (thisRequest != NULL) {
        // If there is a request, then go!!
        processRequest(thisRequest);
      } else if (_threadStateListener->threadCanDie(this)) {
        // If restarted and no request, then commit suicide
        _deactivated = true;
      }

    } catch (const CORBA::Exception& unhandledException) {
      // Unhandled exception. Should never happen!!
      if (_poaManager->orb->trace != NULL){
        TIDorb::util::StringBuffer  msg;
        msg << toString() << "Unhandled Exception ";
        _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(), unhandledException);
      }
      _deactivated = true;
    }
    
    if(thisRequest) {
      //PRA
      //delete thisRequest;
      thisRequest->destroy();
      //EPRA
      thisRequest = NULL;
    }
  }
  _threadStateListener->threadHasDied(this);
}




void TIDorb::core::poa::ExecThread::processLocalRequest(TIDorb::core::poa::AbstractLocalRequest* thisRequest,
  TIDorb::core::poa::POAManagerImpl* current_manager)
{
  if (_poaManager->orb->trace != NULL){
    TIDorb::util::StringBuffer  msg;
    msg << toString() << ": Executing local request " << thisRequest->toString();
    _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  try {

    if(current_manager == _poaManager) {
      if(!checkState(thisRequest))
      return;
    } else {
      if(!checkForeingState(thisRequest,current_manager))
      return;
    }

    // find poa

    TIDorb::core::poa::POAImpl* current_poa = thisRequest->get_current_POA();

    try {
      while (!thisRequest->isFinalPOA()) {
        //jagd
        //PortableServer::POA_ptr aux_poa=current_poa->find_POA((const char*)thisRequest->get_current_child_POA_name(), true);
        //current_poa = dynamic_cast<TIDorb::core::poa::POAImpl*>(aux_poa);
        current_poa=current_poa->find_POA((const char*)thisRequest->get_current_child_POA_name(), true); 
         
        thisRequest->next_child_POA(current_poa);

        PortableServer::POAManager_ptr aux_nextPOAManager = current_poa->the_POAManager();
        //jagd
        //TIDorb::core::poa::POAManagerImpl* nextPOAManager = dynamic_cast<TIDorb::core::poa::POAManagerImpl*>(aux_nextPOAManager);
        TIDorb::core::poa::POAManagerImpl* nextPOAManager = (TIDorb::core::poa::POAManagerImpl*)(aux_nextPOAManager);

        if (nextPOAManager != _poaManager) {
          // bypass request to nextPOAManager

          if (_poaManager->orb->trace != NULL){
            TIDorb::util::StringBuffer  msg;
            msg << toString() << " bypassing local request " << thisRequest->toString();
            msg << " through "<< current_poa->toString();
            _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
          }

          processLocalRequest(thisRequest,nextPOAManager);
          return;
        }
      }
    } catch (const PortableServer::POA::AdapterNonExistent &ane){

      if (_poaManager->orb->trace != NULL){
        TIDorb::util::StringBuffer  msg;
        msg << toString() << "POA " << thisRequest->get_current_child_POA_name();
        msg << " not found, CORBA::OBJECT_NOT_EXIST thrown";
        _poaManager->orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data(),ane);
      }

      thisRequest->returnError(CORBA::OBJECT_NOT_EXIST(), _poaManager->orb->trace);
      return;
    }

    if(current_poa != NULL)
      execute(thisRequest, current_poa);

  } catch (const CORBA::SystemException &e){
    thisRequest->returnError(e, _poaManager->orb->trace);
  } catch (const exception &tw){
    thisRequest->returnError(CORBA::INTERNAL(tw.what(), 0, thisRequest->getStatus()), 
                             _poaManager->orb->trace);
  }
  _firstTime = false;
}




const char* TIDorb::core::poa::ExecThread::toString()
{
  if (_thread_name == NULL) {
    TIDorb::util::StringBuffer buffer;
    buffer << ((TIDThr::Thread&)*this) << ", " << _poaManager->toString();
    _thread_name = CORBA::string_dup(buffer.str().data());
  }
  return _thread_name;
}




TIDorb::core::poa::CurrentInfo* TIDorb::core::poa::ExecThread::getCurrentInfo()
{
  if (_currentInfoStack.empty()) {
    return NULL;
  }
 
  return _currentInfoStack.top();
}


CORBA::Boolean 
TIDorb::core::poa::ExecThread::belongsToPOAManager(
                                   TIDorb::core::poa::POAManagerImpl* poaManager)
{
  
  return (poaManager == _poaManager);
}
