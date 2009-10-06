/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPCancelRequestMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPCANCELREQUESTMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPCANCELREQUESTMESSAGE_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPCancelRequestMessage : public GIOPMessage {
private:

        RequestId request_id;

public:

        GIOPCancelRequestMessage(const Version& version,
                                 RequestId id);

        GIOPCancelRequestMessage(const GIOPHeader& header);
        
        RequestId getRequestId() const;

        void set_body(TIDorb::core::cdr::BufferCDR_ref buf,
                      TIDorb::core::cdr::CDRInputStream* input);

        void receive_body (TIDorb::core::comm::Connection* conn,
                           const CORBA::Octet* header_buffer);

        TIDorb::core::cdr::BufferCDR* get_message_buffer();

        bool hasBody() const;

        ~GIOPCancelRequestMessage() {}

	void* _impl();
	const char* _typeid();

};
} //iiop
} //comm
} //core
} //TIDorb
#endif
