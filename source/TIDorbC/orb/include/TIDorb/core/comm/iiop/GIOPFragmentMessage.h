/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPFragmentMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPFRAGMENTMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPFRAGMENTMESSAGE_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPFragmentMessage : public GIOPMessage {
private:
        RequestId _request_id;
public:

        static CORBA::ULong FRAGMENT_HEADER_SIZE_1_1;
        static CORBA::ULong FRAGMENT_HEADER_SIZE_1_2;

        GIOPFragmentMessage(const Version& version,
                            RequestId id);

        GIOPFragmentMessage(const GIOPHeader& header);


        RequestId getRequestId() const;

        bool hasBody() const;
        
		void set_body(TIDorb::core::cdr::BufferCDR_ref buf,
                      TIDorb::core::cdr::CDRInputStream* input);

        void receive_body (TIDorb::core::comm::Connection* conn,
                           const CORBA::Octet* header_bytes);

        static void write_header(TIDorb::core::cdr::CDROutputStream& out,
                           GIOPHeader* header,
                           RequestId request_id);

        void send(TIDorb::core::comm::Connection* conn);

        static void skip_fragment_header_1_2(TIDorb::core::cdr::CDRInputStream& in);

        static void skip_fragment_header_1_2(TIDorb::core::cdr::CDROutputStream& out);

        ~GIOPFragmentMessage();

	void* _impl();
	const char* _typeid();
};
} //iiop
} //comm
} //core
} //TIDorb

ostream& operator<<(ostream&,
                    const TIDorb::core::comm::iiop::GIOPFragmentMessage&);
                    
#endif
