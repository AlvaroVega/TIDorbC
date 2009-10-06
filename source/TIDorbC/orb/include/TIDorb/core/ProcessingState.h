#include "TIDorb/core.h"

#ifndef _TIDorb_core_PROCESSING_STATE_H_
#define _TIDorb_core_PROCESSING_STATE_H_


/* -----------------------------------------------------------------------------

 File: TIDorb/core/ProcessingState.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDThr.h"

namespace TIDorb {
namespace core {


class ProcessingState : public virtual TIDThr::Monitor {
 
public:

  enum ProcessingStatus {CLIENT = 0, RUNNING, SHUTDOWNING, SHUTDOWNED};

//MLG  
  ProcessingState (TIDORB* orb) throw (TIDThr::SystemException) 
    : m_state(CLIENT), m_orb(orb)  {}
  ~ProcessingState() throw (TIDThr::SystemException) {}  
//EMLG
  
  ProcessingStatus state() {
    return m_state;
  }

  void running();
  
  bool is_shutdowned();
  
  void shutdown();
  
  void shutdowned();
  
  void wait_for_shutdown();


protected:
  
  ProcessingStatus m_state;
  
  TIDORB* m_orb;  
  
};

} // TIDorb
} // core


#endif
