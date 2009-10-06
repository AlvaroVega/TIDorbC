//////////////////////////////////////////////////////////////////////////////////
//
// File:        AbstractMIOPRequest.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_GOA_AbstractMIOPRequest_H_
#define _TIDORB_CORE_GOA_AbstractMIOPRequest_H_



#include "TIDorb/core/comm/ConnectionState.h"
#include "TIDorb/core/comm/Connection.h"


namespace TIDorb {
namespace core {
namespace goa {

class AbstractMIOPRequest : public TIDorb::core::poa::QueuedRequest {

public:
        TIDorb::core::comm::Connection_ref connection;

        AbstractMIOPRequest(TIDorb::core::poa::POAKey* poakey,
                            TIDorb::core::poa::POAImpl* root_poa,
                            TIDorb::core::comm::Connection* conn);
        ~AbstractMIOPRequest();

        TIDorb::core::comm::Connection* get_connection() const;

// 	virtual void* _impl();
// 	virtual const char* _typeid();

};

} //goa
} //core
} //TIDorb
#endif

