///////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationLayer.h
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

#ifndef _TIDORB_core_comm_CommunicationLayer_H_
#define _TIDORB_core_comm_CommunicationLayer_H_


#include "CORBA.h"


/**
 * Base class for the ORB communications layers.
 */

namespace TIDorb {
namespace core {
namespace comm {

class CommunicationLayer : public TIDThr::RefCounter
{
  protected:
  TIDorb::core::TIDORB* _orb;
  const TIDorb::core::ConfORB& conf;
  	
  public:

  CommunicationLayer(TIDorb::core::TIDORB* orb) throw(TIDThr::SystemException);
  virtual ~CommunicationLayer() throw(TIDThr::SystemException) {}

  TIDorb::core::TIDORB* orb()
	{
	  return _orb;
	} 
	
  /**
   * Sends a request.
   * @param request the CORBA request.
   */
  virtual void request(TIDorb::core::RequestImpl* request,
                       TIDorb::core::iop::IOR* ior)
        throw(TIDorb::core::ForwardRequest,CORBA::SystemException) = 0;

  /**
   * Sends a oneway request.
   * @param request the CORBA request.
   */
  virtual void oneway_request(TIDorb::core::RequestImpl* request,
                              TIDorb::core::iop::IOR* ior) = 0;


  virtual void reliable_oneway_run(TIDorb::core::RequestImpl* request,
                                   TIDorb::core::iop::IOR* ior) = 0;

  /**
   * Sends a object existence request.
   * @param ior the object IOR.
   */
  virtual bool object_exists(TIDorb::core::iop::IOR* ior,
                             const TIDorb::core::PolicyContext& policy_context)
        throw(TIDorb::core::ForwardRequest,CORBA::SystemException) = 0;

   /**
    * @return wether the layer is local or not
    */
  virtual bool is_local_layer() const {return false;}
   
  /**
   * The layer can use this IOR to stablish a remote connection
   */
   
  virtual bool accepts(const TIDorb::core::iop::IOR& ior) = 0;
  
  /**
   * Creates a Delegate for a CORBA::Object
   */
   
   virtual TIDorb::core::ObjectDelegateImpl* 
     createDelegate(TIDorb::core::iop::IOR* ior) = 0;
  
};

} //comm
} //core
} //TIDorb
#endif

