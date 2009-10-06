/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/HeartbeatThread.h
  
 Revisions:
 
 Copyright 2004 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_HeartbeatThread_H_
#define _TIDORB_core_comm_HeartbeatThread_H_ 1

 
namespace TIDorb {
namespace core {
namespace comm {

class HeartbeatThread : public TIDThr::Thread
{
    ConnectionManager* conn_mngr;
    CORBA::ULong       interval_millis;
    CORBA::ULong       timeout_millis;

  public:
    HeartbeatThread(ConnectionManager* mngr,
                    TIDThr::ThreadGroup* group,
                    ::TimeBase::TimeT heartbeat_interval,
                    ::TimeBase::TimeT heartbeat_timeout)
      throw (TIDThr::SystemException);

    void run();
};

}
}
}

#endif
