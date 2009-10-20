//////////////////////////////////////////////////////////////////////////////////
//
// File:        IIOPCommLayer.C
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


#include "TIDSocket.h"


#undef ERROR




TIDorb::core::comm::IIOPCommLayer::IIOPCommLayer(TIDorb::core::TIDORB* orb)
  throw (TIDThr::SystemException)
  : CommunicationLayer(orb)
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  _group = new TIDThr::ThreadGroup(NULL, "", &attr);
//FRAN
  // it can be destroyed because ThreadGroup copies it inside the constructor
  pthread_attr_destroy(&attr);
//EFRAN

  destroyed = false;
  bidirectional_service = NULL;
  server_listener = NULL;

  // pra@tid.es - FT extensions
  server_heartbeat_enabled = false;
  client_heartbeat_policy.heartbeat = false;

  CORBA::Policy_var policy;
  const TIDorb::core::PolicyContext& policy_context = 
    _orb->getPolicyManager()->getPolicyContext();
  policy = policy_context.getPolicy(::FT::HEARTBEAT_ENABLED_POLICY);

  if ((policy)) {
    ::FT::HeartbeatEnabledPolicy_var heartbeat = ::FT::HeartbeatEnabledPolicy::_narrow(policy);
    server_heartbeat_enabled = heartbeat->heartbeat_enabled_policy_value();
  }
  policy = policy_context.getPolicy(::FT::HEARTBEAT_POLICY);

  if ((policy)) {
    ::FT::HeartbeatPolicy_var heartbeat = ::FT::HeartbeatPolicy::_narrow(policy);
    client_heartbeat_policy = heartbeat->heartbeat_policy_value();
  }
  // end FT extensions

  connection_manager = new TIDorb::core::comm::ConnectionManager(orb, client_heartbeat_policy);

  max_recover_count = conf.max_comm_recovering_times;
  recover_time = conf.comm_recover_time;
}




bool TIDorb::core::comm::IIOPCommLayer::is_local(const TIDorb::core::iop::IOR& ior) const
{
  if(server_listener.is_null()) // no object adapter initialized
    return false;

  return (server_listener->get_listen_point() 
          == ior.profile_IIOP()->getListenPoint());
}




void TIDorb::core::comm::IIOPCommLayer::getListenPoint(TIDorb::core::iop::IOR* ior,
                                                       TIDorb::core::comm::iiop::ListenPoint& listen_point)
  throw(CORBA::SystemException)
{
  TIDorb::core::comm::iiop::ProfileIIOP* profile = ior->profile_IIOP();

  if (profile == NULL)
    throw CORBA::INTERNAL("Cannot get IIOP Profile.");

  // Choose listen point to send message to
  if (conf.fault_tolerant)
    listen_point = profile->getAnyListenPoint();
  else
    listen_point = profile->getListenPoint();

  // pra@tid.es - FT extensions
  if (client_heartbeat_policy.heartbeat) {
    const TIDorb::core::iop::VectorTaggedComponent& components = profile->getTaggedComponents();
    for (size_t i = 0; i < components.size(); i++) {
      TIDorb::core::iop::TaggedComponent* comp = components[i];
      if (comp->_tag == TIDorb::core::iop::TAG_FT_HEARTBEAT_ENABLED) {
        //jagd
        //TIDorb::core::comm::FT::HeartbeatEnabled* heartbeat =
        //  dynamic_cast<TIDorb::core::comm::FT::HeartbeatEnabled*>(comp);
        TIDorb::core::comm::FT::HeartbeatEnabled* heartbeat =
          (TIDorb::core::comm::FT::HeartbeatEnabled*)(comp);
        listen_point._heartbeat_enabled = heartbeat->heartbeat_enabled;
      }
    }
  }
  // end FT extensions
}




/**
 * Sends a request (with response) allocating an active connection with
 * the corresponding server referenced by its target address.
 * <p>If necessary, tries to do a recovery loop.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::IIOPCommLayer::send_request(TIDorb::core::RequestImpl* request, 
                                                     TIDorb::core::iop::IOR* ior)
  throw(TIDorb::core::ForwardRequest, CORBA::SystemException)
{

  TIDorb::core::PolicyContext* policy_context = request->getPolicyContext();
/*
  TIDorb::core::comm::iiop::ProfileIIOP* profile = ior.profile_IIOP();

  if (profile == NULL)
    throw CORBA::INTERNAL("Cannot get IIOP Profile.");
*/
  Connection_ref conn = NULL;
  int recover_count = max_recover_count;

  // recovering loop

  while (true) {

    try {

      TIDorb::core::comm::iiop::ListenPoint point;
      getListenPoint(ior, point);

      conn = connection_manager->getClientConnection(point, policy_context);
      conn->send_request(request,ior);
      return;

    } catch(const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& rcf) {
      recover_count--;
      if(recover_count <= 0) {
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "Cannot recover the communication any more: ";
          _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(),rcf.comm_failure);
        }
        rcf.comm_failure._raise();
      }

      if (_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "Communication recovered from: ";
        _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(), rcf.comm_failure);
      }

      TIDThr::Thread::sleep(recover_time);
      
    } catch (const CORBA::TRANSIENT& tr) {
      // Add feature [#344] Support for corbalocs with more than one address and protocol
      // TRANSIENT catched: try to use another IIOP profile 
      if (ior->use_another_Profile_IIOP()) {
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "TRANSIENT in remote invocation. Trying connect with another IIOP Profile... ";
          _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
        }
        // continues bucle
      } else {
        if (_orb->trace != NULL) {
          _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in remote invocation", tr);
        }
        tr._raise(); 
      }
    } catch (const CORBA::SystemException& se) {
      if (_orb->trace != NULL) {
        _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in remote invocation", se);
      }
      se._raise();

    } catch (const TIDorb::core::ForwardRequest&) {
      throw;

    } catch (const exception& re) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in remote invocation", re);
      }
      //throw (re);
      throw CORBA::INTERNAL(re.what());
    }

  } //while
}




