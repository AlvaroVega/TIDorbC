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
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
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
