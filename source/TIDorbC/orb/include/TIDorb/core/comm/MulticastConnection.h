/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/MulticastConnection.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_MulticastConnection_H_
#define _TIDORB_core_comm_MulticastConnection_H_ 1


/**
 * TIDorb multicast connection: sends and receives MIOP multicast messages.
 */

#include <sys/types.h>
#include "TIDSocket.h"

namespace TIDorb {
namespace core {
namespace comm {


class MulticastConnection : public UDPConnection
{
  protected:
    /**
     * Register the connection at the connection manager.
     */
    MulticastConnection(ConnectionManager* mngr, TIDSocket::MulticastSocket* s,
                        const char* _outgoing_interface)
      throw (TIDThr::SystemException, CORBA::SystemException);

  public:

    virtual ~MulticastConnection() throw (TIDThr::SystemException);

    const char* toString();

  public:
    /**
     * Creates a new connection in SERVER_MODE.
     */
    static Connection* server_connection(ConnectionManager* mngr, const TIDorb::core::comm::miop::ListenPoint& listen_point);

    /**
     * Creates a new connection in CLIENT_MODE.
     */
    static Connection* client_connection(ConnectionManager* mngr, const TIDorb::core::comm::miop::ListenPoint& listen_point);

};

} //comm
} //core
} //TIDorb

#endif
