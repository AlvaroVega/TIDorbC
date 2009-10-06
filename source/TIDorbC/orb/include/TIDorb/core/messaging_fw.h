//////////////////////////////////////////////////////////////////////////////////
//
// File:        messaging_fw.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_MESSSAGING_FW_H_
#define _TIDORB_CORE_MESSSAGING_FW_H_

// Forward Declarations

namespace TIDorb {
namespace core {
namespace messaging {
  
  class PoliciesComponent;
  typedef TIDThr::HandleT<PoliciesComponent> PoliciesComponent_ref;
  class QoS;
  class QueueOrderPolicyImpl;
  class RebindPolicyImpl;
  class RelativeRequestTimeoutPolicyImpl;
  class RelativeRoundtripTimeoutPolicyImpl;
  class RequestEndTimePolicyImpl;
  class RequestPriorityPolicyImpl;
  class RequestStartTimePolicyImpl;


}
}
}
#endif
