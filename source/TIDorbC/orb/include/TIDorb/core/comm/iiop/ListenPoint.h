/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ListenPoint.h

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
