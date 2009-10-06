//////////////////////////////////////////////////////////////////////////////////
//
// File:        MIOPRequest.h
// Description: MIOPRequest
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

#ifndef _TIDORB_CORE_GOA_MIOPRequest_H_
#define _TIDORB_CORE_GOA_MIOPRequest_H_


namespace TIDorb {
namespace core {
namespace goa {


class MIOPRequest : public TIDorb::core::goa::AbstractMIOPRequest {

protected:
        TIDorb::core::ServerRequestImpl* request;
        char* miop_name;

public:
        MIOPRequest(TIDorb::core::poa::POAKey* poakey,
                    TIDorb::core::poa::POAImpl* root_poa,
                    TIDorb::core::comm::Connection* conn,
                    TIDorb::core::ServerRequestImpl* req);

        ~MIOPRequest();

        TIDorb::core::ServerRequestImpl* get_request() const;

        const char* toString() const;

        void returnError(const CORBA::SystemException& e, TIDorb::util::Trace* trace);

//      void destroy(); //Este metodo sobra al tener el destructor

        void dispatch(TIDorb::core::poa::ExecThread* exec, TIDorb::core::poa::POAImpl* poa);

// 	virtual void* _impl();
// 	virtual const char* _typeid();

};

} //goa
} //core
} //TIDorb
#endif
