/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyContextManagerImpl.C
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

#include "TIDorb/core/PolicyContextManagerImpl.h"


namespace TIDorb {
namespace core {


PolicyContextManagerImpl::PolicyContextManagerImpl(TIDorb::core::TIDORB* orb)
{
  m_orb = orb;
  
  CLEANUP_ROUND = 100;
  m_cleanup_round = 0;
}
  
PolicyContextManagerImpl::~PolicyContextManagerImpl()
{
  ThreadContextMapT::iterator it = m_thread_contexts.begin();
  ThreadContextMapT::iterator end = m_thread_contexts.end();
  
  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "Destroying PolicyContextManagerImpl() " << flush;
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }  

  while (it != end){
    // delete (*it).first;
    delete (*it).second;
    it++;
  }
  m_thread_contexts.clear();
}

TIDorb::core::PolicyContext* PolicyContextManagerImpl::getThreadContext(TIDThr::Thread* th)
{
  ThreadContextMapT::iterator it =  m_thread_contexts.find(th);
  TIDorb::core::PolicyContext* context = NULL;
  
  if (it == m_thread_contexts.end()) {
    // Thread PolicyContext overrides ORB policies (that is has 
    // the ORB PolicyContext as father
    TIDorb::core::ConfORB& conf = const_cast<TIDorb::core::ConfORB&>(m_orb->conf());
    context = new PolicyContext(conf.getPolicyContext());
    m_thread_contexts[th] = context;
    
    // Start cleaner thread
    if ((++m_cleanup_round) > CLEANUP_ROUND) {
      m_cleanup_round = 0;
      clear_contexts();
    }
  }
  else{
    context = (*it).second;
  }
  
  return context;
}
  
TIDorb::core::PolicyContext* PolicyContextManagerImpl::tryToGetThreadContext(TIDThr::Thread* th)
{
  return m_thread_contexts[th];
}

void PolicyContextManagerImpl::clear_contexts()
{
  ThreadContextMapT::iterator it = m_thread_contexts.begin();
  ThreadContextMapT::iterator end = m_thread_contexts.end();

  while (it != end){
    if (!(*it).first->isAlive()){
      m_thread_contexts.erase(it);
      it = m_thread_contexts.begin();
    }
    else
      it++;
  }
}


} //core
} //TIDorb
