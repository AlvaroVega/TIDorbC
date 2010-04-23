//////////////////////////////////////////////////////////////////////////////////
//
// File:        SSLIOPCorbaloc.h
// Description: 
//
// Rel:         01.00
// Created:     Nov, 2009
// Author:      
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////


#include "TIDorb/core/comm/ssliop.h"


#ifndef _TIDORB_CORE_COMM_SSLIOP_SSLIOPCORBALOC_H_
#define _TIDORB_CORE_COMM_SSLIOP_SSLIOPCORBALOC_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace ssliop {

class SSLIOPCorbaloc 
{
	
public:
  static char* get_object_id(const char* corbaloc_URL) 
    throw(CORBA::ORB::InvalidName);
  	  
  static TIDorb::core::iop::IOR* get_IOR(const char* corbaloc_URL) 
    throw(CORBA::ORB::InvalidName);
	  
private:
  
  static TIDorb::core::comm::iiop::ObjectKey* create_key(const char* url) 
    throw(CORBA::ORB::InvalidName);
  	  
};

}
}
}
}

#endif
