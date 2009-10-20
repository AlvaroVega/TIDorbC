/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPFragmentMessage.h

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
