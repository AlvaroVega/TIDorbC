//////////////////////////////////////////////////////////////////////////////////
//
// File:        PolicyContextManagerImpl.h
// Description: PolicyContextManager implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef __TIDorb_core_PolicyContextManagerImpl_H_
#define __TIDorb_core_PolicyContextManagerImpl_H_

#include "CORBA.h"

#include <map>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif


namespace TIDorb {
namespace core {

class PolicyContextManagerImpl 
{
protected:
  TIDorb::core::TIDORB* m_orb;
  
  int CLEANUP_ROUND;

  // typedef map<TIDThr::Thread*, TIDorb::core::PolicyContext*> ThreadContextMapT;
  typedef map<TIDThr::ThreadHandle, TIDorb::core::PolicyContext*> ThreadContextMapT;
  ThreadContextMapT m_thread_contexts;   
  
  int m_cleanup_round;


public:
 
  PolicyContextManagerImpl(TIDorb::core::TIDORB* orb);

  ~PolicyContextManagerImpl();

  TIDorb::core::PolicyContext* getThreadContext(TIDThr::Thread* th);

  TIDorb::core::PolicyContext* tryToGetThreadContext(TIDThr::Thread* th);

  void clear_contexts();

};

}
}

#endif

