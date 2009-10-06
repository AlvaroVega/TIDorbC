/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractIIOPRequest.h
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

#ifndef _TIDORB_CORE_POA_AbstractIIOPRequest_H_
#define _TIDORB_CORE_POA_AbstractIIOPRequest_H_


//PRA
#include "TIDorb/core/comm/ConnectionState.h"
#include "TIDorb/core/comm/Connection.h"
//EPRA


namespace TIDorb {
namespace core {
namespace poa {


class AbstractIIOPRequest : public TIDorb::core::poa::QueuedRequest
{
  public:
        TIDorb::core::comm::Connection_ref connection;

        AbstractIIOPRequest(TIDorb::core::poa::POAKey* poakey,
                            TIDorb::core::poa::POAImpl* root_poa,
                            TIDorb::core::comm::Connection* conn);

        ~AbstractIIOPRequest();

        TIDorb::core::comm::Connection* get_connection() const;
// //MLG
// 	virtual void* _impl();
// 	virtual const char* _typeid();
// //EMLG	    
};


} //poa
} //core
} //TIDorb


#endif
