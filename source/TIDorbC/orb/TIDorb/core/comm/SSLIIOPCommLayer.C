//////////////////////////////////////////////////////////////////////////////////
//
// File:        SSLIIOPCommLayer.C
// Description: 
//
// Rel:         01.00
// Created:     June, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"


#include <exception>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


#include "TIDSocket.h"


#undef ERROR




TIDorb::core::comm::SSLIIOPCommLayer::SSLIIOPCommLayer(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
  : IIOPCommLayer(orb)
{
}


bool TIDorb::core::comm::SSLIIOPCommLayer::is_local(const TIDorb::core::iop::IOR& ior) const
{

  if(ssl_server_listener.is_null()) // no object adapter initialized
    return false;

  SSLIOP::SSL* ior_ssl_component = ior.profile_IIOP()->get_SSL();

  if (ior_ssl_component == NULL)
    return false;
  
  const TIDorb::core::comm::iiop::ListenPoint& server_listen_point = 
    ssl_server_listener->get_listen_point();

  const TIDorb::core::comm::iiop::ListenPoint& ior_listen_point =
    ior.profile_IIOP()->getListenPoint();

  return ( (strcmp(server_listen_point._host, ior_listen_point._host) == 0)
           && (server_listen_point._port == ior_ssl_component->port) );
}



/**
 * ORB Server Connection part shutdown.
 */
void TIDorb::core::comm::SSLIIOPCommLayer::shutdown()
{
  if (_orb->trace != NULL){
    _orb->print_trace(TIDorb::util::TR_ERROR, "Closing SSLIIOPCommLayer and SSLServerListener");
  }
  try {
    TIDThr::Synchronized synchro(recursive_mutex);

    if(!ssl_server_listener.is_null())
      {
        ssl_server_listener->shutdown();
        ssl_server_listener = NULL;
      }
  } catch (const exception& e) {
  }
  
  TIDorb::core::comm::IIOPCommLayer::shutdown();
}


void TIDorb::core::comm::SSLIIOPCommLayer::destroy()
{
  try {

    TIDThr::Synchronized sync(recursive_mutex);

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Destroing SSLIOPCommLayer... ";
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    TIDorb::core::comm::SSLIIOPCommLayer::shutdown();

//     if(!connection_manager.is_null()) {
//       connection_manager->destroy();
//       connection_manager = NULL;
//     }
  } catch (...) {}

  TIDorb::core::comm::IIOPCommLayer::destroy();
}



void TIDorb::core::comm::SSLIIOPCommLayer::init_server_listener()
{
  TIDThr::Synchronized sync(recursive_mutex);

  if(ssl_server_listener.is_null())
    {
      try {
        ssl_server_listener = new SSLServerListener(connection_manager, _group);
        ssl_server_listener->start();

      } catch(const exception& thw) {
        throw CORBA::NO_RESOURCES("Can't create thread SSLServerListener");
      }
    }

  // Also init IIOP server listener. It will always repond "NO_PERMISSION"
  TIDorb::core::comm::IIOPCommLayer::init_server_listener();
}


/**
 * Sends a request (with response) allocating an active connection with
 * the corresponding server referenced by its target address.
 * <p>If necessary, tries to do a recovery loop.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::SSLIIOPCommLayer::send_request(TIDorb::core::RequestImpl* request, 
                                                     TIDorb::core::iop::IOR* ior)
  throw(TIDorb::core::ForwardRequest, CORBA::SystemException)
{
  TIDorb::core::PolicyContext* policy_context = request->getPolicyContext();

  Connection_ref conn = NULL;
  int recover_count = max_recover_count;

  // recovering loop

  while (true) {

    try {

      TIDorb::core::comm::iiop::ListenPoint point;
      getListenPoint(ior, point);

      conn = connection_manager->getSSLClientConnection(point, policy_context);
      conn->send_request(request,ior);
      return;

    } catch(const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& rcf) {
      recover_count--;
      if(recover_count <= 0) {
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "SSLIOPCommLayer Cannot recover the communication any more: ";
          _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(),rcf.comm_failure);
        }
        rcf.comm_failure._raise();
      }

      if (_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "SSLIOPCommLayer Communication recovered from: ";
        _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(), rcf.comm_failure);
      }

      TIDThr::Thread::sleep(recover_time);
      
    } catch (const CORBA::TRANSIENT& tr) {
      // Add feature [#344] Support for corbalocs with more than one address and protocol
      // TRANSIENT catched: try to use another IIOP profile 
      if (ior->use_another_Profile_IIOP()) {
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "SSLIIOPCommLayer: TRANSIENT in remote invocation. Trying connect with another IIOP Profile... ";
          _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
        }
        // continues bucle
      } else {
        if (_orb->trace != NULL) {
          _orb->print_trace(TIDorb::util::TR_ERROR, "SSLIIOPCommLayer: TransientException in remote invocation", tr);
        }
        tr._raise(); 
      }
    } catch (const CORBA::SystemException& se) {
      if (_orb->trace != NULL) {
        _orb->print_trace(TIDorb::util::TR_ERROR, "SSLIIOPCommLayer: SystemException in remote invocation", se);
      }
      se._raise();

    } catch (const TIDorb::core::ForwardRequest&) {
      if (_orb->trace != NULL) {
        _orb->print_trace(TIDorb::util::TR_USER, "SSLIOPCommLayer: ForwardRequest in remote invocation");
      }
      throw;

    } catch (const exception& re) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_ERROR, "SSLIIOPCommLayer: Exception in remote invocation", re);
        _orb->print_trace(TIDorb::util::TR_ERROR, "SSLIOPCommLayer: raising INTERNAL ");
      }
      //throw (re);
      throw CORBA::INTERNAL(re.what());
    }

  } //while
}


/**
 * Sends a oneway request using the IIOP protocol allocating an active Connection
 * with the server referenced in the request target address.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::SSLIIOPCommLayer::oneway_request(TIDorb::core::RequestImpl* request,
                                                       TIDorb::core::iop::IOR* ior)
{
  if (conf.reliable_oneway) {
    try {
      ReliableOnewayThread* th = new ReliableOnewayThread(this, request, ior, _group);
      th->start();
      return;
    } catch(const exception& thw) {
      // MLG de momento...un saco de cemento
      //throw CORBA::NO_RESOURCES("Cannot create thread: " + thw.toString());
      throw CORBA::NO_RESOURCES("Cannot create thread");
    }
  } else {

    try {

      request->with_response(false);

      Connection* conn=NULL;

      //TIDorb::core::comm::iiop::ListenPoint point = profile->getListenPoint();
      TIDorb::core::comm::iiop::ListenPoint point;
      getListenPoint(ior, point);

      // Unnecesary since this is a oneway
      // TIDorb::core::PolicyContext* policy_context request->getPolicyContext();
      TIDorb::core::PolicyContext* policy_context = NULL; 
      conn = connection_manager->getSSLClientConnection(point, policy_context);

      conn->send_oneway_request_async(request, ior);

    } catch (const CORBA::SystemException& se) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_DEBUG, "SytemException in oneway remote invocation", se);
      }
    } catch (const exception& th) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_DEBUG, "Exception in oneway remote invocation" , th);
      }

    }
  }
}



/**
 * Sends a oneway request using the SSLIIOP protocol allocating an active Connection
 * with the server referenced in the request target address.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::SSLIIOPCommLayer::reliable_oneway_run(TIDorb::core::RequestImpl* request,
                                                            TIDorb::core::iop::IOR* ior)
{
  try {
    request->reliable_oneway(true);

    request->with_response(false);

    Connection_ref conn=NULL;

    int recover_count = max_recover_count;

    // recovering loop

    while (true) {

      try {

        TIDorb::core::comm::iiop::ListenPoint point;
        getListenPoint(ior, point);
        
        // Unnecesary since this is a oneway
        // TIDorb::core::PolicyContext* policy_context request->getPolicyContext();
        TIDorb::core::PolicyContext* policy_context = NULL; 

        conn = connection_manager->getSSLClientConnection(point, policy_context);

        conn->send_oneway_request_sync(request, ior);

        return;

      } catch (const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& rcf) {

        recover_count--;

        if(recover_count <= 0)
          rcf.comm_failure._raise();

        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_DEBUG, "Communication recovered: " , rcf.comm_failure);
        }

        TIDThr::Thread::sleep(recover_time);

      } catch (const TIDorb::core::ForwardRequest& fr) {
        ior = (TIDorb::core::iop::IOR*) (fr.reference());
        recover_count--;

        if (_orb->trace != NULL){
          TIDorb::util::StringBuffer msg;
          msg << "Communication forwarded: " << ior->toString();
          _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
        }

      }
    }

  } catch (const CORBA::SystemException& se) {

    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_DEBUG, "Exception in reliable remote oneway invocation :",se);
    }

  } catch (const exception& e) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_DEBUG, "Exception in reliable remote oneway invocation :",e);
    }
  }
}


/**
 * Sends a object existence request.
 * @param ior the object IOR.
 */
bool 
TIDorb::core::comm::SSLIIOPCommLayer::object_exists(TIDorb::core::iop::IOR* ior,
                                                    const TIDorb::core::PolicyContext& policy_context)
  throw(TIDorb::core::ForwardRequest,CORBA::SystemException)
{

  Connection_ref conn = NULL;

  int recover_count = max_recover_count;

  // recovering loop

  while (true) {

    try {
      TIDorb::core::comm::iiop::ListenPoint point;
      getListenPoint(ior, point);

      conn = connection_manager->getSSLClientConnection(point, 
                                                     (TIDorb::core::PolicyContext*)&policy_context);

      return conn->send_locate_request(ior, policy_context);

    } catch (const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& rcf) {

      recover_count--;

      if(recover_count <= 0) {

        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_DEBUG, 
                            "SSLIIOPCommLayer: Cannot recover the communication any more: ",rcf);
        }

        rcf.comm_failure._raise();
      }
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_DEBUG, 
                          "SSLIIOPCommLayer: Cannot recover the communication any more: ",rcf);
      }

      TIDThr::Thread::sleep(recover_time);

    } catch (const TIDorb::core::ForwardRequest&) {
      throw;

    } catch (const CORBA::SystemException& se) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_ERROR, 
                          "SSLIIOPCommLayer: Exception in remote invocation", se);
      }
      //throw se;
      se._raise();

    } catch (const exception& re) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_ERROR, 
                          "SSLIIOPCommLayer: Exception in remote invocation", re);
      }
      //throw re;
      throw CORBA::INTERNAL(re.what());
    }
  }
}



