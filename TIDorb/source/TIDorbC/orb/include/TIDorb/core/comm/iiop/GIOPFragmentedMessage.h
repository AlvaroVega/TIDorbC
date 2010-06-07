/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPFragmentedMessage.h

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

        //void create_body_buffer_output(TIDorb::core::TIDORB* orb);

        RequestId _request_id;

        TIDorb::core::cdr::CDRInputStream* message_buffer_in;

        TIDorb::core::cdr::CDROutputStream* message_buffer_out;

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
