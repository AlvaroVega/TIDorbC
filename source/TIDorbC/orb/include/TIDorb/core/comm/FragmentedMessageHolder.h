/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FragmentedMessageHolder.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_FragmentedMessageHolder_H_
#define _TIDORB_core_comm_FragmentedMessageHolder_H_ 1


/**
 * Adapter class for maintaining an uncompleted fragmented message in a 
 * <code>UseTable</code>.
 * @see es.tid.TIDorbj.core.util.UseTable
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

#include "TIDorb/core/util.h"

namespace TIDorb {
  
namespace core {
  
namespace comm {
   
class FragmentedMessageHolder : public TIDorb::core::util::RemovableObject {
    
  protected:
  
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* message;

  public: 
  
  FragmentedMessageHolder(TIDorb::core::comm::iiop::GIOPFragmentedMessage* msg);
  virtual ~FragmentedMessageHolder();
  
  /**
   * Gets the message
   */
   
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* get_message()
  {
    return message;
  }
  
  /**
   * Gets and set to NULL the message, next consume_message will be NULL
   */
   
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* consume_message();
  
  bool canBeRemoved();
};

}

}

}
#endif

