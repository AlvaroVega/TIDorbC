/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ServiceId.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_SERVICEID_H_
#define _TIDORB_CORE_COMM_IIOP_SERVICEID_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {


class ServiceId
{
public:
	static const CORBA::ULong TransactionService;
	static const CORBA::ULong CodeSets;
	static const CORBA::ULong ChainBypassCheck;
	static const CORBA::ULong ChainBypassInfo;
	static const CORBA::ULong LogicalThreadId;
	static const CORBA::ULong BI_DIR_IIOP;
	static const CORBA::ULong SendingContextRunTime;
	static const CORBA::ULong INVOCATION_POLICIES;
	static const CORBA::ULong FORWARDED_ENTITY;
	static const CORBA::ULong UnknownExceptionInfo;
};

} //iiop
} //comm
} //core
} //TIDorb

#endif
