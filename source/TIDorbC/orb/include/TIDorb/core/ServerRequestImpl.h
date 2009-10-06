//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServerRequestImpl.h
// Description: ServerRequest implementation.
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

#ifndef _TIDORB_CORE_SERVER_REQUEST_IMPL_H_
#define _TIDORB_CORE_SERVER_REQUEST_IMPL_H_

//MCPG
#include "TIDorb/core/comm/iiop.h"


namespace TIDorb {
namespace core {

class ServerRequestImpl : public virtual CORBA::ServerRequest
{
  public:
    ServerRequestImpl(const TIDorb::core::comm::iiop::Version& ver,
                      TIDorb::core::comm::iiop::RequestId id,
                      char* operation, // consume
                      TIDorb::core::cdr::CDRInputStream* cdr_parameters,
                      TIDorb::core::comm::iiop::TargetAddress* target, // consume
                      Compression::CompressorIdLevel compressor_id = 
                      Compression::CompressorIdLevel(0,0) );

    ServerRequestImpl(const ServerRequestImpl&  other);

    //PRA
    virtual ~ServerRequestImpl();
    //EPRA

    const char* operation() const;
    void arguments(CORBA::NVList_ptr& parameters);
    CORBA::Context_ptr ctx();
    void set_result(const CORBA::Any& result);
    void set_exception(const CORBA::Any& except);

// TIDorb operations

    void set_system_exception(const CORBA::SystemException& except);
    void set_forward(CORBA::Object_ptr obj);

    const TIDorb::core::comm::iiop::TargetAddress* get_target() const;
    TIDorb::core::comm::iiop::RequestId get_id() const;
    void set_id(TIDorb::core::comm::iiop::RequestId id);
    void with_response(bool value);
    bool with_response() const;
    ServerRequestStatus::ReturnStatus get_return_status() const;
    const CORBA::Any& get_result() const;
    const CORBA::Any& get_exception() const;
    const CORBA::SystemException& get_system_exception() const;
    CORBA::NVList_ptr get_parameters() const;
    CORBA::Object_ptr get_forward() const;
    const TIDorb::core::comm::iiop::Version& getVersion() const;
    Compression::CompressorIdLevel get_compressor() const;
    void set_compressor(Compression::CompressorIdLevel compressor);
    
  protected:

    ServerRequestStatus::CompletionStatus    m_completion_status;
    ServerRequestStatus::ReturnStatus        m_return_status;
    TIDorb::core::comm::iiop::RequestId      m_request_id;
    bool                                     m_with_response; // = false;
    char*                                    m_operation_name;
    TIDorb::core::comm::iiop::TargetAddress* m_target;
    TIDorb::core::comm::iiop::Version        m_version;
    ContextImpl*                             m_context;
    NVListImpl*                              m_server_parameters;
    CORBA::Any                               m_result;
    CORBA::Any                               m_exception;
    CORBA::SystemException*                  m_system_exception;
    CORBA::Object_ptr                        m_forward_obj;
    TIDorb::core::cdr::CDRInputStream*       m_marshaled_parameters;
    Compression::CompressorIdLevel           m_compressor;

};

} // end core
} // end TIDorb

#endif

