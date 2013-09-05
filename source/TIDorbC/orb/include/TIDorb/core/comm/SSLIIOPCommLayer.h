/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/SSLCommLayer

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
//
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_SSLIIOPCommLayer_H_
#define _TIDORB_core_comm_SSLIIOPCommLayer_H_ 1


#include "TIDSocket.h"
#include "CORBA.h"
#include "FT.h"


namespace TIDorb {
namespace core {
namespace comm {

class SSLIIOPCommLayer : virtual public IIOPCommLayer
{

private:

  /**
   * Server Socket listening thread.
   */
  SSLServerListener_ref ssl_server_listener;


public:

  SSLIIOPCommLayer(TIDorb::core::TIDORB* orb)
    throw (TIDThr::SystemException);

  bool is_initialized() { return !server_listener.is_null();}

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
   * SSLIOP Layer close.
   */

  bool is_local(const TIDorb::core::iop::IOR& ior) const;
    
  bool has_server_listener() {return !server_listener.is_null();}

  void destroy();

  void init_server_listener();

  TIDorb::core::iop::IOR* createIOR(const char* id, TIDorb::core::poa::POAKey* key, 
                                    const TIDorb::core::iop::VectorTaggedComponent& extraComponents);

protected:

  /**
   * Sends a request (with response) allocating an active connection with
   * the corresponding server referenced by its target address.
   * <p>If necessary, tries to do a recovery loop.
   * @param request the CORBA request.
   */
  
  void send_request(TIDorb::core::RequestImpl* request, TIDorb::core::iop::IOR* ior)
    throw(TIDorb::core::ForwardRequest,CORBA::SystemException);


  //
  // Common SSL configuration methods
  //

public:

/*   get_ssl_miininum_configuration(); */

/*   bool check_ssl_mininum_configuration(const TIDorb::core::ConfORB& conf,  */
/*                                        Security::AssociationOption); */

  static bool check_certificate(const char* ssl_certificate,
                                const char* ssl_private_key,
                                const char* ssl_ca);
};

}
}
}

#endif

