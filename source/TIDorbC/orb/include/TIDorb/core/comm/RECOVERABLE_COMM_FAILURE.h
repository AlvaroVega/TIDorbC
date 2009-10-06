/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/RECOVERABLE_COMM_FAILURE
  
 Revisions:
 
 Copyright 2004 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_RECOVERABLE_COMM_FAILURE_H_
#define _TIDORB_core_comm_RECOVERABLE_COMM_FAILURE_H_ 1

 
/**
 * Failure thrown in connection that can be recovered for resending requests.
 * 
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

#include <exception>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::exception;
#endif

namespace TIDorb {
namespace core {
namespace comm {

class RECOVERABLE_COMM_FAILURE : public virtual exception {
	
	public:
	 CORBA::COMM_FAILURE comm_failure;
	
  	 RECOVERABLE_COMM_FAILURE(const CORBA::COMM_FAILURE& failure) throw();
	 ~RECOVERABLE_COMM_FAILURE() throw() {};

	 const char* what() const throw();
};

}
}
}
#endif