/**
 * Sends a request using the IIOP protocol.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::IIOPCommLayer::request(TIDorb::core::RequestImpl* request,
                                                TIDorb::core::iop::IOR* ior)
  throw(TIDorb::core::ForwardRequest, CORBA::SystemException)
{
  request->with_response(true);
  send_request(request, ior);
}




/**
 * Sends a oneway request using the IIOP protocol allocating an active Connection
 * with the server referenced in the request target address.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::IIOPCommLayer::oneway_request(TIDorb::core::RequestImpl* request,
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
/*

      TIDorb::core::comm::iiop::ProfileIIOP* profile = ior.profile_IIOP();

      if (profile == NULL)
        throw CORBA::INTERNAL("Cannot get IIOP Profile.");
*/
      Connection* conn=NULL;

      //TIDorb::core::comm::iiop::ListenPoint point = profile->getListenPoint();
      TIDorb::core::comm::iiop::ListenPoint point;
      getListenPoint(ior, point);

      // Unnecesary since this is a oneway
      // TIDorb::core::PolicyContext* policy_context request->getPolicyContext();
      TIDorb::core::PolicyContext* policy_context = NULL; 
      conn = connection_manager->getClientConnection(point, policy_context);

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
 * Sends a oneway request using the IIOP protocol allocating an active Connection
 * with the server referenced in the request target address.
 * @param request the CORBA request.
 */
void TIDorb::core::comm::IIOPCommLayer::reliable_oneway_run(TIDorb::core::RequestImpl* request,
                                                            TIDorb::core::iop::IOR* ior)
{
  try {
    request->reliable_oneway(true);

    request->with_response(false);
/*

    TIDorb::core::comm::iiop::ProfileIIOP* profile = ior.profile_IIOP();

    if (profile == NULL)
      throw CORBA::INTERNAL("Cannot get IIOP Profile.");
*/
    Connection_ref conn=NULL;

    int recover_count = max_recover_count;

    // recovering loop

    while (true) {

      try {

        //TIDorb::core::comm::iiop::ListenPoint point = profile->getListenPoint();
        TIDorb::core::comm::iiop::ListenPoint point;
        getListenPoint(ior, point);
        
        // Unnecesary since this is a oneway
        // TIDorb::core::PolicyContext* policy_context request->getPolicyContext();
        TIDorb::core::PolicyContext* policy_context = NULL; 

        conn = connection_manager->getClientConnection(point, policy_context);

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
TIDorb::core::comm::IIOPCommLayer::object_exists(TIDorb::core::iop::IOR* ior,
                                                 const TIDorb::core::PolicyContext& policy_context)
  throw(TIDorb::core::ForwardRequest,CORBA::SystemException)
{
/*
  TIDorb::core::comm::iiop::ProfileIIOP* profile = ior.profile_IIOP();

  if (profile == NULL)
    throw CORBA::INTERNAL("Cannot get IIOP Profile.");
*/
  Connection_ref conn = NULL;

  int recover_count = max_recover_count;

  // recovering loop

  while (true) {

    try {
      //TIDorb::core::comm::iiop::ListenPoint point = profile->getListenPoint();
      TIDorb::core::comm::iiop::ListenPoint point;
      getListenPoint(ior, point);

      conn = connection_manager->getClientConnection(point, 
                                                     (TIDorb::core::PolicyContext*)&policy_context);

      return conn->send_locate_request(ior, policy_context);

    } catch (const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& rcf) {

      recover_count--;

      if(recover_count <= 0) {

        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_DEBUG, "Cannot recover the communication any more: ",rcf);
        }

        rcf.comm_failure._raise();
      }
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_DEBUG, "Cannot recover the communication any more: ",rcf);
      }

      TIDThr::Thread::sleep(recover_time);

    } catch (const TIDorb::core::ForwardRequest&) {
      throw;

    } catch (const CORBA::SystemException& se) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in remote invocation", se);
      }
      //throw se;
      se._raise();

    } catch (const exception& re) {
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_ERROR, "Exception in remote invocation", re);
      }
      //throw re;
      throw CORBA::INTERNAL(re.what());
    }
  }
}




