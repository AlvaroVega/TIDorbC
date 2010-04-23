/////////////////////////////////////////////////////////////////////////
//
// File:        POAManagerImpl.C
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

#include "TIDorb/core/poa/POAManagerImpl.h"


void* TIDorb::core::poa::POAManagerImpl::_impl()
{
	return this;
}

const char* TIDorb::core::poa::POAManagerImpl::_typeid()
{
	//return CORBA::string_dup("POAManagerImpl");
	return "POAManagerImpl";
}



TIDorb::core::util::Counter TIDorb::core::poa::POAManagerImpl::_serial;




TIDorb::core::poa::POAManagerImpl::POAManagerImpl(TIDorb::core::TIDORB* the_orb)
  : _completion(the_orb)
{
  orb = the_orb;

  destroyed = false;
  orb->POAManagers.push_back((PortableServer::POAManager*) this);

  CORBA::ULong manager_id = _serial.post_inc();

  TIDorb::util::StringBuffer buffer;
  buffer << "POAManager " << manager_id << ends;
  _manager_name = CORBA::string_dup(buffer.str().data());

  _state = PortableServer::POAManager::HOLDING;

  const TIDorb::core::ConfORB& orb_conf = the_orb->conf();

  conf = new POAManagerConf(orb_conf.poa_min_threads,
                            orb_conf.poa_max_threads,
                            orb_conf.poa_max_queued_requests,
                            orb_conf.poa_starving_time,
                            orb_conf.poa_thread_stack_size,
                            orb_conf.poa_bound_threads);

  _poas = new vector<PortableServer::POA_var>;

  _pool = new ThreadPool(this);
  conf->setListener(_pool);
  
  _requestQueue = new RequestQueue(this, _pool, //TEMPORAL_REQUEST_COMPARATOR
                                   NULL_REQUEST_COMPARATOR);
  _pool->minThreadsHasChanged();

//  m_queue_order = Messaging::ORDER_TEMPORAL;
}




TIDorb::core::poa::POAManagerImpl::~POAManagerImpl()
{
  CORBA::string_free(_manager_name);
  delete _pool;
  delete _poas;
  delete _requestQueue;
  delete conf;
}




/**
 * Sets the mininum number of execution threads
 */
void TIDorb::core::poa::POAManagerImpl::set_min_threads(CORBA::ULong min_threads)
{
  conf->setMinThreads(min_threads);
}




/**
 * Sets the maximum number of execution threads
 */
void TIDorb::core::poa::POAManagerImpl::set_max_threads(CORBA::ULong max_threads)
{
  conf->setMaxThreads(max_threads);
}




/**
 * Sets the maximum number of request queued in the POAManager
 */
void TIDorb::core::poa::POAManagerImpl::set_max_queued_requests(CORBA::ULong max_queued_requests)
{
  conf->setMaxQueuedRequests(max_queued_requests);
}




/**
 * Sets the maximum time an execution thread is inactive.
 */
void TIDorb::core::poa::POAManagerImpl::set_starving_time(CORBA::ULong millisecs)
{
  conf->setStarvingTime(millisecs);
}




/**
 * @return maximum number of request queued in the POAManager
 */
CORBA::ULong TIDorb::core::poa::POAManagerImpl::get_max_queued_requests()
{
  return conf->getMaxQueuedRequests();
}




/**
 * @return the maximum number of execution threads
 */
CORBA::ULong TIDorb::core::poa::POAManagerImpl::get_max_threads()
{
  return conf->getMaxThreads();
}




/**
 * @return the mininum number of execution threads
 */
CORBA::ULong TIDorb::core::poa::POAManagerImpl::get_min_threads()
{
  return conf->getMinThreads();
}




/**
 * @return the maximum time an execution thread is inactive.
 */
CORBA::ULong TIDorb::core::poa::POAManagerImpl::get_starving_time()
{
  return conf->getStarvingTime();
}




/**
 * @return The associated POAManagerConf object.
 */
TIDorb::core::poa::POAManagerConf* TIDorb::core::poa::POAManagerImpl::getConf()
{
  return conf;
}




/**
 * Adds a POA to this POAManager.
 * @param poa The POA.
 */
void TIDorb::core::poa::POAManagerImpl::addPOA(PortableServer::POA_ptr poa)
{
  TIDThr::Synchronized synchro(monitor);
  _poas->push_back(poa);
}




/**
 * @return The request queue of this POAManager.
 */
TIDorb::core::poa::RequestQueue* TIDorb::core::poa::POAManagerImpl::getRequestQueue()
{
  return _requestQueue;
}




/**
 * @return The thread pool of this POAManager.
 */
TIDorb::core::poa::ThreadPool* TIDorb::core::poa::POAManagerImpl::getThreadPool()
{
  return _pool;
}




/* ************ State changes ************* */



