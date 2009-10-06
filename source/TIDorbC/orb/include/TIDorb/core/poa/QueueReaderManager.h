/////////////////////////////////////////////////////////////////////////
//
// File:        QueueReaderManager.h
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

#ifndef _TIDORB_CORE_POA_QueueReaderManager_H_
#define _TIDORB_CORE_POA_QueueReaderManager_H_


namespace TIDorb {
namespace core {
namespace poa {
/**
 * Interface for queue readers that want to be notified of item availability.
 */

class QueueReaderManager {
public:  
	virtual bool createNewReader()=0;
	virtual ~QueueReaderManager() {}
};

} //poa
} //core
} //TIDorb
#endif
