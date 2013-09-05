/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/RequestId.h

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

#ifndef _TIDORB_CORE_COMM_IIOP_REQUESTID_H_
#define _TIDORB_CORE_COMM_IIOP_REQUESTID_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {


typedef CORBA::ULong RequestId;


// class RequestIdHolder
// {
// protected:
//         RequestId _value;

// public:
//         RequestIdHolder();
//         RequestIdHolder(RequestId val);
//         RequestIdHolder(const RequestIdHolder& val);

//         CORBA::ULong value() const;
//         CORBA::ULong hashCode() const;

//         RequestIdHolder& operator=(const RequestIdHolder& val);
//         RequestIdHolder& operator=(RequestId val);

//         bool operator==(RequestId val) const;
//         bool operator!=(RequestId val) const { return !(*this == val); }
//         bool operator< (RequestId val) const;
//         bool operator>=(RequestId val) const { return !(*this < val);  }
//         bool operator> (RequestId val) const;
//         bool operator<=(RequestId val) const { return !(*this > val);  }

//         bool operator==(const RequestIdHolder& val) const;
//         bool operator!=(const RequestIdHolder& val) const { return !(*this == val); }
//         bool operator< (const RequestIdHolder& val) const;
//         bool operator>=(const RequestIdHolder& val) const { return !(*this < val);  }
//         bool operator> (const RequestIdHolder& val) const;
//         bool operator<=(const RequestIdHolder& val) const { return !(*this > val);  }

//         char* toString() const;
// };


} //iiop
} //comm
} //core
} //TIDorb

#endif
