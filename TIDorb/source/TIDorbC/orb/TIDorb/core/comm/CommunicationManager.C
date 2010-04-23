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

#include "TIDorb/core/comm/CommunicationManager.h"



TIDorb::core::comm::CommunicationManager::CommunicationManager(TIDorb::core::TIDORB* orb)     
  : _orb(orb)
{

  const TIDorb::core::ConfORB& conf = orb->conf();

  // Check if minimum SSL configuration is available
  // TODO: NoProtection when no SSL configuration is provided??
  if ( (strcmp(conf.ssl_private_key, "") != 0) &&  
       (strcmp(conf.ssl_certificate, "") != 0) ) {
    external_layer = new TIDorb::core::comm::SSLIIOPCommLayer(orb);
  } 
  else {
    external_layer = new TIDorb::core::comm::IIOPCommLayer(orb);
  }

  internal_layer = new TIDorb::core::comm::LocalCommLayer(orb);

  // MIOP extensions
  TIDorb::core::comm::ConnectionManager* conn_mngr = external_layer->getConnectionManager();
  multicast_layer = new TIDorb::core::comm::MIOPCommLayer(orb, conn_mngr);
  // end MIOP extensions
}

 
  
void TIDorb::core::comm::CommunicationManager::shutdown()
{
  external_layer->shutdown();

  // MIOP extensions
  multicast_layer->shutdown();
  // end MIOP extensions
}




void TIDorb::core::comm::CommunicationManager::destroy()
{
  //Cambio el orden porque solo elimino el connectionmanager en la IIOPCommLayer
  //y la MIOPCommLayer tiene una referencia

  // MIOP extensions
  multicast_layer->destroy();
  // end MIOP extensions	
  external_layer->destroy();
}




TIDorb::core::comm::IIOPCommLayer* 
    TIDorb::core::comm::CommunicationManager::getExternalLayer()
{
  return external_layer;
}



//
// MIOP extensions
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
  // TODO: Improvement performance: 'accepts' is called 2 times: 
  // 1 explicit here and 1 indirectly by layer::createDelegate
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
