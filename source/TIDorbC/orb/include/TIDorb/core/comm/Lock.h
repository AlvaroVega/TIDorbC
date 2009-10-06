/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/Lock.C
  
 Revisions:
    25/10/2005 caceres@tid.es Performance improvements
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_Lock_H_
#define _TIDORB_core_comm_Lock_H_ 1


/**
 * Lock where the the threads that have invoked a request must be locket until the complete
 * response has been replied or the response timeout has expired.
 *
 * 
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb {

namespace core {
	
namespace comm {


class Lock : public virtual TIDorb::core::util::OperationCompletion {
   
   private:
	
	/**
	 * Request identifier. This identifier will be the key for the connection thread to 
	 */
	
	TIDorb::core::comm::iiop::RequestId _request_id;
	TIDorb::core::comm::iiop::GIOPFragmentedMessage* message;
	
	public:
    
	 Lock() throw (TIDThr::SystemException);
	 ~Lock() throw (TIDThr::SystemException);
	 
	 TIDorb::core::comm::iiop::RequestId request_id()
     {
        return _request_id;
     }
     
	 void request_id(TIDorb::core::comm::iiop::RequestId id)
     {
        _request_id = id;
     }
     	 
	 void put_reply(TIDorb::core::comm::iiop::GIOPFragmentedMessage* msg);
     
     /**
      * Gets and consume the message, a new consume_message will return NULL
      */
	 TIDorb::core::comm::iiop::GIOPFragmentedMessage* consume_reply();
     
     void clear();
};

}

}

}
#endif

