/////////////////////////////////////////////////////////////////////////
//
// File:        IIOPRequest.h
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


#ifndef _TIDORB_CORE_POA_IIOPRequest_H_
#define _TIDORB_CORE_POA_IIOPRequest_H_


namespace TIDorb {
namespace core {
namespace poa {


class IIOPRequest : public TIDorb::core::poa::AbstractIIOPRequest
{
    protected:
        TIDorb::core::ServerRequestImpl* request;
        char* iiop_name;

    public:
        IIOPRequest(TIDorb::core::poa::POAKey* poakey, 
                    TIDorb::core::poa::POAImpl* root_poa,
                    TIDorb::core::comm::Connection* conn, 
                    TIDorb::core::ServerRequestImpl* req);

        ~IIOPRequest();

        TIDorb::core::ServerRequestImpl* get_request() const;

        const char* toString() const;

        void returnError(const CORBA::SystemException& e, TIDorb::util::Trace* trace);

        void dispatch(ExecThread* exec, POAImpl* poa);

// 	virtual void* _impl();
// 	virtual const char* _typeid();


};


} //poa
} //core
} //TIDorb


#endif
