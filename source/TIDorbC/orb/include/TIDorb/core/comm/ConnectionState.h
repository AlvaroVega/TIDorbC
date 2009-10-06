#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ConnectionState_H_
#define _TIDORB_core_comm_ConnectionState_H_ 1

/* -----------------------------------------------------------------------------

 File: es.tid.TIDorbj.core.comm.ConnectionState.java

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

/**
 * TIDorb IIOP Connection State.
 *
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 0.1
 */

namespace TIDorb {

namespace core {

namespace comm {

class ConnectionState: public TIDThr::Mutex
 {

  public:
  /**
   *  Open connection state.
   */

   static const short OPEN_STATE = 0;

  /**
   *  Open connection state.
   */
   static const short CLOSING_STATE = 1;

  /**
   *  Open connection state.
   */
  static const short ERROR_STATE = 2;


  /**
   *  Connection state.
   */
  private:
   short state;

    protected:
    /**
    * Connection error that unhabilitates the connection.
    */
   CORBA::COMM_FAILURE* connection_error;

  public:
   ConnectionState() throw (TIDThr::SystemException);
//MLG
   ~ConnectionState() throw (TIDThr::SystemException);
//EMLG
   
   bool set_closing();
   bool is_open();
   bool set_error(const CORBA::COMM_FAILURE& the_error);
   short get_value();
   const CORBA::COMM_FAILURE* get_error();
   
   /**
    * Verify if the reply can be sent.
    */
   void verify_reply();
   
  /**
     * Verify if the request can be sent.
     */
  void verify_request() throw(RECOVERABLE_COMM_FAILURE);
};

}

}

}
#endif

