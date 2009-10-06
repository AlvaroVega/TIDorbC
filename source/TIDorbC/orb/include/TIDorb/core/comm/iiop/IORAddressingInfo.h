/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/IORBAddressing.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_IORADDRESSINGINFO_H_
#define _TIDORB_CORE_COMM_IIOP_IORADDRESSINGINFO_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class IORAddressingInfo {
public:

        CORBA::ULong _selected_profile_index;
        TIDorb::core::iop::IOR _ior;

        TIDorb::core::comm::iiop::ProfileIIOP* get_ProfileIIOP();

        IORAddressingInfo(CORBA::ULong selected_profile_index, TIDorb::core::iop::IOR* ior);
        IORAddressingInfo(const IORAddressingInfo& info);

        static TIDorb::core::comm::iiop::IORAddressingInfo* read(TIDorb::core::cdr::CDRInputStream& input);

        void write(TIDorb::core::cdr::CDROutputStream& output) const;

        static void  write(TIDorb::core::cdr::CDROutputStream& output,
                           CORBA::ULong selected_profile_index, 
                           const TIDorb::core::iop::IOR& ior);
                     
        ~IORAddressingInfo() {};

};
}// iiop
}// comm
}// core
}// TIDorb
#endif

