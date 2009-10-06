/////////////////////////////////////////////////////////////////////////
//
// File:        POAManagerConfListener.h
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

#ifndef _TIDORB_CORE_POA_POAManagerConfListener_H_
#define _TIDORB_CORE_POA_POAManagerConfListener_H_

namespace TIDorb {
namespace core {
namespace poa {

class POAManagerConfListener {
public:  
	virtual void minThreadsHasChanged()=0; 
	virtual void maxThreadsHasChanged()=0; 
	virtual ~POAManagerConfListener() {}
  
};

} //poa
} //core
} //TIDorb
#endif
