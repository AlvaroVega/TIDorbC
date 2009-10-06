/* -----------------------------------------------------------------------------

 File:TIDorb/core/util/OperationCompletion.def
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

 
#include "TIDorb/core/util.h"

#ifndef _TIDORB_CORE_UTIL_OPERATION_COMPLETION_H_
#define _TIDORB_CORE_UTIL_OPERATION_COMPLETION_H_

#include "TIDThr.h"

/**
 * Manages the completion status of an operation.<p>
 * Also, makes one thread to wait the completion.
 * <p>The status are:
 * <ul>
 * <li><code>INITIAL</code>: the operation is not completed and there is not 
 * any Thread waiting for the completion
 * <li>WAITING: there is a Thread that is waiting for the completion
 * <li>COMPLETED: the operation is completed.
 * </ul>
 * 
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace util {
 
class OperationCompletion : public virtual TIDThr::Monitor 
{
	
public:

  enum OperationStatus { INITIAL = 0, WAITING, COMPLETED};
	
//MLG	
  OperationCompletion() throw (TIDThr::SystemException) : m_state(INITIAL) {}
  ~OperationCompletion() throw(TIDThr::SystemException) {}
//EMLG
	
  /**
   * @return whether or not the operation is completed
   */
  bool is_completed() {
    return m_state==COMPLETED;
  }

  /**
   * The operation is completed.<p>
   * If there is a thread waiting for the completion notifies it.
   */  
  void set_completed();
	
  /**
   * Makes a thread wait for the operation completion (with a timeout).
   * @timeout the timeout
   */
  void wait_for_completion(int timeout) 
    throw (TIDThr::InterruptedException, TIDorb::core::util::OnlyOneThreadCanWait);

  /**
   * Set to INITIAL state
   */  
  void recycle(){
    m_state = INITIAL;
  }

protected:

  /**
   * The Completion status.
   */
  
  OperationStatus m_state;
  
};
//FRAN
typedef TIDThr::HandleT<OperationCompletion> OperationCompletion_ref;
//EFRAN

} // TIDorb
} // core
} // util


#endif
