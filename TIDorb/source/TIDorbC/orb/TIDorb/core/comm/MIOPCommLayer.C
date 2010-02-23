//////////////////////////////////////////////////////////////////////////////////
//
// File:        MIOPCommLayer.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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



#undef ERROR




TIDorb::core::comm::MIOPCommLayer::MIOPCommLayer
  (TIDorb::core::TIDORB* orb,
   TIDorb::core::comm::ConnectionManager* conn_mngr)
  throw (TIDThr::SystemException)
  : TIDorb::core::comm::CommunicationLayer(orb)
{
  connection_manager = conn_mngr;
  destroyed = false;
}




bool TIDorb::core::comm::MIOPCommLayer::is_local(TIDorb::core::iop::IOR* ior)
{
  return false;
}




bool TIDorb::core::comm::MIOPCommLayer::is_multicast(TIDorb::core::iop::IOR* ior)
{
  return ior->is_group_reference();
}



bool TIDorb::core::comm::MIOPCommLayer::is_valid_synchronous_operation(const char* operation_name)
{
  return ((strcmp(operation_name, "_is_a") == 0) ||
          (strcmp(operation_name, "_non_existent") == 0) ||
          (strcmp(operation_name, "_validate_connection") == 0) ||
          (strcmp(operation_name, "_get_domain_managers") == 0) ||
          (strcmp(operation_name, "_get_interface") == 0) ||
          (strcmp(operation_name, "_is_nil") == 0) ||
          (strcmp(operation_name, "_is_equivalent") == 0) ||
          (strcmp(operation_name, "_hash") == 0) ||
          (strcmp(operation_name, "_create_request") == 0) ||
          (strcmp(operation_name, "_get_policy") == 0) ||
          (strcmp(operation_name, "_set_policy_overrides") == 0));
}




/**
 * Sends a request using the IIOP profile component of the MIOP profile.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::MIOPCommLayer::request(TIDorb::core::RequestImpl* request,
                                                TIDorb::core::iop::IOR* ior)
  throw(TIDorb::core::ForwardRequest, CORBA::SystemException)
{
  request->with_response(true);

  // Check for a CORBA::Object operation
  if (! is_valid_synchronous_operation(request->operation()))
    throw CORBA::BAD_OPERATION();

  TIDorb::core::comm::miop::ProfileMIOP* miop_profile = ior->profile_MIOP();
  TIDorb::core::comm::miop::GroupIIOPProfile* group_iiop_profile = NULL;

  // Look for a GroupIIOPProfile component in the MIOP profile
  const TIDorb::core::iop::VectorTaggedComponent& components = miop_profile->getTaggedComponents();
  TIDorb::core::iop::VectorTaggedComponent::const_iterator iter;
  for (iter = components.begin(); iter != components.end(); iter++) {
    TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
    if (comp->_tag == TIDorb::core::iop::TAG_GROUP_IIOP) {
      //jagd
      //group_iiop_profile = dynamic_cast<TIDorb::core::comm::miop::GroupIIOPProfile*>(comp);
      group_iiop_profile = (TIDorb::core::comm::miop::GroupIIOPProfile*)(comp);
    }
  }

  if (group_iiop_profile == NULL)
    throw CORBA::BAD_OPERATION();

  TIDorb::core::comm::iiop::ProfileIIOP& iiop_profile = group_iiop_profile->_iiop_profile;

  try {
    // Create a new IOR from group IIOP profile
    TIDorb::core::iop::VectorTaggedProfile profiles(1, new TIDorb::core::comm::iiop::ProfileIIOP(iiop_profile));
    TIDorb::core::iop::IOR_ref iiop_ior = new TIDorb::core::iop::IOR(ior->get_type_id(), &profiles);

    // Send request to external (IIOP) layer
    _orb->getCommunicationManager()->getExternalLayer()->request(request, iiop_ior);

  } catch (TIDThr::SystemException& e) {
    throw CORBA::INTERNAL(e.what());
  }
}




/**
 * Sends a oneway request using the MIOP protocol allocating an active Connection
 * with the server referenced in the request target address.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::MIOPCommLayer::oneway_request(TIDorb::core::RequestImpl* request,
                                                       TIDorb::core::iop::IOR* ior)
{
  try {
    request->with_response(false);


    TIDorb::core::comm::miop::ProfileMIOP* profile = ior->profile_MIOP();

    if (profile == NULL)
      throw CORBA::INTERNAL("Cannot get MIOP Profile.");

    Connection* conn = NULL;

    TIDorb::core::comm::miop::ListenPoint point = profile->getListenPoint();
    conn = connection_manager->getMIOPClientConnection(point);

    conn->send_oneway_request_async(request, ior);

  } catch (const CORBA::SystemException& se) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_DEBUG, "Exception in oneway remote invocation", se);
    }
  } catch (const exception& th) {
    if (_orb->trace != NULL){
      _orb->print_trace(TIDorb::util::TR_DEBUG, "Exception in oneway remote invocation" , th);
    }
  }
}




/**
 * Sends a object existence request.
 * @param ior the object IOR.
 */
