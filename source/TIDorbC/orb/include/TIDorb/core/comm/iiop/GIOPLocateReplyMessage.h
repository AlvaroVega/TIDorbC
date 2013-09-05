/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPLocateReplyMessage.h

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
