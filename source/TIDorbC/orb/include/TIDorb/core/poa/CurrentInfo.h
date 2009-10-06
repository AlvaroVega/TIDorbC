/////////////////////////////////////////////////////////////////////////
//
// File:        CurrentInfo.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_CurrentInfo_H_
#define _TIDORB_CORE_POA_CurrentInfo_H_

namespace TIDorb {
namespace core {
namespace poa {

class CurrentInfo {
protected:
	
  	bool _request_in_poa;
	
public: 
	TIDorb::core::poa::POAImpl* _current_poa;
	TIDorb::core::poa::OID _current_oid;
	PortableServer::ServantBase* _servant;
	
	CurrentInfo(TIDorb::core::poa::POAImpl* current_poa, const TIDorb::core::poa::OID& current_oid);
	~CurrentInfo();
	
 	bool is_request_in_POA();
  
  	void set_request_in_POA(bool val);
  
  	void setServant(PortableServer::ServantBase* servant);
    
};

} //poa
} //core
} //TIDorb
#endif