/**
 * This operation changes the POA manager to ACTIVE.
 * @exception PortableServer::POAManager::AdapterInactive  If POA manager state is INACTIVE.
 */
void TIDorb::core::poa::POAManagerImpl::activate()
  throw(PortableServer::POAManager::AdapterInactive, CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST("POAManager destroyed.");

  // State change -> ACTIVE
  {
    TIDThr::Synchronized synchro(stateMutex);
    if (_state == PortableServer::POAManager::INACTIVE) {
      throw PortableServer::POAManager::AdapterInactive();
    }
    if (_state != PortableServer::POAManager::ACTIVE) {
      _completion.stopWaiting();
      _state = PortableServer::POAManager::ACTIVE;
      stateMutex.notifyAll();
    }
  }
}




/**
 * This operation changes the POA manager to HOLDING.
 * @param wait_for_completion Wait-for-completion flag.
 * @exception PortableServer::POAManager::AdapterInactive  If POA manager state is INACTIVE.
 */
void TIDorb::core::poa::POAManagerImpl::hold_requests(CORBA::Boolean wait_for_completion)
  throw (PortableServer::POAManager::AdapterInactive, CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST("POAManager destroyed.");

  // State change -> HOLDING
  {
    TIDThr::Synchronized synchro(stateMutex);
    if (_state == PortableServer::POAManager::INACTIVE) {
      throw PortableServer::POAManager::AdapterInactive();
    }
    if (_state != PortableServer::POAManager::HOLDING) {
      _completion.stopWaiting();
      _state = PortableServer::POAManager::HOLDING;
      stateMutex.notifyAll();
    }
  }

  // Wait for completion, if necessary
  // if (wait_for_completion && _completion->conditionToWait()) {
  //   _completion->waitForCompletion(); // synchronized
  // }
  if (wait_for_completion && _completion.conditionToWait()) {
    _completion.waitForCompletion(); // synchronized
  }
}




/**
 * This operation changes the POA manager to DISCARDING.
 * @param wait_for_completion Wait-for-completion flag.
 * @exception PortableServer::POAManager::AdapterInactive  If POA manager state is INACTIVE.
 */
void TIDorb::core::poa::POAManagerImpl::discard_requests(bool wait_for_completion)
  throw(PortableServer::POAManager::AdapterInactive, CORBA::SystemException)
  // State change -> DISCARDING
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST("POAManager destroyed.");

  {
    TIDThr::Synchronized synchro(stateMutex);

    if (_state == PortableServer::POAManager::INACTIVE) {
      throw PortableServer::POAManager::AdapterInactive();
    }

    if (_state != PortableServer::POAManager::DISCARDING) {
      _completion.stopWaiting();
      _state = PortableServer::POAManager::DISCARDING;
      stateMutex.notifyAll();
    }
  }

  // Wait for completion, if necessary

  // if (wait_for_completion && _completion->conditionToWait()) {
  //   _completion->waitForCompletion(); // synchronized
  // }
  if (wait_for_completion && _completion.conditionToWait()) {
    _completion.waitForCompletion(); // synchronized
  }
}




/**
 * This operation changes the POA manager to DEACTIVATE.
 * @param etherealize_object If it is true, then all objects must be etherealized.
 * @param wait_for_completion Wait-for-completion flag.
 * @exception PortableServer::POAManager::AdapterInactive  If POA manager state is INACTIVE.
 */

void TIDorb::core::poa::POAManagerImpl::deactivate(CORBA::Boolean etherealize_objects, bool wait_for_completion)
  throw (PortableServer::POAManager::AdapterInactive, CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST("POAManager destroyed.");

  // State change -> INACTIVE
  {
    TIDThr::Synchronized synchro(stateMutex);

    if (_state == PortableServer::POAManager::INACTIVE) {
      throw PortableServer::POAManager::AdapterInactive();
    }

    // reset the last completion waiters
    _completion.stopWaiting();

    _pool->deactivation();

    _requestQueue->deactivation();

    _state = PortableServer::POAManager::INACTIVE;

    {
      TIDThr::Synchronized synchro(monitor);

      vector<PortableServer::POAManager*>::iterator iter;
      for (iter = orb->POAManagers.begin(); iter != orb->POAManagers.end(); iter++) {
        if (*iter == (PortableServer::POAManager*) this) {
          orb->POAManagers.erase(iter);
          break;
        }
      }
    }
  }

  // Wait for completion, if necessary
  // if (wait_for_completion && _completion->conditionToWait()) {
  //   _completion->waitForCompletion(); //synchronized
  if (wait_for_completion && _completion.conditionToWait()) {
    _completion.waitForCompletion(); //synchronized
    if (etherealize_objects) {
      // Etherealize objects (blocking)
      etherealizeAllPOAs();
    }
  } else if (etherealize_objects) {
    // Etherealize in background
    TIDorb::core::poa::POAManagerImpl::EtherealizerThread* t = new EtherealizerThread(this);
    t->start();
  }
}




