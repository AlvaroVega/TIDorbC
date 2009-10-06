/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/miop_fw.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm_fw.h"

#ifndef _TIDORB_CORE_COMM_MIOP_FW_H_
#define _TIDORB_CORE_COMM_MIOP_FW_H_


// Forward Declarations

namespace TIDorb {
namespace core {
namespace comm {
namespace miop {
  
  typedef TIDorb::core::comm::iiop::Version Version;
  typedef TIDorb::core::comm::iiop::ListenPoint ListenPoint;

  class ProfileMIOP;
  typedef TIDThr::HandleT<ProfileMIOP> ProfileMIOP_ref;

  class GroupInfo;
  class GroupIIOPProfile;

  class MIOPCorbaloc;
  class MIOPPacket;
  class MIOPPacketCollection;

  typedef CORBA::ULongLong UniqueId; // 8 bytes

}
}
}
}


#endif
