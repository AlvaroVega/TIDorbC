/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/MIOPCommLayer.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_MIOPCommLayer_H_
#define _TIDORB_core_comm_MIOPCommLayer_H_ 1


namespace TIDorb {
namespace core {
namespace comm {


class MIOPCommLayer : public TIDorb::core::comm::CommunicationLayer
{
   private:

   /**
    * Monitorizada....
    */
   TIDThr::RecursiveMutex recursive_mutex;

   /**
    * Tell whether or not it is synchronous operation from OMA of CORBA::Object
    */
   bool is_valid_synchronous_operation(const char* operation_name);

   /**
    * Connection manager.
    */
   ConnectionManager_ref connection_manager;


   public:

    /**
    * The ORB has been destroyed.
    */

    bool destroyed;

   public:

    MIOPCommLayer(TIDorb::core::TIDORB* orb,
                  TIDorb::core::comm::ConnectionManager* conn_mngr)
        throw(TIDThr::SystemException);

    /**
     * Sends a request using the IIOP profile component of the MIOP profile.
     * @param request the CORBA request.
     */
    void request(TIDorb::core::RequestImpl* request,
                 TIDorb::core::iop::IOR* ior)
        throw(TIDorb::core::ForwardRequest, CORBA::SystemException);

    /**
     * Sends a oneway request using the MIOP protocol allocating an active Connection
     * with the server referenced in the request target address.
     * @param request the CORBA request.
     */
    void oneway_request(TIDorb::core::RequestImpl* request,
                        TIDorb::core::iop::IOR* ior);

    /**
     * Sends a object existence request.
     * @param ior the object IOR.
     */
    bool object_exists(TIDorb::core::iop::IOR* ior,
                       const TIDorb::core::PolicyContext& policy_context)
        throw(TIDorb::core::ForwardRequest,CORBA::SystemException);

   /**
     * The layer can use this IOR to stablish a remote connection
     */
    bool accepts(const TIDorb::core::iop::IOR& ior);
    
    
  /**
   * Creates a Delegate for a CORBA::Object
   */
   
   TIDorb::core::ObjectDelegateImpl* createDelegate(TIDorb::core::iop::IOR* ior);
    
    /**
     * ORB Server Connection part shutdown.
     */
    void shutdown();

    /**
     * MIOP Layer close.
     */
    void destroy();

    bool is_local(TIDorb::core::iop::IOR* ior);
    bool is_multicast(TIDorb::core::iop::IOR* ior);

    /**
     * Join to multicast group (if listen_point is a multicast address),
     * or bind datagram --unicast-- socket to the listen point. NON-STANDARD FEATURE!
     */
    void join_group(const TIDorb::core::comm::miop::ListenPoint& listen_point);

};

} //comm
} //core
} //TIDorb

#endif
