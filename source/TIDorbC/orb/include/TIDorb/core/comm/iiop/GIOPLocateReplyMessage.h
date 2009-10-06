/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPLocateReplyMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPLOCATEREPLYMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPLOCATEREPLYMESSAGE_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPLocateReplyMessage : public GIOPFragmentedMessage {
private:

  LocateReplyStatusType _reply_status;

  void reset();

public:

  GIOPLocateReplyMessage(const GIOPHeader& header);

  GIOPLocateReplyMessage(const Version& version, RequestId id);
 
  LocateReplyStatusType reply_status() const;

  void insert_unknown_object(TIDorb::core::TIDORB* orb);

  void insert_object_here(TIDorb::core::TIDORB* orb);

  void insert_system_exception(TIDorb::core::TIDORB* orb,
                               const CORBA::SystemException& exception);

  //TIDorb::core::util::exceptions::SystemExceptionEncoder* extract_system_exception();
  CORBA::SystemException* extract_system_exception();

  void insert_forward(TIDorb::core::TIDORB* orb,
                      CORBA::Object* obj);

  TIDorb::core::iop::IOR* extract_forward();

  void insert_forward_perm(TIDorb::core::TIDORB* orb,
                           CORBA::Object* obj);

  TIDorb::core::iop::IOR* extract_forward_perm();

  void insert_needs_addressing_mode(TIDorb::core::TIDORB* orb,
                                    AddressingDisposition disposition);

  AddressingDisposition extract_addressing_disposition();

  void create_message_buffer_output(TIDorb::core::TIDORB* orb);

  void receive_body(TIDorb::core::comm::Connection* conn,
                    const CORBA::Octet* header_bytes);

  ~GIOPLocateReplyMessage() {};

  void* _impl();
  const char* _typeid();

};
} //iiop
} //comm
} //core
} //TIDorb

ostream& operator<<(ostream& os,
                    const TIDorb::core::comm::iiop::GIOPLocateReplyMessage& msg);
                    
#endif
