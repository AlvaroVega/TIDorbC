/////////////////////////////////////////////////////////////////////////
//
// File:        OperationCompletion.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////


 
#include "TIDorb/core/util.h"


/**
 * The operation is completed.<p>
 * If there is a thread waiting for the completion notifies it.
 */  

void TIDorb::core::util::OperationCompletion::set_completed() 
{
  TIDThr::Synchronized synchro (*this);
  
  OperationStatus current_state = m_state;
  
  m_state = COMPLETED;
  
  if(current_state == WAITING)
    notify();
}
	
/**
 * Makes a thread wait for the operation completion (with a timeout).
 * @timeout the timeout
 */
void TIDorb::core::util::OperationCompletion::wait_for_completion(int timeout) 
  throw (TIDThr::InterruptedException, TIDorb::core::util::OnlyOneThreadCanWait)
{

//TODO: revisar esta solucion temporal al problema de los throws
//PRA: bug #66 de TIDorbC en babel.hi.inet
try {
//EPRA  

  TIDThr::Synchronized synchro (*this);


  if(m_state == INITIAL) { 
    m_state = WAITING;
    wait(timeout);
  } else if (m_state == WAITING) {
    throw TIDorb::core::util::OnlyOneThreadCanWait();
  }
  // else do nothing, is completed


//PRA: bug #66 de TIDorbC en babel.hi.inet
} catch (const TIDThr::Exception& exc) {
  throw TIDThr::InterruptedException(exc.what());
}
//EPRA


}
