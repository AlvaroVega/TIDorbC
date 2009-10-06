/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ObjectKey.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_OBJECTKEY_H_
#define _TIDORB_CORE_COMM_IIOP_OBJECTKEY_H_

#include "CORBA.h"
#include "TIDorb/core/poa.h"
#include "TIDorb/core/cdr.h"

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class ObjectKey: public TIDThr::RecursiveMutex {
private:
	  
	TIDorb::core::poa::POAKey_ref _key;
  
  	TIDorb::core::cdr::CDRInputStream* _marshaled_key;
        
        char* _url;
	
public:
	ObjectKey() throw (TIDThr::SystemException);

	ObjectKey(TIDorb::core::cdr::CDRInputStream* key)  throw (TIDThr::SystemException);
  
	ObjectKey(TIDorb::core::poa::POAKey* key)  throw (TIDThr::SystemException);
	
	bool operator==(const TIDorb::core::comm::iiop::ObjectKey& other) const;
	
	void read(TIDorb::core::cdr::CDRInputStream& input);
	 	
	void write(TIDorb::core::cdr::CDROutputStream& output) const;
	
	const TIDorb::core::cdr::CDRInputStream* getMarshaledKey() const;
	
	TIDorb::core::poa::POAKey* get_key() const;

	char* get_url();
	
	virtual ~ObjectKey() throw (TIDThr::SystemException);
	
};
}// iiop
}// comm
}// core
}// TIDorb
#endif

