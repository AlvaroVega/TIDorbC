/////////////////////////////////////////////////////////////////////////
//
// File:        ProcessingState.C
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

#include "TIDorb/core.h"
  
void TIDorb::core::ProcessingState::running()
{
  TIDThr::Synchronized synchro (*this);
  if(m_state == CLIENT) {
    m_state = RUNNING;
  }
}

bool TIDorb::core::ProcessingState::is_shutdowned()
{
  return m_state == SHUTDOWNED;
}

void TIDorb::core::ProcessingState::shutdown()
{
  TIDThr::Synchronized synchro (*this);
  
  if(m_state == RUNNING) {
    m_state = SHUTDOWNING;
    ShutdownThread_ref thread =  new ShutdownThread(m_orb);
    thread->start();
  }    
}
  
void TIDorb::core::ProcessingState::shutdowned()
{
  TIDThr::Synchronized synchro (*this);
  
  if(m_state == SHUTDOWNING) {
    m_state = SHUTDOWNED;
    notifyAll();
  }
}

void TIDorb::core::ProcessingState::wait_for_shutdown()
{
  TIDThr::Synchronized synchro (*this);
  
  if(m_state == SHUTDOWNING) {  
    try {
      wait(m_orb->conf().max_time_in_shutdown);
    } catch (const TIDThr::InterruptedException& ie) {}
    
  } else if (m_state == RUNNING){
    try{
      wait();
    } catch (const TIDThr::InterruptedException& ie) {}
  }
}