/**
 * @return Return the state of the POA manager.
 */
PortableServer::POAManager::State TIDorb::core::poa::POAManagerImpl::get_state()
  throw()
{
  return _state;
}




/* ************ TIDorb methods - POA Location ************* */


/**
 * Puts a new request in this POA manager.
 * @param request The request.
 */
void TIDorb::core::poa::POAManagerImpl::put(TIDorb::core::poa::QueuedRequest* request)
{
  _requestQueue->add(request);
}




/**
 * Removes a POA from this POA manager.
 * @param poa The POA being removed.
 */
void TIDorb::core::poa::POAManagerImpl::removePOA(PortableServer::POA_ptr poa)
{
  TIDThr::Synchronized synchro(monitor);

  vector<PortableServer::POA_var>::iterator iter;
  for (iter = _poas->begin(); iter != _poas->end(); iter++) {
    if (*iter == poa) {
      _poas->erase(iter);
      break;
    }
  }
}




/**
 * Finds a POA in this POA manager.
 * @param poaName The POA name.
 */
PortableServer::POA_ptr TIDorb::core::poa::POAManagerImpl::findPOA(const char* poaName)
{
  TIDThr::Synchronized synchro(monitor);

  vector<PortableServer::POA_var>::iterator iter;
  vector<PortableServer::POA_var>::const_iterator end = _poas->end();
  for (iter = _poas->begin(); iter != end; iter++) {
    PortableServer::POA_ptr poa = (PortableServer::POA_ptr) *iter;
    if (strcmp(poa->the_name(),poaName)==0) {
      return poa;
    }
  }
  return NULL;
}




/**
 * Begins the execution of a request.
 */
void TIDorb::core::poa::POAManagerImpl::beginRequest()
{
  _completion.beginRequest();
}




/**
 * Ends the execution of a request.
 */
void TIDorb::core::poa::POAManagerImpl::endRequest()
{
  _completion.endRequest();
}




/**
 * Etherealizes all POAs (due to POA manager deactivation).
 */
void TIDorb::core::poa::POAManagerImpl::etherealizeAllPOAs()
{
  TIDThr::Synchronized synchro(monitor);

  vector<PortableServer::POA_var>::iterator iter;
  vector<PortableServer::POA_var>::const_iterator end = _poas->end();
  for (iter = _poas->begin(); iter != end; iter++) {
    PortableServer::POA_ptr poa = (PortableServer::POA_ptr) *iter;
    TIDorb::core::poa::POAImpl* poaimpl = dynamic_cast<TIDorb::core::poa::POAImpl*>(poa);
    poaimpl->etherealizeAllObjects();
  }
}




const char* TIDorb::core::poa::POAManagerImpl::toString()
{
  return _manager_name;
}




/**
 * Class to etherealize all POAs in background.
 */
TIDorb::core::poa::POAManagerImpl::EtherealizerThread::EtherealizerThread(TIDorb::core::poa::POAManagerImpl* the_poaManager)
{
  poaManager = the_poaManager;
}




void TIDorb::core::poa::POAManagerImpl::EtherealizerThread::run()
{
  poaManager->etherealizeAllPOAs();
}

void TIDorb::core::poa::POAManagerImpl::setQueueOrderPolicy(CORBA::Policy_ptr policy) 
{
  TIDorb::core::messaging::QueueOrderPolicyImpl* _policy = 
    dynamic_cast<TIDorb::core::messaging::QueueOrderPolicyImpl*> (policy);
  Messaging::Ordering order = _policy->allowed_orders();
  
  this->set_queue_order(order);
}


void TIDorb::core::poa::POAManagerImpl::set_queue_order(Messaging::Ordering order)
{

  if( (order == Messaging::ORDER_PRIORITY) && 
      (m_queue_order != Messaging::ORDER_PRIORITY) ){
    _requestQueue->setComparator(PRIORITY_REQUEST_COMPARATOR);
    m_queue_order = Messaging::ORDER_PRIORITY;
  } else if ( (order == Messaging::ORDER_DEADLINE) && 
              (m_queue_order != Messaging::ORDER_DEADLINE) ) {
    _requestQueue->setComparator(DEADLINE_REQUEST_COMPARATOR);
    m_queue_order = Messaging::ORDER_DEADLINE;
  } else if ((m_queue_order != Messaging::ORDER_TEMPORAL)
             || (m_queue_order != Messaging::ORDER_ANY)) { 
    // default comparator will be used            
    _requestQueue->setComparator(TEMPORAL_REQUEST_COMPARATOR);
  }
}


Messaging::Ordering TIDorb::core::poa::POAManagerImpl::get_queue_order()
{
  return m_queue_order;
}
