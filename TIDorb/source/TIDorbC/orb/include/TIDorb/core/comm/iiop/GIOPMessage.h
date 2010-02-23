/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPMessage.h

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

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPMESSAGE_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPMessage {

protected:

        TIDorb::core::comm::iiop::GIOPHeader _header;
        TIDorb::core::cdr::BufferCDR_ref     _message_buffer;
        TIDorb::core::cdr::BufferCDR_ref     _body_buffer;
        CORBA::Boolean                       _message_completed;
        CORBA::Boolean                       _headers_marshaled;

        Compression::CompressorIdLevel       _compressor;

        virtual void write_headers();
        GIOPMessage();

public:

        GIOPMessage(const GIOPHeader& header);
        virtual ~GIOPMessage();
        
        const GIOPHeader& getHeader() const;

        TIDorb::core::cdr::BufferCDR* get_message_buffer() const;

        void set_message_buffer(TIDorb::core::cdr::BufferCDR* cdr);

        void set_message_completed(CORBA::Boolean value);

        virtual bool hasBody() const;

        //static TIDorb::core::cdr::ChunkCDR* receive_chunk(TIDorb::core::comm::Connection* conn,
        //                                                  TIDorb::core::comm::iiop::GIOPHeader* header,
        //                                                  const CORBA::Octet* header_bytes);
        void set_body(TIDorb::core::cdr::BufferCDR_ref buf);
 
        virtual void receive_body(TIDorb::core::comm::Connection* conn,
                                  const CORBA::Octet* header_bytes);

        //void send(TIDorb::core::comm::Connection* conn) const;

        Compression::CompressorIdLevel get_compressor() const;
        void set_compressor(Compression::CompressorIdLevel compressor);


	virtual void* _impl();
	virtual const char* _typeid();

};

} //iiop
} //comm
} //core
} //TIDorb

ostream& operator<<(ostream&, const TIDorb::core::comm::iiop::GIOPMessage&);

#endif