bool 
TIDorb::core::comm::MIOPCommLayer::object_exists(TIDorb::core::iop::IOR* ior,
                                                 const TIDorb::core::PolicyContext& policy_context)
  throw(TIDorb::core::ForwardRequest,CORBA::SystemException)
{
  TIDorb::core::comm::miop::ProfileMIOP* miop_profile = ior->profile_MIOP();
  TIDorb::core::comm::miop::GroupIIOPProfile* group_iiop_profile = NULL;

  // Look for a GroupIIOPProfile component in the MIOP profile
  const TIDorb::core::iop::VectorTaggedComponent& components = miop_profile->getTaggedComponents();
  TIDorb::core::iop::VectorTaggedComponent::const_iterator iter;
  for (iter = components.begin(); iter != components.end(); iter++) {
    TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
    if (comp->_tag == TIDorb::core::iop::TAG_GROUP_IIOP) {
      //jagd
      //group_iiop_profile = dynamic_cast<TIDorb::core::comm::miop::GroupIIOPProfile*>(comp);
      group_iiop_profile = (TIDorb::core::comm::miop::GroupIIOPProfile*)(comp);
    }
  }

  if (group_iiop_profile == NULL)
    throw CORBA::BAD_OPERATION();

  TIDorb::core::comm::iiop::ProfileIIOP& iiop_profile = group_iiop_profile->_iiop_profile;

  try {
    // Create a new IOR from group IIOP profile
    TIDorb::core::iop::VectorTaggedProfile profiles(1, new TIDorb::core::comm::iiop::ProfileIIOP(iiop_profile));
    TIDorb::core::iop::IOR iiop_ior(ior->get_type_id(), &profiles);

    // Send request to external (IIOP) layer
    return _orb->getCommunicationManager()->getExternalLayer()->object_exists(&iiop_ior,
                                                                              policy_context);

  } catch (TIDThr::SystemException& e) {
    throw CORBA::INTERNAL(e.what());
  }

  return false;
}




/**
 * ORB Server Connection part shutdown.
 */
void TIDorb::core::comm::MIOPCommLayer::shutdown()
{
  if (_orb->trace != NULL){
    _orb->print_trace(TIDorb::util::TR_ERROR, "Closing MIOPCommLayer");
  }
}




/**
 * MIOP Layer close.
 */
void TIDorb::core::comm::MIOPCommLayer::destroy()
{
  try {

    TIDThr::Synchronized sync(recursive_mutex);

    shutdown();
//MLG el connection lo destruye la IIOPCommLayer yo solo decremento
//el contador    
/*
    if(!connection_manager.is_null()) {
      connection_manager->destroy();
      connection_manager = NULL;
    }
*/    
//EMLG          
   } catch (...) {
     }
}




void TIDorb::core::comm::MIOPCommLayer::join_group(const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  //TIDThr::Synchronized sync(recursive_mutex);
  connection_manager->getMIOPServerConnection(listen_point); // synchronized
}

/**
  * The layer can use this IOR to stablish a remote connection
  */
bool TIDorb::core::comm::MIOPCommLayer::accepts(const TIDorb::core::iop::IOR& ior)
{
    return ior.is_group_reference();     
}
    
    
/**
 * Creates a Delegate for a CORBA::Object
 */
TIDorb::core::ObjectDelegateImpl* 
    TIDorb::core::comm::MIOPCommLayer::createDelegate(TIDorb::core::iop::IOR* ior)
{
  return new TIDorb::core::ObjectDelegateImpl(_orb, ior, this);
}
