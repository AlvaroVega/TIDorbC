/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/RequestId.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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