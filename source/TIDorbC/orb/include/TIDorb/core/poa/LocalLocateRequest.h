/////////////////////////////////////////////////////////////////////////
//
// File:        LocalLocateRequest.h
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

#ifndef _TIDORB_CORE_POA_LocalLocateRequest_H_
#define _TIDORB_CORE_POA_LocalLocateRequest_H_


namespace TIDorb {
namespace core {
namespace poa {


class LocalLocateRequest;
typedef TIDThr::HandleT<LocalLocateRequest> LocalLocateRequest_ref; /* reference counting */


class LocalLocateRequest : public TIDorb::core::poa::AbstractLocalRequest
{
    private:
        TIDorb::core::util::OperationCompletion state;

        const char* _locate_name;

        CORBA::SystemException* _exception;

        CORBA::Object_ptr forward_obj;

    public:
        bool found;

        LocalLocateRequest(TIDorb::core::poa::POAKey* poakey, 
                           TIDorb::core::poa::POAImpl* root_poa);

        ~LocalLocateRequest() throw(TIDThr::SystemException);

        CORBA::SystemException* exception();

        void exception(const CORBA::SystemException& excp);

        bool has_exception();

        bool is_completed();

        void set_completed();

        void wait_for_completion(CORBA::ULong how_long);

        bool is_forwarded();

        void set_forward(CORBA::Object_ptr obj);

        CORBA::Object_ptr get_forward();

        const char* toString() const;

        void returnError(const CORBA::SystemException& e, TIDorb::util::Trace* trace);

        void dispatch (ExecThread * exec,POAImpl  * poa);
 
// 	void* _impl();
// 	const char* _typeid();
};


} //poa
} //core
} //TIDorb


#endif
