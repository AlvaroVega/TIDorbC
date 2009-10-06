/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/AddressingDisposition.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_ADRESSINGDISPOSITION_H_
#define _TIDORB_CORE_COMM_IIOP_ADRESSINGDISPOSITION_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

enum AddressingDisposition {KeyAddr=0, ProfileAddr, ReferenceAddr=2};

class AddressingDispositionHelper {
  public:
   static AddressingDisposition from_ushort(CORBA::UShort type);
   static AddressingDisposition read(TIDorb::portable::InputStream& input);
   static void write(TIDorb::portable::OutputStream& output, AddressingDisposition value);
};

}//iiop
}//comm
}//core
}//TIDorb

#endif
