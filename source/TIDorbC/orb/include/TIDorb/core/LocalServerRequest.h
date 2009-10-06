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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
