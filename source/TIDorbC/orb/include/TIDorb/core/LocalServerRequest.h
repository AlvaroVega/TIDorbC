//////////////////////////////////////////////////////////////////////////////////
//
// File:        LocalServerRequest.h
// Description: LocalServerRequest implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_LOCAL_SERVER_REQUEST_H_
#define _TIDORB_CORE_LOCAL_SERVER_REQUEST_H_


namespace TIDorb {
namespace core {


class LocalServerRequest: public virtual CORBA::ServerRequest
{
  public:
    LocalServerRequest(RequestImpl* req);

    //PRA
    virtual ~LocalServerRequest();
    //EPRA

    const char* operation() const;
    void arguments(CORBA::NVList_ptr& parameters);
    CORBA::Context_ptr ctx();
    void set_result(const CORBA::Any& result);
    void set_exception(const CORBA::Any& except);

// TIDorb operations

    void fix_out_arguments();
    void set_system_exception(const CORBA::SystemException& except);
    void set_forward(CORBA::Object_ptr obj);
    CORBA::Object_ptr get_forward();
    bool is_forwarded();

  protected:
    ServerRequestStatus::CompletionStatus  m_completion_status;
    RequestImpl*                           m_local_request;
    NVListImpl*                            m_server_parameters;
    CORBA::Object_ptr                      m_forward_obj;
};

} // end core
} // end TIDorb


#endif
