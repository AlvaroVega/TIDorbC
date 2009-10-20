/////////////////////////////////////////////////////////////////////////
//
// File:        RequestQueue.C
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

#include "TIDorb/core/poa/RequestQueue.h"

TIDorb::core::poa::RequestQueue::RequestQueue(
                              TIDorb::core::poa::POAManagerImpl* poaManager,
                              TIDorb::core::poa::QueueReaderManager* reader,
                              TIDorb::core::poa::RequestComparatorType type) 
   throw (TIDThr::SystemException)
{

  _reader = reader;
  _poaManager = poaManager;
  _poaManager->_add_ref();
  _deactivation = false;
  
  //jagd
  _maxAvailableRequests = (_poaManager->conf->getMaxQueuedRequests() +
                           _poaManager->conf->getMaxThreads());
  _starving_time = _poaManager->conf->getStarvingTime();

  _comparator_type = type;

  _temporal_value = NULL;
  _priority_value = NULL;
  _deadline_value = NULL;
  _value = NULL;
  
  switch (_comparator_type){
  case PRIORITY_REQUEST_COMPARATOR:
    _priority_value = new priority_queue<TIDorb::core::poa::QueuedRequest*, 
                                         deque<TIDorb::core::poa::QueuedRequest*>, 
                                         PriorityRequestComparator<QueuedRequest*> >;
    break;
  case DEADLINE_REQUEST_COMPARATOR:
    _deadline_value = new priority_queue<TIDorb::core::poa::QueuedRequest*, 
                                         deque<TIDorb::core::poa::QueuedRequest*>, 
                                         DeadlineRequestComparator<QueuedRequest*> >;
    break; 
  case TEMPORAL_REQUEST_COMPARATOR:
    _temporal_value = new priority_queue<TIDorb::core::poa::QueuedRequest*, 
                                         deque<TIDorb::core::poa::QueuedRequest*>, 
                                         TemporalRequestComparator<QueuedRequest*> >;
    break;
  default:
    _value = new simple_queueT;
    _comparator_type = TIDorb::core::poa::NULL_REQUEST_COMPARATOR;
    break;
  }


};

TIDorb::core::poa::RequestQueue::~RequestQueue()  throw (TIDThr::SystemException)
{
  _poaManager->_remove_ref();
  delete _reader;
};

void TIDorb::core::poa::RequestQueue::add(TIDorb::core::poa::QueuedRequest* request)
{
  TIDThr::Synchronized synchro(*this);

  if ( (_deactivation) ||
       // jagd (this->size() >= (_poaManager->conf->getMaxQueuedRequests()
       (this->size() >= (_maxAvailableRequests
                         //jagd + _poaManager->conf->getMaxThreads()
                         - _poaManager->getThreadPool()->getActives())) ) {
    
    request->returnError(CORBA::TRANSIENT(NULL, 1, CORBA::COMPLETED_NO),
                         _poaManager->orb->trace);

    // Destroy actual request because cannot be inserted in queue request
    request->destroy();      

    return;
  }

  switch (_comparator_type){
  case PRIORITY_REQUEST_COMPARATOR:
    _priority_value->push(request);
    break;
  case DEADLINE_REQUEST_COMPARATOR:
    _deadline_value->push(request);
    break; 
  case TEMPORAL_REQUEST_COMPARATOR:
    _temporal_value->push(request);
    break;
  default:
    _value->push_back(request);
    break;
  }

  if (!_reader->createNewReader()) {
    notify();
  }
};

TIDorb::core::poa::QueuedRequest* TIDorb::core::poa::RequestQueue::get()
{
  TIDThr::Synchronized synchro(*this);

  if (this->size() == 0) {

    if(_deactivation) {
      return NULL;
    }
    try {
      //jagd wait(_poaManager->conf->getStarvingTime());
      wait(_starving_time);
    } catch(TIDThr::InterruptedException &ie) {}

    if (this->size() == 0) {
      return NULL;
    }
  }

  QueuedRequest* req;

  switch (_comparator_type){
  case PRIORITY_REQUEST_COMPARATOR:
    req = _priority_value->top();
    _priority_value->pop();
    break;
  case DEADLINE_REQUEST_COMPARATOR:
    req = _deadline_value->top();
    _deadline_value->pop();
    break; 
  case TEMPORAL_REQUEST_COMPARATOR:
    req = _temporal_value->top();
    _temporal_value->pop();
    break;
  default:
    req = _value->front();
    _value->pop_front();
    break;
  }

  return req;
};

