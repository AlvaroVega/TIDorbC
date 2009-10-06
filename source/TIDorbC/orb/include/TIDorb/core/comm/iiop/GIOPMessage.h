/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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
