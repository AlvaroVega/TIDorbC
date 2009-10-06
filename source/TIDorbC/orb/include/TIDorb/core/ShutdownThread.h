#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_SHUTDOWN_THREAD_H_
#define  _TIDORB_CORE_SHUTDOWN_THREAD_H_

/* -----------------------------------------------------------------------------

 File: TIDorbj/core/ShutdownThread.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */ 
 

namespace TIDorb {
namespace core {

class ShutdownThread: public virtual TIDThr::Thread {
  
 
public:

  ShutdownThread(TIDORB* orb) : m_orb(orb) {}

  
  void run();

protected:


  TIDORB* m_orb;
  
};

} // TIDorb
} // core

#endif
