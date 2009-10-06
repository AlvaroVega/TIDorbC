#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ConnectionThread_H_
#define _TIDORB_core_comm_ConnectionThread_H_ 1

 
/**
 * Thread that receives messages in the connection.
 * 
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @author Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace comm {

class ConnectionThread : public TIDThr::Thread
{
    Connection_ref connection;

  public:
    ConnectionThread (Connection* conn, TIDThr::ThreadGroup* group)
      throw (TIDThr::SystemException);
    
    virtual ~ConnectionThread() throw (TIDThr::SystemException) {}

    void run();
};

}
}
}

#endif
