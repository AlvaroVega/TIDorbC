/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/LocalCommLayer

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_LocalCommLayer_H_
#define _TIDORB_core_comm_LocalCommLayer_H_

/**
 * Layer that dispatchs the request to local objects.
 *
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Javier Fdez. Mejuto
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace comm {

class LocalCommLayer : public CommunicationLayer
{
  public:
    LocalCommLayer(TIDorb::core::TIDORB* orb) throw (TIDThr::SystemException);

    bool object_exists(TIDorb::core::iop::IOR* ior, 
                       const TIDorb::core::PolicyContext& policy_context)
      throw(TIDorb::core::ForwardRequest,CORBA::SystemException);

    void request(TIDorb::core::RequestImpl* request,
                 TIDorb::core::iop::IOR* ior)
      throw(TIDorb::core::ForwardRequest,CORBA::SystemException);

    void oneway_request(TIDorb::core::RequestImpl* request,
                        TIDorb::core::iop::IOR* ior);
                        
    bool is_local() const { return true;}
    
    /**
     * The layer can use this IOR to stablish a remote connection
     */
   
     bool accepts(const TIDorb::core::iop::IOR& ior);
  
    /**
     * Creates a Delegate for a CORBA::Object
     */
   
     TIDorb::core::ObjectDelegateImpl* 
         createDelegate(TIDorb::core::iop::IOR* ior);
    
  protected:
    void send_request(TIDorb::core::RequestImpl* request,
                      TIDorb::core::iop::IOR* ior)
      throw(TIDorb::core::ForwardRequest,CORBA::SystemException);
    
 private:
    /**
     * Maximum time where a request is waiting for a response.
     */
    int m_max_response_blocked_time;

    /**
     * QoS enabled.
     */
    bool m_qos_enabled;
    
};

}
}
}

#endif