TIDorb::core::iop::IOR*
TIDorb::core::comm::SSLIIOPCommLayer::createIOR(const char* id, 
                       TIDorb::core::poa::POAKey* key,
                       const TIDorb::core::iop::VectorTaggedComponent& extraComponents)
{
  if(ssl_server_listener.is_null())
    throw CORBA::INTERNAL("SSLServerListener not initialized");

  TIDorb::core::iop::VectorTaggedComponent components;
  size_t num_components = 0;
  size_t extra_components_size = extraComponents.size();


  // TODO: Is this copy necessary?
  if (extra_components_size > 0) {
    components.resize(extra_components_size + num_components);
    for (size_t i = 0; i < extra_components_size; i++) {
      components[i] = extraComponents[i];
    }
  } else {
    components.resize(num_components);
  }

  // Add SSLComponent
  // TODO: fill SSLComponent following Configuration & ObjectPolicies
  num_components++;
  components.resize(num_components);

  SSLIOP::SSL ssl(CSIIOP::Integrity | CSIIOP::Confidentiality | CSIIOP::EstablishTrustInTarget | CSIIOP::NoDelegation,
                  CSIIOP::Integrity | CSIIOP::Confidentiality |  CSIIOP::NoDelegation,
                  ssl_server_listener->get_listen_point()._port);

  TIDorb::core::comm::ssliop::SSLComponent* ssl_component = 
    new TIDorb::core::comm::ssliop::SSLComponent(ssl);
  components[num_components - 1] = ssl_component;



  // Add CSIComponent
  if (conf.csiv2) {
    num_components++;
    components.resize(num_components);

    CSIIOP::CompoundSecMechList mech_list;

    mech_list.stateful = false;
    mech_list.mechanism_list.length(1);

    mech_list.mechanism_list[0].target_requires = 
      CSIIOP::Integrity | CSIIOP::Confidentiality;
    if ( (strcmp(conf.gssup_user, "") != 0) &&
         (strcmp(conf.gssup_password, "") != 0) )
      mech_list.mechanism_list[0].target_requires |= CSIIOP::EstablishTrustInClient;

    mech_list.mechanism_list[0].transport_mech.tag = IOP::TAG_TLS_SEC_TRANS;
    {
      TIDorb::core::cdr::CDROutputStream output(_orb);
      TIDorb::core::cdr::CDROutputStream* out = output.create_encapsulation();
      out->write_ushort(ssl_component->getTargetSupports());
      out->write_ushort(ssl_component->getTargetRequires());
      out->write_ulong(1);
      TIDorb::core::comm::iiop::ListenPointSet listenpoints = server_listener->get_listen_points();
      out->write_string((*(listenpoints.begin()))._host); 
      out->write_ushort(ssl_component->getSSLPort());    
      TIDorb::core::cdr::BufferCDR_ref buffer = out->get_buffer();
      CORBA::ULong length = buffer->get_available_bytes();
      mech_list.mechanism_list[0].transport_mech.component_data.length(length);
      TIDorb::core::cdr::ChunkCDR* chunk = buffer->get_chunk(0);
      for (int i=0; i < length; i++)
        mech_list.mechanism_list[0].transport_mech.component_data[i] = 
          chunk->get_buffer()[i];
      delete out;
    }
    
    //
    // AS Context: client authentication functionality that the target expects
    //
    mech_list.mechanism_list[0].as_context_mech.target_supports = 
      CSIIOP::Integrity | CSIIOP::Confidentiality;
    mech_list.mechanism_list[0].as_context_mech.target_requires = 
      CSIIOP::Integrity | CSIIOP::Confidentiality;

    if ( (strcmp(conf.gssup_user, "") != 0) &&
         (strcmp(conf.gssup_password, "") != 0) ) {
      mech_list.mechanism_list[0].as_context_mech.target_supports |= CSIIOP::EstablishTrustInClient;
      mech_list.mechanism_list[0].as_context_mech.target_requires |= CSIIOP::EstablishTrustInClient;
    }


    mech_list.mechanism_list[0].as_context_mech.client_authentication_mech.length(0);
    // Check for GSSUP::GSSUPMechOID
    if ( (strcmp(conf.gssup_user, "") != 0) &&
         (strcmp(conf.gssup_password, "") != 0) ) {
      CORBA::OctetSeq_var oid_buffer = 
        TIDorb::core::security::sas::SASManager::convert_oid_to_asn1(GSSUP::GSSUPMechOID);
       mech_list.mechanism_list[0].as_context_mech.client_authentication_mech.
        length(oid_buffer->length());
      //mech_list.mechanism_list[0].as_context_mech.client_authentication_mech = (*oid_buffer);
      for (CORBA::ULong i = 0; i < oid_buffer->length(); i++) 
        mech_list.mechanism_list[0].as_context_mech.client_authentication_mech[i] = 
          (*oid_buffer)[i];
    }

    mech_list.mechanism_list[0].as_context_mech.target_name.length(0);
    if ( strcmp(conf.csiv2_target_name, "") != 0) {
      CORBA::ULong target_name_length = strlen(conf.csiv2_target_name); 
      CORBA::OctetSeq_var oid_type = 
          TIDorb::core::security::sas::SASManager::convert_oid_to_asn1(CSI::GSS_NT_Export_Name_OID);

      CORBA::UShort oid_type_length = oid_type->length(); 
      CORBA::ULong i = 0;
      mech_list.mechanism_list[0].as_context_mech.target_name.length(
                                                                     2 + /* token_id */
                                                                     2 + /* oid_type_length*/  
                                                                     oid_type_length + /* oid_type*/
                                                                     4 + /* target_name_length */
                                                                     target_name_length /* target_name */
                                                                     );
      // Token_id
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x04;
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x01;

      // length of type OID (TODO: hardcoded!! CSI::GSS_NT_Export_Name_OID )
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x00; 
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x08;

      // OID
      for (int j = 0; j < oid_type_length; j++)
        mech_list.mechanism_list[0].as_context_mech.target_name[i++] = (*oid_type)[j];

      // length of target_name (TODO: hardcoded!! @tid.es )
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x00;
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x00;
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x00;
      mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 0x07;

      for (CORBA::ULong j = 0; j < target_name_length; j++) {
        mech_list.mechanism_list[0].as_context_mech.target_name[i++] = 
          (CORBA::Octet)conf.csiv2_target_name[j];
      }
    }
      
    //
    // SAS Context: Authorization mechanims
    //
    mech_list.mechanism_list[0].sas_context_mech.target_supports = CSIIOP::IdentityAssertion;
    mech_list.mechanism_list[0].sas_context_mech.target_requires = 0;

    
    mech_list.mechanism_list[0].sas_context_mech.privilege_authorities.length(0);

    mech_list.mechanism_list[0].sas_context_mech.supported_naming_mechanisms.length(0);

    // Check for GSSUP::GSSUPMechOID
    if ( (strcmp(conf.gssup_user, "") != 0) &&
         (strcmp(conf.gssup_password, "") != 0) ) {
      mech_list.mechanism_list[0].sas_context_mech.supported_naming_mechanisms.length(1);
      CORBA::OctetSeq_var oid_buffer = 
        TIDorb::core::security::sas::SASManager::convert_oid_to_asn1(GSSUP::GSSUPMechOID);
      //mech_list.mechanism_list[0].sas_context_mech.supported_naming_mechanisms[0] = (*oid_buffer);
      mech_list.mechanism_list[0].sas_context_mech.supported_naming_mechanisms[0].
        length(oid_buffer->length());
      for (CORBA::ULong i = 0; i < oid_buffer->length(); i++) 
        mech_list.mechanism_list[0].sas_context_mech.supported_naming_mechanisms[0][i] = 
          (*oid_buffer)[i];
    }

    mech_list.mechanism_list[0].sas_context_mech.supported_identity_types = 
      CSI::ITTAbsent;

    if ( (strcmp(conf.gssup_user, "") != 0) &&
         (strcmp(conf.gssup_password, "") != 0) ) {
      mech_list.mechanism_list[0].sas_context_mech.supported_identity_types |= 
        CSI::ITTPrincipalName;
    }

    components[num_components - 1] = 
      new TIDorb::core::security::CSIComponent(mech_list);
  }


  return TIDorb::core::comm::IIOPCommLayer::createIOR(id, key, components);
}




