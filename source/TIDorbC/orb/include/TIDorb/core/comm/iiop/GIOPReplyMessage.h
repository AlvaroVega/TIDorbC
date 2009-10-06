/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPReplyMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPREPLYMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPREPLYMESSAGE_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPReplyMessage : public GIOPFragmentedMessage {
  friend class TIDorb::core::comm::ziop::ZIOPMessage;
private:

  TIDorb::core::comm::iiop::ReplyStatusType _reply_status;
  const TIDorb::core::comm::iiop::ServiceContextList* _service_context_list;

protected:
  void reset();

  void insert_arguments(TIDorb::core::TIDORB* orb,
                        TIDorb::core::ServerRequestImpl* request);

  void insert_system_exception(TIDorb::core::TIDORB* orb,
                               const CORBA::SystemException& exception);

  void insert_user_exception(TIDorb::core::TIDORB* orb,
                             const CORBA::Any& exception);

  void marshal_reply_header_1_1();

  void marshal_reply_header_1_2();

  void unmarshal_reply_header_1_1();

  void unmarshal_reply_header_1_2();


public:
  GIOPReplyMessage(const TIDorb::core::comm::iiop::Version& version);

  GIOPReplyMessage(const TIDorb::core::comm::iiop::GIOPHeader& header);

  GIOPReplyMessage(const TIDorb::core::comm::iiop::Version& ver,
                   TIDorb::core::comm::iiop::RequestId id);
 
  TIDorb::core::comm::iiop::ReplyStatusType reply_status() const;

  void set_service_context_list(TIDorb::core::comm::iiop::ServiceContextList* list);

  const TIDorb::core::comm::iiop::ServiceContextList* get_service_context_list() const;

  void insert_result_request(TIDorb::core::TIDORB* orb,
                             TIDorb::core::ServerRequestImpl* request);

  void extract_arguments(TIDorb::core::RequestImpl* request);

  CORBA::Any* extract_user_exception(CORBA::ExceptionList* list);

  //TIDorb::core::util::exceptions::SystemExceptionEncoder* extract_system_exception();
  CORBA::SystemException* extract_system_exception();

  void insert_forward(TIDorb::core::TIDORB* orb,
                      CORBA::Object* obj);

  TIDorb::core::iop::IOR* extract_forward();

  void insert_forward_perm(TIDorb::core::TIDORB* orb,
                           CORBA::Object* obj);

  TIDorb::core::iop::IOR* extract_forward_perm();

  void insert_needs_addressing_mode(TIDorb::core::TIDORB* orb,
                                    TIDorb::core::comm::iiop::AddressingDisposition disposition);

  TIDorb::core::comm::iiop::AddressingDisposition extract_addressing_disposition();

  void create_message_buffer_output(TIDorb::core::TIDORB* orb);

  void receive_body(TIDorb::core::comm::Connection* conn,
                    const CORBA::Octet* header_bytes);

  ~GIOPReplyMessage();

  void* _impl();
  const char* _typeid();

};
} //iiop
} //comm
} //core
} //TIDorb

ostream& operator<<(ostream& os,
                    const TIDorb::core::comm::iiop::GIOPReplyMessage& msg);
                    
#endif
