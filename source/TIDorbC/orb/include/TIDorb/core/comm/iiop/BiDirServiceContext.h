/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/BiDirServiceContext.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_BIDIRSERVICECONTEXT_H_
#define _TIDORB_CORE_COMM_IIOP_BIDIRSERVICECONTEXT_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class BiDirServiceContext : public ServiceContext {

public:
	
	VectorListenPoint listen_points;
	
	void partial_read(TIDorb::core::cdr::CDRInputStream& input);
	
	void write(TIDorb::core::cdr::CDROutputStream& output) const;
	
	BiDirServiceContext(CORBA::ULong size) throw (TIDThr::SystemException)
	 : ServiceContext (BI_DIR_IIOP) {};
	
	BiDirServiceContext() throw (TIDThr::SystemException)
	 : ServiceContext(BI_DIR_IIOP) {};
	
	~BiDirServiceContext() throw (TIDThr::SystemException) {
	};
	
};
} //iiop	
} //comm
} //core
} //TIDorb
#endif

