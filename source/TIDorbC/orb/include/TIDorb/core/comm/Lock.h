/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/Lock.C
  
 Revisions:
    25/10/2005 caceres@tid.es Performance improvements
 
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

