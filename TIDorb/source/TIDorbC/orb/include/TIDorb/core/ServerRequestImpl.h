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

#ifndef _TIDORB_CORE_SERVER_REQUEST_IMPL_H_
#define _TIDORB_CORE_SERVER_REQUEST_IMPL_H_


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

    virtual ~ServerRequestImpl();

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
    CSI::SASContextBody_ptr get_sas_context_body() const;
    void set_sas_context_body(CSI::SASContextBody_ptr sas_context);
    
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
    CSI::SASContextBody_ptr                  m_sas_context;

};

} // end core
} // end TIDorb

#endif

