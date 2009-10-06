/////////////////////////////////////////////////////////////////////////
//
// File:        ThreadPool.C
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

#include "TIDorb/core/poa/ThreadPool.h"





TIDorb::core::poa::ThreadPool::ThreadPool(TIDorb::core::poa::POAManagerImpl* poaManager)
{
  _poaManager = poaManager;
  _poaManager->_add_ref();
  _used = 0;
  _actuallyUsed = 0;
  _active = 0;
  _deactivated = false;

  //jagd 
  _maxThreads=_poaManager->conf->getMaxThreads();
  _minThreads=_poaManager->conf->getMinThreads();

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, _poaManager->conf->getThreadStackSize());

  if (_poaManager->conf->getBoundThread())
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  else
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

  _group = (TIDThr::ThreadGroup*) new TIDThr::ThreadGroup(NULL, "", &attr);
  //PRA
  // it can be destroyed because ThreadGroup copies it inside the constructor
  pthread_attr_destroy(&attr);
  //EPRA
}




TIDorb::core::poa::ThreadPool::~ThreadPool()
{
  _poaManager->_remove_ref();
}




CORBA::Boolean TIDorb::core::poa::ThreadPool::createNewReader()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  //jagd
  //if ((_used < _poaManager->conf->getMaxThreads()) && (_actuallyUsed == _active)) {
  if ((_used < _maxThreads) && (_actuallyUsed == _active)) {
    createThread();
    return true;
  }

  return false;
}




void TIDorb::core::poa::ThreadPool::createThread()
{
  TIDThr::Synchronized synchro(recursive_mutex);

  try {
     TIDorb::core::poa::ExecThread_ref t =
       new TIDorb::core::poa::ExecThread(_poaManager,_group);

    //_pool->addElement(t);
    _pool.push_back(t);
    t->setThreadStateListener(this);
    t->setDaemon(false);
    t->start();
    _used++;

    if (_poaManager->orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "New thread in " << _poaManager->toString() << " created: "  
          << _used << " threads are now active"
          //jagd << " (max " << _poaManager->conf->getMaxThreads()
          << " (max " << _maxThreads 
          << ", min " << _minThreads
          << ")";
      _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }

  } catch(const TIDThr::Exception& e) {
    cerr << "ThreadPool::createThread: " << e.getName()
         << " Reason: " << e.getReason() << endl;
  }
}




void TIDorb::core::poa::ThreadPool::setActive(TIDThr::Thread* t, CORBA::Boolean firstTime)
{
  //jagd que mas da que no controle exactamente el numero de threads del pool
  // ojo que nunca me paso del numMaxThreads 
  //TIDThr::Synchronized synchro(recursive_mutex);

  _active++;
  if (firstTime) _actuallyUsed++;
}




void TIDorb::core::poa::ThreadPool::setInactive(TIDThr::Thread* t)
{
  //jagd que mas da que no controle exactamente el numero de threads del pool
  // ojo que nunca me paso del numMaxThreads 
  //TIDThr::Synchronized synchro(recursive_mutex);
  _active--;
}




void TIDorb::core::poa::ThreadPool::threadHasDied(TIDThr::Thread* t)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  TIDorb::core::poa::ExecThread_ref exec_thr = dynamic_cast<TIDorb::core::poa::ExecThread*>(t);
  _pool.remove(exec_thr);
  _used--;
  _actuallyUsed--;
  _active--;

  if (_poaManager->orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Finalization of " << exec_thr->toString() << ": "
        << _used << " threads are now active"
        //jagd << " (max " << _poaManager->conf->getMaxThreads()
        << " (max " << _maxThreads 
        << ", min " << _minThreads
        << ")";
    _poaManager->orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
}




void TIDorb::core::poa::ThreadPool::deactivation()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  _deactivated = true;
}




CORBA::Boolean TIDorb::core::poa::ThreadPool::threadCanDie(TIDThr::Thread* t)
{
  TIDThr::Synchronized synchro(recursive_mutex);

  if(_deactivated)
    return true;
  else
    return (_used > _minThreads);
}




void TIDorb::core::poa::ThreadPool::minThreadsHasChanged()
{
  TIDThr::Synchronized synchro(recursive_mutex);

  _minThreads = _poaManager->conf->getMinThreads();

  while (_used < _minThreads) {
    createThread();
  }
}


void TIDorb::core::poa::ThreadPool::maxThreadsHasChanged()
{

  TIDThr::Synchronized synchro(recursive_mutex);

  list<TIDorb::core::poa::ExecThread_ref>::iterator it =  _pool.begin();
  list<TIDorb::core::poa::ExecThread_ref>::iterator end =  _pool.end();

  _maxThreads=_poaManager->conf->getMaxThreads();

  while ( (_used > _maxThreads) && 
          (it != end) ) {
    TIDorb::core::poa::ExecThread_ref t;
    t = (*it);

    if (threadCanDie(t)) {
      threadHasDied(t);
      it  = _pool.begin();
      end = _pool.end();
    } else {
      it++;
    }
    
  }

}
