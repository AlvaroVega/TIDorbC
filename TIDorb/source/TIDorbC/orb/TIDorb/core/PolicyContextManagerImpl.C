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

#include "TIDorb/core/PolicyContextManagerImpl.h"


namespace TIDorb {
namespace core {


PolicyContextManagerImpl::PolicyContextManagerImpl(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
{
  m_orb = orb;
  
  CLEANUP_ROUND = 100;
  m_cleanup_round = 0;
}
  
PolicyContextManagerImpl::~PolicyContextManagerImpl()
  throw (TIDThr::SystemException)
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
    TIDThr::Synchronized sync(*this);

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
  if (m_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "PolicyContextManagerImp clearing_context()" << flush;
    m_orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  ThreadContextMapT::iterator it = m_thread_contexts.begin();
  ThreadContextMapT::iterator end = m_thread_contexts.end();

  while (it != end){
    // Check if already was removed
    if ((*it).first == NULL) {
      m_thread_contexts.erase(it);
      it = m_thread_contexts.begin();
    }
    else {
      if (!(*it).first->isAlive()){
        m_thread_contexts.erase(it);
        it = m_thread_contexts.begin();
      }
      else
        it++;
    }
  }
}


} //core
} //TIDorb
