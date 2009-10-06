///////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationLayer.h
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

