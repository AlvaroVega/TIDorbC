/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPFragmentedMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPFRAGMENTEDMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPFRAGMENTEDMESSAGE_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPFragmentedMessage : public GIOPMessage {
protected:

        GIOPFragmentedMessage();

        GIOPFragmentedMessage(const GIOPHeader& header);

        GIOPFragmentedMessage(RequestId request_id,
                              size_t fragment_size = 0);

        GIOPFragmentedMessage(const GIOPHeader& header,
                              RequestId request_id,
                              size_t fragment_size = 0);

        // TODO: GIOPFragmentedMessage: ?tiene que ser virtual?
        void create_message_buffer_output(TIDorb::core::TIDORB* orb);

        void create_body_buffer_output(TIDorb::core::TIDORB* orb);

        RequestId _request_id;

        TIDorb::core::cdr::CDRInputStream* message_buffer_in;

        TIDorb::core::cdr::CDROutputStream* message_buffer_out;

        TIDorb::core::cdr::CDROutputStream* body_buffer_in;

        TIDorb::core::cdr::CDROutputStream* body_buffer_out;

  	// MIOP extensions
        size_t _fragment_size;
        size_t get_fragment_size(TIDorb::core::TIDORB* orb);
	// end MIOP extensions

public:        

        RequestId getRequestId() const;
        
        void set_body(TIDorb::core::cdr::BufferCDR_ref buf, 
                      TIDorb::core::cdr::CDRInputStream* input);
                      
        void receive_body(TIDorb::core::comm::Connection* conn,
                          const CORBA::Octet* header_bytes);

        void write_headers();

        void add_fragment(GIOPFragmentMessage* fragment); //consume

        ~GIOPFragmentedMessage();

	void* _impl();
	const char* _typeid();

};
} //iiop
} //comm
} //core
} //TIDorb

ostream& operator<<(ostream& os, 
                    const TIDorb::core::comm::iiop::GIOPFragmentedMessage& header);

#endif
