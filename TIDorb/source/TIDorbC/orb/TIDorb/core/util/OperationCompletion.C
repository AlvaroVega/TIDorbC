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

try {

  TIDThr::Synchronized synchro (*this);


  if(m_state == INITIAL) { 
    m_state = WAITING;
    wait(timeout);
  } else if (m_state == WAITING) {
    throw TIDorb::core::util::OnlyOneThreadCanWait();
  }
  // else do nothing, is completed


} catch (const TIDThr::Exception& exc) {
  throw TIDThr::InterruptedException(exc.what());
}


}
