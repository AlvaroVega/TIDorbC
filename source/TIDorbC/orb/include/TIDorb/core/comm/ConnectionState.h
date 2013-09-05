#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ConnectionState_H_
#define _TIDORB_core_comm_ConnectionState_H_ 1

/* -----------------------------------------------------------------------------

 File: es.tid.TIDorbj.core.comm.ConnectionState.java

 Revisions:

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

   ~ConnectionState() throw (TIDThr::SystemException);

   
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

