/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ServiceContext.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDorb_core_comm_iiop_SERVICECONTEXT_H_
#define _TIDorb_core_comm_iiop_SERVICECONTEXT_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class ServiceContext: public virtual TIDThr::RefCounter {
public:
	
	CORBA::ULong _context_id;
	
  TIDorb::core::cdr::CDRInputStream* _context_data;
  
	ServiceContext(CORBA::ULong context_id) throw (TIDThr::SystemException);
	
	ServiceContext(CORBA::ULong context_id, TIDorb::core::cdr::CDRInputStream* context_data)
	 throw (TIDThr::SystemException);
	
	virtual void write(TIDorb::core::cdr::CDROutputStream& output) const;
		
	virtual void partial_read(TIDorb::core::cdr::CDRInputStream& input);
	
	static void skip(TIDorb::core::cdr::CDRInputStream& input);
	
	static void partial_skip(TIDorb::core::cdr::CDRInputStream& input);
	
	~ServiceContext()  throw (TIDThr::SystemException);
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
