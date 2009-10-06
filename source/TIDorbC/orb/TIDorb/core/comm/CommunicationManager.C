/////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationManager.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/CommunicationManager.h"



TIDorb::core::comm::CommunicationManager::CommunicationManager(TIDorb::core::TIDORB* orb)     
  : _orb(orb)
{
  external_layer = new TIDorb::core::comm::IIOPCommLayer(orb);
  internal_layer = new TIDorb::core::comm::LocalCommLayer(orb);

  // pra@tid.es - MIOP extensions
  TIDorb::core::comm::ConnectionManager* conn_mngr = external_layer->getConnectionManager();
  multicast_layer = new TIDorb::core::comm::MIOPCommLayer(orb, conn_mngr);
  // end MIOP extensions
}

 
  
void TIDorb::core::comm::CommunicationManager::shutdown()
{
  external_layer->shutdown();

  // pra@tid.es - MIOP extensions
  multicast_layer->shutdown();
  // end MIOP extensions
}




void TIDorb::core::comm::CommunicationManager::destroy()
{
//MLG Cambio el orden porque solo elimino el connectionmanager en la IIOPCommLayer
//y la MIOPCommLayer tiene una referencia
  // pra@tid.es - MIOP extensions
  multicast_layer->destroy();
  // end MIOP extensions	
  external_layer->destroy();
//EMLG
}




TIDorb::core::comm::IIOPCommLayer* 
    TIDorb::core::comm::CommunicationManager::getExternalLayer()
{
  return external_layer;
}



//
// pra@tid.es - MIOP extensions
//

TIDorb::core::comm::MIOPCommLayer* 
    TIDorb::core::comm::CommunicationManager::getMulticastLayer()
{
  return multicast_layer;
}

/**
 * Creates a Delegate for a CORBA::Object
 */
   
TIDorb::core::ObjectDelegateImpl* 
   TIDorb::core::comm::CommunicationManager::createDelegate
     (TIDorb::core::iop::IOR* ior)
{
    if(multicast_layer->accepts(*ior)) {
      return multicast_layer->createDelegate(ior);
    } else if(internal_layer->accepts(*ior)) {
        return internal_layer->createDelegate(ior);
    } else if(external_layer->accepts(*ior)) {
        return external_layer->createDelegate(ior);
    } else {
        throw CORBA::INV_OBJREF();
    } 
}
