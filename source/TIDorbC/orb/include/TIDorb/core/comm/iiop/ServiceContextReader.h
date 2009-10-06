/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ServiceContextReader.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_SERVICECONTEXTREADER_H_
#define _TIDORB_CORE_COMM_IIOP_SERVICECONTEXTREADER_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {
 
class ServiceContextReader {
public:	
	
	static TIDorb::core::comm::iiop::ServiceContext* read(TIDorb::core::cdr::CDRInputStream& input);
	
};
} //iiop	
} //comm
} //core
} //TIDorb
#endif


