/* -----------------------------------------------------------------------------

File: TIDorb/core/comm/ServerListener.h

Revisions:

Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
The copyright to the software program(s) is property of Telefonica I+D.
The program(s) may be used and or copied only with the express written
consent of Telefonica I+D or in acordance with the terms and conditions
stipulated in the agreement/contract under which the program(s) have
been supplied.
------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ServerListener_H_
#define _TIDORB_core_comm_ServerListener_H_ 1


#include "TIDSocket.h"


namespace TIDorb {
namespace core {
namespace comm {

  class ServerListener : public TIDThr::Thread
  {
  private:
    TIDSocket::ServerSocket* server_socket;
    ConnectionManager_ref manager;

    bool do_shutdown;

    /**
     * Listen point where the ServerSocket will be listening. This point(host,port)
     * will be used to create de local IOR's.
     */
    TIDorb::core::comm::iiop::ListenPoint listen_point;

    TIDorb::core::TIDORB_ref _orb;

    const TIDorb::core::ConfORB& conf;

  public:

    ServerListener(ConnectionManager* the_manager, TIDThr::ThreadGroup* group)
      throw (TIDThr::SystemException,CORBA::SystemException);

    ~ServerListener() throw (TIDThr::SystemException);
        
    void run();
        
    void shutdown();
        
    const TIDorb::core::comm::iiop::ListenPoint& get_listen_point();
    TIDorb::core::iop::IOR* createIOR(const char* id, TIDorb::core::poa::POAKey* key);

        
  protected:

    void reset_server_socket();
  };

}
}
}

#endif


