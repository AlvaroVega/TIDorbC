/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ListenPoint.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"


#ifndef _TIDORB_CORE_COMM_IIOP_LISTENPOINT_H_
#define _TIDORB_CORE_COMM_IIOP_LISTENPOINT_H_


#include "CORBA.h"
#include "FT.h"


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {


class ListenPoint
{
private:
        char* _str;

public:
        char*         _host;
        CORBA::UShort _port;
        char*         _interface;
        char*         _incoming_interface;
        char*         _outgoing_interface;

        // pra@tid.es - FT extensions
        CORBA::Boolean _heartbeat_enabled;
        // end FT extensions

        ListenPoint();
        ListenPoint(const char* host, CORBA::UShort port);
        ListenPoint(const char* host, CORBA::UShort port, 
                    const char* incoming_interface, const char* outgoing_interface);
        ListenPoint(const ListenPoint& other);

        ~ListenPoint();

        ListenPoint& operator= (const ListenPoint& obj);

        bool operator== (const ListenPoint& obj) const;
        bool operator!= (const ListenPoint& obj) const { return !(*this == obj); }
        bool operator<  (const ListenPoint& obj) const;

        //PRA
        // hash code
        //CORBA::ULong hashCode();
        //EPRA

        void read(TIDorb::core::cdr::CDRInputStream& input);
        void write(TIDorb::core::cdr::CDROutputStream& output) const;

        const char* toString() const;
        bool fromString(const char* s);

};

} //iiop
} //comm
} //core
} //TIDorb

#endif
