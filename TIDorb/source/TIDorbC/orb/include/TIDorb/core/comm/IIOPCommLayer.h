/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/IIOPCommLayer

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
   protected:

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


   // FT extensions
   CORBA::Boolean             server_heartbeat_enabled;
   ::FT::HeartbeatPolicyValue client_heartbeat_policy;
   // end FT extensions


   TIDThr::ThreadGroupHandle _group;

   time_t max_recover_count;
   time_t recover_time;


   TIDorb::core::security::sas::SASManager_ref sas_manager;

   public:

    /**
    * The ORB has been destroyed.
    */

    bool destroyed;

   public:

    IIOPCommLayer(TIDorb::core::TIDORB* orb) throw (TIDThr::SystemException);

    virtual bool is_initialized() { return !server_listener.is_null();}

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
    virtual void oneway_request(TIDorb::core::RequestImpl* request,
                                TIDorb::core::iop::IOR* ior);
    /**
     * Sends a oneway request using the IIOP protocol allocating an active Connection
     * with the server referenced in the request target address.
     * @param request the CORBA request.
     */
    virtual void reliable_oneway_run(TIDorb::core::RequestImpl* request,
                                     TIDorb::core::iop::IOR* ior);

    /**
     * Sends a object existence request.
     * @param ior the object IOR.
     */
    virtual bool object_exists(TIDorb::core::iop::IOR* ior, 
                               const TIDorb::core::PolicyContext& policy_context)
        throw(TIDorb::core::ForwardRequest,CORBA::SystemException);

   /**
     * The layer can use this IOR to stablish a remote connection
     */
   
     virtual bool accepts(const TIDorb::core::iop::IOR& ior);
  
    /**
     * Creates a Delegate for a CORBA::Object
     */
   
     virtual TIDorb::core::ObjectDelegateImpl* 
         createDelegate(TIDorb::core::iop::IOR* ior);
    
    /**
     * ORB Server Connection part shutdown.
     */
    virtual void shutdown();

    /**
     * IIOP Layer close.
     */

    virtual bool is_local(const TIDorb::core::iop::IOR& ior) const;
    
    virtual bool has_server_listener() {return !server_listener.is_null();}

    virtual void destroy();

    virtual void init_server_listener();

    virtual TIDorb::core::iop::IOR* createIOR(const char* id, TIDorb::core::poa::POAKey* key, 
                                      const TIDorb::core::iop::VectorTaggedComponent& extraComponents);
    TIDorb::core::comm::iiop::ServiceContextList* get_bidirectional_service();

    // FT extensions
    ::FT::HeartbeatPolicyValue& getClientHeartbeatPolicy() { return client_heartbeat_policy; }
    TIDorb::core::comm::ConnectionManager* getConnectionManager() { return connection_manager; }
    // end FT extensions


    TIDorb::core::security::sas::SASManager* getSASManager() { return sas_manager; }

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

    virtual void send_request(TIDorb::core::RequestImpl* request, TIDorb::core::iop::IOR* ior)
        throw(TIDorb::core::ForwardRequest,CORBA::SystemException);
};

}
}
}

#endif