/**
  * The layer can use this IOR to stablish a remote connection
  */
bool TIDorb::core::comm::SSLIIOPCommLayer::accepts(const TIDorb::core::iop::IOR& ior)
{
  if (ior.is_IIOP()) {

    // TODO: check target_supports, target_requires values are compatibles with ORB config
    // NoProtection -> no ssl_options
    // Integrity    -> ssl_private_key, ssl_certificate
    // Confidentiality -> ssl_private_key, ssl_certificate
    // EstablishTrustInTarget -> ssl_private_key, ssl_certificate, ssl_ca


    SSLIOP::SSL* _ssl = (ior.profile_IIOP())->get_SSL();

    if (_ssl) {
      // Obtain a AssociationOptions from ORB configuration
      // TODO: put it on another place
      Security::AssociationOptions asoc_op = 0;
      if ( (strcmp(conf.ssl_private_key, "") != 0) && 
           (strcmp(conf.ssl_certificate, "") != 0) ) {
        asoc_op |= (Security::Integrity | Security::Confidentiality);
      }
      if (strcmp(conf.ssl_ca, "") != 0)
        asoc_op |= Security::EstablishTrustInClient | Security::EstablishTrustInTarget;

      return ((asoc_op & _ssl->target_requires) > 0);
    }
    else 
      return false; // ((asoc_op & 0) > 0)

    // Old way
    //   return (ior.profile_IIOP())->is_SSL() || ( (strcmp(conf.ssl_private_key, "") != 0) && 
    //                                              (strcmp(conf.ssl_certificate, "") != 0) );
   
  }
  else
    return false;
}
    
    
/**
 * Creates a Delegate for a CORBA::Object
 */
TIDorb::core::ObjectDelegateImpl* 
    TIDorb::core::comm::SSLIIOPCommLayer::createDelegate(TIDorb::core::iop::IOR* ior)
{
  return new TIDorb::core::ObjectDelegateImpl(_orb, ior, this);
}


bool TIDorb::core::comm::SSLIIOPCommLayer::check_certificate(const char* ssl_certificate,
                                                             const char* ssl_private_key,
                                                             const char* ssl_ca)

{
  // TO DO: check if ssl_certificate is signed by ssl_ca using TIDSocket SSL API
  
  return true;
}