/**
 * ORB Server Connection part shutdown.
 */
void TIDorb::core::comm::IIOPCommLayer::shutdown()
{

  if (_orb->trace != NULL){
    _orb->print_trace(TIDorb::util::TR_ERROR, "Closing IIOPCommLayer and ServerListener");
  }
  try {
    TIDThr::Synchronized synchro(recursive_mutex);

    if(!server_listener.is_null())
      {
        server_listener->shutdown();
        server_listener = NULL;
      }
  } catch (const exception& e) {
  }
}




/**
 * IIOP Layer close.
 */
void TIDorb::core::comm::IIOPCommLayer::destroy()
{
  try {

    TIDThr::Synchronized sync(recursive_mutex);

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Destroing IIOPCommLayer... ";
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    shutdown();

    if(!connection_manager.is_null()) {
      connection_manager->destroy();
      connection_manager = NULL;
    }
  } catch (...) {}
}




void TIDorb::core::comm::IIOPCommLayer::init_server_listener()
{
  TIDThr::Synchronized sync(recursive_mutex);

  if(server_listener.is_null())
    {
      try {
        server_listener = new ServerListener(connection_manager, _group);
        server_listener->start();

      } catch(const exception& thw) {
        throw CORBA::NO_RESOURCES("Can't create thread ServerListener");
      }
    }
}




TIDorb::core::iop::IOR*
TIDorb::core::comm::IIOPCommLayer::createIOR(const char* id, TIDorb::core::poa::POAKey* key,
                                             const TIDorb::core::iop::VectorTaggedComponent& extraComponents)
{
  if(server_listener.is_null())
    throw CORBA::INTERNAL("ServerListener not initialized");

  TIDorb::core::iop::VectorTaggedProfile profiles;
  TIDorb::core::iop::VectorTaggedComponent components;
  size_t num_components = 1; //ORB_TYPE
  size_t extra_components_size = extraComponents.size();

  if (extra_components_size > 0) {
    components.resize(extra_components_size + num_components);
    for (size_t i = 0; i < extra_components_size; i++) {
      components[i] = extraComponents[i];
    }
    components[extra_components_size] = 
      new TIDorb::core::iop::ORBComponent(ConfORB::ORB_TYPE->orb_type);
  } else {
    components.resize(num_components);
    components[0] = new TIDorb::core::iop::ORBComponent(ConfORB::ORB_TYPE->orb_type);
  }

  // pra@tid.es - FT extensions
  if (server_heartbeat_enabled) {
    num_components++;
    components.resize(num_components);
    components[num_components - 1] = new TIDorb::core::comm::FT::HeartbeatEnabled(true);
  }

  size_t num_alternate_addresses = conf.alternate_addresses.size();
  if (num_alternate_addresses > 0) {
    components.resize(num_components + num_alternate_addresses);
    for (size_t i = 0; i < num_alternate_addresses; i++) {
      components[num_components + i] = new TIDorb::core::comm::FT::AlternateIIOPAddress(conf.alternate_addresses[i]);
      num_components++;
    }
  }
  // end FT extensions

  profiles.resize(1);
  profiles[0] = new TIDorb::core::comm::iiop::ProfileIIOP(
                       conf.GIOPVersion,
                       server_listener->get_listen_point(),
                       new TIDorb::core::comm::iiop::ObjectKey(key),
                       components);

  return new TIDorb::core::iop::IOR(id, &profiles);
}




TIDorb::core::comm::iiop::ServiceContextList* 
   TIDorb::core::comm::IIOPCommLayer::get_bidirectional_service()
{

  if(bidirectional_service == NULL) {

    TIDThr::Synchronized sync (recursive_mutex);

    if (bidirectional_service != NULL) // May be changed after return of synchro
      return bidirectional_service;

    if(server_listener.is_null())
      throw CORBA::INTERNAL("Trying to create a bidirectional context without ListenPoint");

    TIDorb::core::comm::iiop::BiDirServiceContext* bidir_context =
      new TIDorb::core::comm::iiop::BiDirServiceContext(1);

    bidir_context->listen_points.push_back(server_listener->get_listen_point());

    bidirectional_service = new TIDorb::core::comm::iiop::ServiceContextList(1);
    bidirectional_service->add(bidir_context);

  }

  return bidirectional_service;
}


/**
  * The layer can use this IOR to stablish a remote connection
  */
bool TIDorb::core::comm::IIOPCommLayer::accepts(const TIDorb::core::iop::IOR& ior)
{
    return ior.is_IIOP();  
}
    
    
/**
 * Creates a Delegate for a CORBA::Object
 */
TIDorb::core::ObjectDelegateImpl* 
    TIDorb::core::comm::IIOPCommLayer::createDelegate(TIDorb::core::iop::IOR* ior)
{
    if(accepts(*ior)) {
        return new TIDorb::core::ObjectDelegateImpl(_orb, ior, this);
    } else {
        throw CORBA::INV_OBJREF();
    }
}
