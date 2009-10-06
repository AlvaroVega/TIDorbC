/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/ReliableOnewayThread.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ReliableOnewayThread_H_
#define _TIDORB_core_comm_ReliableOnewayThread_H_ 1

 
namespace TIDorb {
	
namespace core {
	
namespace comm {
	
class ReliableOnewayThread : public TIDThr::Thread
{
	private:
	
	 IIOPCommLayer* comm;
	 TIDorb::core::RequestImpl* request;
	 TIDorb::core::iop::IOR_ref ior;
  
	public:
	 ReliableOnewayThread (IIOPCommLayer* commi, 
	                       TIDorb::core::RequestImpl* req, 
			       TIDorb::core::iop::IOR* iori, 
			       TIDThr::ThreadGroup* group)
           throw (TIDThr::SystemException);

	 void run();
  
};

}

}

}
#endif

