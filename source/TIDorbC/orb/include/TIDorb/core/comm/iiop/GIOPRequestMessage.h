/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPRequestMessage.h

 Revisions:

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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPREQUESTMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPREQUESTMESSAGE_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPRequestMessage : public GIOPFragmentedMessage {
private:

  ServiceContextList* service_context_list;
  bool service_context_list_owner;
protected:

  void insert_request_header_1_0(const TIDorb::core::RequestImpl* request,
                                 const TIDorb::core::iop::IOR& ior);

  void insert_request_header_1_1(const TIDorb::core::RequestImpl* request,
                                       const TIDorb::core::iop::IOR& ior);
  
  void insert_request_header_1_2(const TIDorb::core::RequestImpl* request,
                                 const TIDorb::core::iop::IOR& ior,
                                 AddressingDisposition disposition);
  
  TIDorb::core::ServerRequestImpl* extract_request_1_0();
  
  TIDorb::core::ServerRequestImpl* extract_request_1_1();
  
  TIDorb::core::ServerRequestImpl* extract_request_1_2();
  
public:
  
  GIOPRequestMessage(const GIOPHeader& header);
  
  GIOPRequestMessage(const Version& ver);
  
  GIOPRequestMessage(const Version& ver,
                     RequestId id,
                     size_t fragment_size = 0);
  
  ~GIOPRequestMessage();
  
  void set_service_context_list(ServiceContextList* list, bool owner = false);
  
  const ServiceContextList* get_service_context_list() const;
  
  TIDorb::core::ServerRequestImpl* extract_request();
  
  void insert_request(const TIDorb::core::RequestImpl* request,
                      const TIDorb::core::iop::IOR& ior,
                      AddressingDisposition disposition);
  
  void insert_heartbeat_request(TIDorb::core::TIDORB* orb,
                                RequestId id);

  /**
   * Get the Request Invocation policies sended by the Client. See QoS. 
   * @return 
   */        
  TIDorb::core::PolicyContext* getRequestInvocationPolicies();
  
  void* _impl();
  const char* _typeid();
  
  
};
} //iiop
} //comm
} //core
} //TIDorb
#endif
