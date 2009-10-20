#include "TIDorb/core.h"

#ifndef _TIDorb_core_PROCESSING_STATE_H_
#define _TIDorb_core_PROCESSING_STATE_H_


/* -----------------------------------------------------------------------------

 File: TIDorb/core/ProcessingState.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDThr.h"

namespace TIDorb {
namespace core {


class ProcessingState : public virtual TIDThr::Monitor {
 
public:

  enum ProcessingStatus {CLIENT = 0, RUNNING, SHUTDOWNING, SHUTDOWNED};

//MLG  
  ProcessingState (TIDORB* orb) throw (TIDThr::SystemException) 
    : m_state(CLIENT), m_orb(orb)  {}
  ~ProcessingState() throw (TIDThr::SystemException) {}  
//EMLG
  
  ProcessingStatus state() {
    return m_state;
  }

  void running();
  
  bool is_shutdowned();
  
  void shutdown();
  
  void shutdowned();
  
  void wait_for_shutdown();


protected:
  
  ProcessingStatus m_state;
  
  TIDORB* m_orb;  
  
};

} // TIDorb
} // core


#endif
