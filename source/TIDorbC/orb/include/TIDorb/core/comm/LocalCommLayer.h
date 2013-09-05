/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/LocalCommLayer

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

    void reliable_oneway_run(TIDorb::core::RequestImpl* request,
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

    void get_poa_stuff();
    
 private:
    /**
     * Maximum time where a request is waiting for a response.
     */
    int m_max_response_blocked_time;

    /**
     * QoS enabled.
     */
    bool m_qos_enabled;

    TIDorb::core::poa::POAImpl * pPOAImpl;
    TIDorb::core::poa::POAManagerImpl* poaManager;
    
};

}
}
}

#endif
