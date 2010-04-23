/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/SSLCommLayer

 Revisions:

 Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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

