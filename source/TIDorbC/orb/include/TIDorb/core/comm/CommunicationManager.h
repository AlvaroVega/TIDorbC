///////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationManager.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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
///////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_CommunicationManager_H_
#define _TIDORB_core_comm_CommunicationManager_H_


/**
 * Communication Layer Manager. It will manage requests,
 * dispatching these to the local or the internet communication layers.
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace comm {

class CommunicationManager
{
  private:
  
  /**
   * The ORB
   */
  TIDorb::core::TIDORB* _orb;
  
  /**
   * the internet layer for sending remote request
   */
  IIOPCommLayer_ref external_layer;


  /**
   * the internet layer for sending multicast request
   */
  // MIOP extensions
  MIOPCommLayer_ref multicast_layer;
  // end MIOP extensions


  /**
   * the local layer for sending request to local objects.
   */
  LocalCommLayer_ref internal_layer;


  public:
   CommunicationManager(TIDorb::core::TIDORB* orb);
    
   virtual ~CommunicationManager(){}
 
   /**
   * Creates a Delegate for a CORBA::Object
   */
   
   TIDorb::core::ObjectDelegateImpl* 
       createDelegate(TIDorb::core::iop::IOR* ior);      
   

  void shutdown();
  void destroy();

  IIOPCommLayer* getExternalLayer();

  // MIOP extensions
  MIOPCommLayer* getMulticastLayer();
  // end MIOP extensions

};

} //comm
} //core
} //TIDorb

#endif
