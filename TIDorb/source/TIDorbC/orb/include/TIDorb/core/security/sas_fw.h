/* -----------------------------------------------------------------------------

 File: TIDorb/core/security/sas_fw.h
  
 Revisions:
 
 Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm_fw.h"

#ifndef _TIDORB_CORE_SECURITY_SAS_FW_H_
#define _TIDORB_CORE_SECURITY_SAS_FW_H_


// Forward Declarations

namespace TIDorb {
namespace core {
namespace security {
namespace sas {
  
  
  class SASServiceContext;

  class SASManager;
  typedef TIDThr::HandleT<SASManager> SASManager_ref;

}
}
}
}


#endif
