/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/LocateReplyStatusType.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_LOCATEREPLAYSTATUSTYPE_H_
#define _TIDORB_CORE_COMM_IIOP_LOCATEREPLAYSTATUSTYPE_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

enum LocateReplyStatusType { UNKNOWN_OBJECT = 0, OBJECT_HERE, OBJECT_FORWARD, OBJECT_FORWARD_PERM,
                              LOC_SYSTEM_EXCEPTION, LOC_NEEDS_ADDRESSING_MODE};


	
LocateReplyStatusType LocateReplyStatusType_from_int(CORBA::ULong type);
	
const char* toString(LocateReplyStatusType type);

}// iiop
}// comm
}// core
}// TIDorb
#endif