CORBA::ULong TIDorb::core::poa::RequestQueue::size() 
{
  //TIDThr::Synchronized synchro(*this); // Always called from a syncronized block
  
  CORBA::ULong size = 0;

  switch (_comparator_type){
  case PRIORITY_REQUEST_COMPARATOR:
    size = _priority_value->size();
    break;
  case DEADLINE_REQUEST_COMPARATOR:
    size = _deadline_value->size();
    break; 
  case TEMPORAL_REQUEST_COMPARATOR:
    size = _temporal_value->size();
    break;
  default:
    size = _value->size();
    break;
  }

  return size;
};

void TIDorb::core::poa::RequestQueue::discardAll() 
{
  // TIDThr::Synchronized synchro(*this); // Always called from a syncronized block

  QueuedRequest* req;
 
  switch (_comparator_type){
  case PRIORITY_REQUEST_COMPARATOR:
    while (!_priority_value->empty()){  
      req = _priority_value->top();
      req->setMustDiscard(true);
      _priority_value->pop();
    }
    break;
  case DEADLINE_REQUEST_COMPARATOR:
    while (!_deadline_value->empty()){  
      req = _deadline_value->top();
      req->setMustDiscard(true);
      _deadline_value->pop();
    }
    break; 
  case TEMPORAL_REQUEST_COMPARATOR:
    while (!_temporal_value->empty()){  
      req = _temporal_value->top();
      req->setMustDiscard(true);
      _temporal_value->pop();
    }
    break;
  default:
    while (!_value->empty()){
      req = _value->front();
      req->setMustDiscard(true);
      _value->pop_front();
    }
    break;
  }

};

void TIDorb::core::poa::RequestQueue::deactivation()
{
  TIDThr::Synchronized synchro(*this);

  if(!_deactivation) {
    _deactivation = true;
    notifyAll();
  }
};


void TIDorb::core::poa::RequestQueue::setComparator(
                                  TIDorb::core::poa::RequestComparatorType type)
{

  if ( _comparator_type != type){

    TIDThr::Synchronized synchro(*this);

    this->discardAll();

    switch (_comparator_type){
    case PRIORITY_REQUEST_COMPARATOR:
      delete _priority_value;
      break;
    case DEADLINE_REQUEST_COMPARATOR:
      delete _deadline_value;
      break; 
    case TEMPORAL_REQUEST_COMPARATOR:
      delete _temporal_value;
      break;
    default:
      delete _value;
      break;
    } 

    switch (type){
    case PRIORITY_REQUEST_COMPARATOR:
      _priority_value = new priority_queue<TIDorb::core::poa::QueuedRequest*, 
                                           deque<TIDorb::core::poa::QueuedRequest*>, 
                                           PriorityRequestComparator<QueuedRequest*> >;
      break;
    case DEADLINE_REQUEST_COMPARATOR:
      _deadline_value = new priority_queue<TIDorb::core::poa::QueuedRequest*, 
                                           deque<TIDorb::core::poa::QueuedRequest*>, 
                                           DeadlineRequestComparator<QueuedRequest*> >;
      break; 
    case TEMPORAL_REQUEST_COMPARATOR:
      _temporal_value = new priority_queue<TIDorb::core::poa::QueuedRequest*, 
                                           deque<TIDorb::core::poa::QueuedRequest*>, 
                                           TemporalRequestComparator<QueuedRequest*> >;
      break;
    default:
      _value = new simple_queueT;
      type = TIDorb::core::poa::NULL_REQUEST_COMPARATOR;
      break;
    }
    _comparator_type = type;
  }

};
