///////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationManager.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
  // pra@tid.es - MIOP extensions
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

  // pra@tid.es - MIOP extensions
  MIOPCommLayer* getMulticastLayer();
  // end MIOP extensions

};

} //comm
} //core
} //TIDorb

#endif
