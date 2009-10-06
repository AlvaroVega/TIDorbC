/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/OpeningLock.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_OpeningLock_H_
#define _TIDORB_core_comm_OpeningLock_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace comm {

class OpeningLock: public virtual TIDThr::Monitor {

  public:
    enum LockStatus{ST_OPENING = 0, ST_OPENED, ST_ERROR};

    OpeningLock() throw(TIDThr::SystemException);

    ~OpeningLock() throw(TIDThr::SystemException){};

    Connection* wait_opening(CORBA::ULongLong time) throw(CORBA::SystemException);

    void set_opened(TIDorb::core::comm::Connection* conn) throw();

    void set_error(const CORBA::COMM_FAILURE& failure) throw();


  private:
    LockStatus status;
    Connection* opened_connection;
    CORBA::COMM_FAILURE error;
};

}
}
}

#endif

