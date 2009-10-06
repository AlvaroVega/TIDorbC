/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPRequestMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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
