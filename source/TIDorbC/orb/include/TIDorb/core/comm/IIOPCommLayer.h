/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/IIOPCommLayer

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_IIOPCommLayer_H_
#define _TIDORB_core_comm_IIOPCommLayer_H_ 1


#include "TIDSocket.h"
#include "CORBA.h"
#include "FT.h"


/**
 *  IIOP Communications layer. Manages the request for a remote object using
 *  the IIOP Protocol.
 *
 * <p> Copyright 2003 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace comm {

class IIOPCommLayer : public CommunicationLayer
{
   private:

   /**
    * Monitorizada....
    */
   TIDThr::RecursiveMutex recursive_mutex;

   /**
    * Server Socket listening thread.
    */
   ServerListener_ref server_listener;

   /**
    * Service context that contains the information for activating the bidirectional service.
    */
   TIDorb::core::comm::iiop::ServiceContextList* bidirectional_service;

   /**
    * Connection manager.
    */
   ConnectionManager_ref connection_manager;


   // pra@tid.es - FT extensions
   CORBA::Boolean             server_heartbeat_enabled;
   ::FT::HeartbeatPolicyValue client_heartbeat_policy;
   // end FT extensions


   TIDThr::ThreadGroupHandle _group;

   time_t max_recover_count;
   time_t recover_time;
   

   public:

    /**
    * The ORB has been destroyed.
    */

    bool destroyed;

   public:

    IIOPCommLayer(TIDorb::core::TIDORB* orb) throw (TIDThr::SystemException);

    bool is_initialized() { return !server_listener.is_null();}

    /**
     * Sends a request using the IIOP protocol.
     * @param request the CORBA request.
     */
    void request(TIDorb::core::RequestImpl* request,
                      TIDorb::core::iop::IOR* ior)
        throw(TIDorb::core::ForwardRequest, CORBA::SystemException);


    /**
     * Sends a oneway request using the IIOP protocol allocating an active Connection
     * with the server referenced in the request target address.
     * @param request the CORBA request.
     */
    void oneway_request(TIDorb::core::RequestImpl* request,
                             TIDorb::core::iop::IOR* ior);
    /**
     * Sends a oneway request using the IIOP protocol allocating an active Connection
     * with the server referenced in the request target address.
     * @param request the CORBA request.
     */
    void reliable_oneway_run(TIDorb::core::RequestImpl* request,
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
   
     TIDorb::core::ObjectDelegateImpl* 
         createDelegate(TIDorb::core::iop::IOR* ior);
    
    /**
     * ORB Server Connection part shutdown.
     */
    void shutdown();

    /**
     * IIOP Layer close.
     */

    bool is_local(const TIDorb::core::iop::IOR& ior) const;
    
    bool has_server_listener() {return !server_listener.is_null();}

    void destroy();

    void init_server_listener();

    TIDorb::core::iop::IOR* createIOR(const char* id, TIDorb::core::poa::POAKey* key, 
                                      const TIDorb::core::iop::VectorTaggedComponent& extraComponents);
    TIDorb::core::comm::iiop::ServiceContextList* get_bidirectional_service();

    // pra@tid.es - FT extensions
    ::FT::HeartbeatPolicyValue& getClientHeartbeatPolicy() { return client_heartbeat_policy; }
    TIDorb::core::comm::ConnectionManager* getConnectionManager() { return connection_manager; }
    // end FT extensions


    /**
     * Sends a request (with response) allocating an active connection with
     * the corresponding server referenced by its target address.
     * <p>If necessary, tries to do a recovery loop.
     * @param request the CORBA request.
     */
   protected:

    void getListenPoint(TIDorb::core::iop::IOR* ior,
                        TIDorb::core::comm::iiop::ListenPoint& listen_point)
        throw(CORBA::SystemException);

    void send_request(TIDorb::core::RequestImpl* request, TIDorb::core::iop::IOR* ior)
        throw(TIDorb::core::ForwardRequest,CORBA::SystemException);
};

}
}
}

#endif

