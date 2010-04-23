/////////////////////////////////////////////////////////////////////////
//
// File:        ConnectionManager.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/ConnectionManager.h"




TIDorb::core::comm::ConnectionManager::ConnectionManager
  (TIDorb::core::TIDORB* orb,
   ::FT::HeartbeatPolicyValue& client_heartbeat_policy)
  throw (TIDThr::SystemException)
    : _orb(orb), 
      uncompleted_messages(orb->conf().max_uncompleted_messages),
      connections(orb->conf().max_connections)
{
  destroyed = false;

  // FT extensions
  _group  = NULL;
  _thread = NULL;

  if (client_heartbeat_policy.heartbeat) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    _group = new TIDThr::ThreadGroup(NULL, "", &attr);
    // it can be destroyed because ThreadGroup copies it inside the constructor
    pthread_attr_destroy(&attr);
    try {
      _thread = new HeartbeatThread(this, _group,
                                    client_heartbeat_policy.heartbeat_interval,
                                    client_heartbeat_policy.heartbeat_timeout);
      _thread->start();
    } catch(const exception& thw) {
      TIDorb::util::StringBuffer buffer;
      buffer << "Cannot create thread: " << thw.what();
      throw CORBA::NO_RESOURCES(buffer.str().data());
    }
  }
  // end FT extensions
  //  sas_manager = new TIDorb::core::security::sas::SASManager(orb);
}




TIDorb::core::comm::ConnectionManager::~ConnectionManager()
  throw (TIDThr::SystemException)
{
  // FT extensions
  delete _thread;
  // delete _group;
  // FT extensions

  if (!destroyed)
    this->destroy();

}




/**
 * Notifies the connection use.
 * @param conn the connection in use
 */
void TIDorb::core::comm::ConnectionManager::use(Connection* conn)
{
  connections.use(conn->get_descriptor());
}




/**
 * The manager is Notified a connection is been closing.
 * @param conn the connection that is closing.
 */
void TIDorb::core::comm::ConnectionManager::closing(Connection* conn)
{
  TIDThr::Synchronized synchro(*this);  
  if(destroyed)
    return;

  remove_listen_points(conn);
  TIDorb::core::comm::Connection_ref* connection = 
    connections.remove(conn->get_descriptor());
  //  (*connection)->_remove_ref(); 
  delete connection;
  
}



  
/**
 * When a new Socket is created (a new connection has been accepted by the
 * <code>ServerSocket</code>) a new Connection, in SERVER mode, must be registered.
 * @param socket the new socket.
 */
void TIDorb::core::comm::ConnectionManager::createServerConnection(TIDSocket::Socket* socket)
{  
  TIDThr::Synchronized synchro(*this);  
  if(destroyed) {
    // drop the socket
    try {
      socket->close();
    } catch (const TIDSocket::IOException& ioe) {
    } 
  } else { 
    // create the new connection and add the new connection to connection table
    new_connection(TCPConnection::server_connection(this, socket));
  }
}




/**
 * Saves the <code>ListenPoint</code> associated to a bidirectional connection to
 * be used as a client connection when a connection to the listen point will be required.
 * @param listen_point the <code>ListenPoint</code> where the connection will be seen as a client connection.
 * @param conn the bidirectional connection.
 */
void TIDorb::core::comm::ConnectionManager::add_bidirectional_connection
  (const TIDorb::core::comm::iiop::ListenPoint& listen_point, Connection* conn)
{
  TIDThr::Synchronized synchro(*this);
  if(destroyed)
    return;

  bidirectional_connections[listen_point]=(Connection_ref*) new Connection_ref(conn);

}
  
  
  
  
/**
 * Looks for a client connection with the listen point. If it does not exist, then the creates one.
 * @param listen_point the <code>ListenPoint</code> that determines a remote ORB in a Object reference.
 */
TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::getClientConnection
  (const TIDorb::core::comm::iiop::ListenPoint& listen_point, 
   TIDorb::core::PolicyContext* policy_context)
{ 
  // opened connection
  Connection_ref* conn =  0;
  
  // check if a connection is opening now
  OpeningLock_ref opening_lock;

  // this thread must open a connection and unlock the OpeningLock
  bool open_a_connection = false;
  
  // iterator for map connections: client, bidirectional
  ConnectionMapIteratorT connIt;

  {
    TIDThr::Synchronized synchro(*this);  
  
    if (destroyed) {
      throw CORBA::TRANSIENT("CommunicationLayer shutdown",0, CORBA::COMPLETED_NO);
    }

    //jagd cambia orden de llamadas para optimizar
    // looks for an existing client connection
    connIt = client_connections.find(listen_point);
    if (connIt != client_connections.end()){
      conn = (Connection_ref*)(*connIt).second;
      if (conn)
        return (Connection*) (*conn); 
    }
    
    // looks for an existing bidirectional connection
    connIt = bidirectional_connections.find(listen_point);
    if (connIt != bidirectional_connections.end()){
      conn = (Connection_ref*)(*connIt).second;
      if (conn)
        return (Connection*) (*conn); 

    }

    
    // check if is opening now
    ConnectionsOpening::iterator i = connections_opening.find(listen_point);
    
    if (i == connections_opening.end()) {
      opening_lock = new OpeningLock();
      connections_opening[listen_point] = opening_lock;
      open_a_connection = true;
    } else {
      opening_lock = i->second;
    }
  }
  
  Connection* client_conn = 0;
  if(! open_a_connection) {
    try {
      client_conn = opening_lock->wait_opening(_orb->conf().socket_connect_timeout);
    } catch (const CORBA::COMM_FAILURE& ce) {
      TIDThr::Synchronized synchro(*this);
      // Delete current lock to allow open new connection at the next retry.
      // Current lock was created at previous retry, which raised TRANSIENT
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "Socket connection timeout exceeded waiting to connecting with ";
        msg << listen_point.toString();
        msg << ". Released lock over it. ";
        _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
      }
      connections_opening.erase(listen_point);
      throw CORBA::TRANSIENT(ce.what());
    }
  } else {
    try {
      client_conn = open_client_connection(listen_point, policy_context);
    } catch (const CORBA::COMM_FAILURE& ce) {
      TIDThr::Synchronized synchro(*this);   
      opening_lock->set_error(ce);
      connections_opening.erase(listen_point);
      //PRA
      //throw ce;         
      throw;
    }
      
    {
      TIDThr::Synchronized synchro(*this);
      
      new_connection(client_conn);
      
      // add the new connection to the client_connections
      client_connections[listen_point]= (Connection_ref*) new Connection_ref(client_conn);
      
      
      opening_lock->set_opened(client_conn);
      connections_opening.erase(listen_point);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << client_conn->toString() << " open!";
        _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
      }
      
    }
  }
  return client_conn;
}




TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::open_client_connection
  (const TIDorb::core::comm::iiop::ListenPoint& listen_point,
   TIDorb::core::PolicyContext* policy_context)
{
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Opening client connection to " << listen_point.toString();
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
  
  // create the new connection
  Connection* conn = TCPConnection::client_connection(this, listen_point);

  BiDirPolicy::BidirectionalPolicy_var bidir = NULL;
  // is bidirectional ???
  if (policy_context != NULL) {
    CORBA::Policy_var pol =      
      policy_context->getPolicy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE);
    
    bidir = BiDirPolicy::BidirectionalPolicy::_narrow(pol);
  }
  
  IIOPCommLayer* comm_layer = _orb->getCommunicationManager()->getExternalLayer();

  if ( (bidir != NULL) && (bidir->value() == BiDirPolicy::BOTH) && 
       (comm_layer->has_server_listener())) {

    conn->set_bidirectional_mode(comm_layer->get_bidirectional_service());

  } else {
    TIDorb::core::poa::CurrentImpl* poa_current = 
      dynamic_cast<TIDorb::core::poa::CurrentImpl*>(_orb->init_POACurrent());
    
    
    if (poa_current->in_context()) { 

      try {
        PortableServer::POA_ptr aux_poa = poa_current->get_POA();
        //jagd
        TIDorb::core::poa::POAImpl* current_poa = dynamic_cast<TIDorb::core::poa::POAImpl*>(aux_poa);
        
        //  IIOPCommLayer* comm_layer = _orb->getCommunicationLayer()->getExternalLayer();
        //IIOPCommLayer* comm_layer = _orb->getCommunicationManager()->getExternalLayer();
        
        if(current_poa->isBidirectional())
          conn->set_bidirectional_mode(comm_layer->get_bidirectional_service());
      } catch(const PortableServer::Current::NoContext& nc) {
      }

      // add the new connection to connection table
    }
    
  }
  
  return conn;
}
  
  
  
 


TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::open_ssl_client_connection
  (const TIDorb::core::comm::iiop::ListenPoint& listen_point,
   TIDorb::core::PolicyContext* policy_context)
{
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Opening SSL client connection to " << listen_point.toString();
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
  
  // create the new connection
  Connection* conn = SSLConnection::client_connection(this, listen_point);

  BiDirPolicy::BidirectionalPolicy_var bidir = NULL;
  // is bidirectional ???
  if (policy_context != NULL) {
    CORBA::Policy_var pol =      
      policy_context->getPolicy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE);
    
    bidir = BiDirPolicy::BidirectionalPolicy::_narrow(pol);
  }
  
  IIOPCommLayer* comm_layer = _orb->getCommunicationManager()->getExternalLayer();

  if ( (bidir != NULL) && (bidir->value() == BiDirPolicy::BOTH) && 
       (comm_layer->has_server_listener())) {

    conn->set_bidirectional_mode(comm_layer->get_bidirectional_service());

  } else {
    TIDorb::core::poa::CurrentImpl* poa_current = 
      dynamic_cast<TIDorb::core::poa::CurrentImpl*>(_orb->init_POACurrent());
    
    
    if (poa_current->in_context()) { 

      try {
        PortableServer::POA_ptr aux_poa = poa_current->get_POA();

        TIDorb::core::poa::POAImpl* current_poa = dynamic_cast<TIDorb::core::poa::POAImpl*>(aux_poa);
        
        if(current_poa->isBidirectional())
          conn->set_bidirectional_mode(comm_layer->get_bidirectional_service());

        // TODO: current_poa will be only for SSL??
        

      } catch(const PortableServer::Current::NoContext& nc) {
      }

      // add the new connection to connection table
    }
    
   }
  
  return conn;
}

    
/**
 * Checks in a new connection for manage it.
 * @param conn the bidirectional connection.
 */
void TIDorb::core::comm::ConnectionManager::new_connection(Connection* conn)
{
  try {   
    connections.append(conn->get_descriptor(), new Connection_ref(conn));
  } catch (const TIDorb::core::util::FullUseTableException& f) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Maximum number of connetions reached";
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    conn->close_by_manager();
    if(conn->is_client_connection())
      throw CORBA::TRANSIENT("Maximum number of connections reached, and all are in use, try later!!",0,
                             CORBA::COMPLETED_NO);
  } 
    
  ConnectionTable::ObjectList* removed = connections.getRemovedObjects();

  if (removed != NULL) {

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Removing removed connections";
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    ConnectionTable::ObjectList::iterator i = removed->begin();
    while (i != removed->end()){
      if ( ((*i) != NULL) && (*(*i) != NULL)){
        close_connection(*(*i));
        delete (*i);
      }
      else
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, "Connection is NULL");
      i++;
    }
    removed->clear();
    delete removed;  
  }
}
  
  
  
  
/** 
 * Removes from the client connection table the listen points associted to a connection.
 * @param conn the connection.
 */
void TIDorb::core::comm::ConnectionManager::remove_listen_points(Connection* conn)
{
    
    const Connection::ListenPointSet& points = conn->get_listen_points();
//     cerr << "conn->addresses.points.size() " << conn->addresses.size() << endl;
//     cerr << "points.size() " << points.size() << endl;
    Connection::ListenPointSet::iterator it = points.begin();
    Connection* conn_aux = NULL;
    ConnectionMapIteratorT connIt;
    
    // remove listenPoints
    while(it != points.end())
    { 
           
      // remove from client_connections
      Connection_ref* c = NULL;
      c = client_connections[*it];
      client_connections.erase(*it);
      delete c;
      
      // remove from bidirectional_connections if ListenPoint refers 
      // to the given connection
      connIt = bidirectional_connections.find(*it);
      if (connIt != bidirectional_connections.end()){
        conn_aux = (Connection*) (*(*connIt).second);
        if (conn_aux == conn){  
          Connection_ref* c = NULL;
          c = bidirectional_connections[*it];
          bidirectional_connections.erase(*it);
          delete c;
        }
      }

      // remove from miop_connections if ListenPoint refers 
      // to the given connection
      connIt = miop_connections.find(*it);
      if (connIt != miop_connections.end()){
        conn_aux = (Connection*) (*(*connIt).second);
        if (conn_aux == conn){  
          Connection_ref* c = NULL;
          c = miop_connections[*it];
          miop_connections.erase(*it);
          delete c;
        }
      }
      
      it++;
    }
}
  
  
  
  
/**
 * Close the connection due to it has been decided that it not has been used for a long time.
 * @param conn the connection.
 */
void TIDorb::core::comm::ConnectionManager::close_connection(Connection* conn)
{
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << conn->toString() << " clossing by Connection Manager";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }
  remove_listen_points(conn);
  conn->close_by_manager();
}
  
  
  
  
/**
 * @return <code>true</code> if it has not any active connection
 */
bool TIDorb::core::comm::ConnectionManager::active_connections() 
{
  return (connections.getSize() > 0);
}
  
  
  
  
/**
 * Close all connections due to an ORB close session.
 */
void TIDorb::core::comm::ConnectionManager::destroy()
{
  TIDThr::Synchronized synchro(*this);  

  if (destroyed)
    return;

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << " Destroying connection manager";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }
  
  uncompleted_messages.destroy();  

  // Destroy connections of connectionsTable

  typedef vector<Connection_ref*> refVectorT;
  typedef refVectorT::iterator   refVectorIteratorT;   

  refVectorT* conns = connections.elements();
  Connection_ref* conn = NULL;
  refVectorIteratorT it;

  for (it = conns->begin(); it != conns->end(); it++){
    conn = (Connection_ref*) (*it);
    connections.remove((*conn)->get_descriptor());
    close_connection((Connection*) (*conn));
    delete conn; 
  }
  if (!conns->empty()){
    conns->clear();
  }
  delete conns;
  connections.clear();


//   { // Destroy connections of connections_opening table
//     ConnectionsOpening::iterator it = connections_opening.begin();
//     while (it != connections_opening.end()){
//       connections_opening.erase(it);
//       delete (*it).second;
//       it++;
//     }
//     connections_opening.clear();
//   }


  destroyed = true;

}




//
// pra@tid.es - MIOP extensions
//

TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::getMIOPClientConnection
  (const TIDorb::core::comm::miop::ListenPoint& listen_point)
{ 
  TIDThr::Synchronized synchro(*this);  

  Connection_ref* conn = NULL;

  if (destroyed)
    throw CORBA::TRANSIENT("CommunicationLayer shutdown", 0, CORBA::COMPLETED_NO);

  if (_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "trying getMIOPClientConnection at " << listen_point.toString();
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  ConnectionMapIteratorT connIt = client_connections.find(listen_point);
  if (connIt != client_connections.end()){
    conn = (Connection_ref*)(*connIt).second; 
    if (conn){
      if (_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "getMIOPClientConnection client_connection found";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
      return (Connection*) (*conn); 
    }
  }

  Connection* client_conn = open_miop_client_connection(listen_point);

  new_connection(client_conn);
  client_connections[listen_point]= (Connection_ref*)  new Connection_ref(client_conn);
      
  if (_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << client_conn->toString() << " open!";
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  return client_conn;
}




TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::getMIOPServerConnection
  (const TIDorb::core::comm::miop::ListenPoint& listen_point)
{ 
  TIDThr::Synchronized synchro(*this);  

  ConnectionMapIteratorT connIt;
  Connection_ref* conn = NULL;

  if (destroyed)
    throw CORBA::TRANSIENT("CommunicationLayer shutdown", 0, CORBA::COMPLETED_NO);
  
  if (_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << "trying getMIOPServerConnection at " << listen_point.toString();
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
  connIt = miop_connections.find(listen_point);
  if (connIt != miop_connections.end()){
    conn = (Connection_ref*)(*connIt).second;
    if (conn){
      if (_orb->trace != NULL){
        TIDorb::util::StringBuffer msg;
        msg << "getMIOPServerConnection server_connection found";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
      return (Connection*) (*conn); 
    }
  }

  Connection* serv_conn = open_miop_server_connection(listen_point);

  new_connection(serv_conn);
  miop_connections[listen_point]= (Connection_ref*) new Connection_ref(serv_conn);
      
  if (_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    msg << serv_conn->toString() << " open!";
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  return serv_conn;
}




TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::open_miop_client_connection(
  const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  bool is_multicast = false;
  try {
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "trying open_miop_client_connection at " << listen_point.toString();
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
    TIDSocket::InetSocketAddress inet(listen_point._host, listen_point._port);
    is_multicast = inet.getAddress().isMulticastAddress();
  } catch(...) {
  }

  if (_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    if (is_multicast) msg << "Connect to multicast group" << listen_point.toString();
    else msg << "Connect to unicast group" << listen_point.toString();
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
  
  // create the new connection
  if (is_multicast)
    return MulticastConnection::client_connection(this, listen_point);
  else
    return UDPConnection::client_connection(this, listen_point);
}




TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::open_miop_server_connection(
  const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  bool is_multicast = false;
  try {
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "trying open_miop_server_connection at " << listen_point.toString();
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
    TIDSocket::InetSocketAddress inet(listen_point._host, listen_point._port);
    is_multicast = inet.getAddress().isMulticastAddress();
  } catch(...) {
  }

  if (_orb->trace != NULL){
    TIDorb::util::StringBuffer msg;
    if (is_multicast) msg << "Join to multicast group " << listen_point.toString();
    else msg << "Joint to unicast group/Bind to " << listen_point.toString();
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
  
  // create the new connection
  if (is_multicast)
    return MulticastConnection::server_connection(this, listen_point);
  else
    return UDPConnection::server_connection(this, listen_point);
}



/**
 * When a new Socket is created (a new connection has been accepted by the
 * <code>ServerSocket</code>) a new Connection, in SERVER mode, must be registered.
 * @param socket the new socket.
 */
void TIDorb::core::comm::ConnectionManager::createSSLServerConnection(TIDSocket::SSLSocket* socket)
{  
  TIDThr::Synchronized synchro(*this);  
  if(destroyed) {
    // drop the socket
    try {
      ((TIDSocket::Socket*)socket)->close();
    } catch (const TIDSocket::IOException& ioe) {
    } 
  } else { 
    // create the new connection and add the new connection to connection table
    new_connection(SSLConnection::server_connection(this, socket));
  }
}



/**
 * Looks for a client connection with the listen point. If it does not exist, then the creates one.
 * @param listen_point the <code>ListenPoint</code> that determines a remote ORB in a Object reference.
 */
TIDorb::core::comm::Connection* 
TIDorb::core::comm::ConnectionManager::getSSLClientConnection
  (const TIDorb::core::comm::iiop::ListenPoint& listen_point, 
   TIDorb::core::PolicyContext* policy_context)
{ 
  // opened connection
  Connection_ref* conn =  0;
  
  // check if a connection is opening now
  OpeningLock_ref opening_lock;

  // this thread must open a connection and unlock the OpeningLock
  bool open_a_connection = false;
  
  // iterator for map connections: client, bidirectional
  ConnectionMapIteratorT connIt;

  {
    TIDThr::Synchronized synchro(*this);  
  
    if (destroyed) {
      throw CORBA::TRANSIENT("CommunicationLayer shutdown",0, CORBA::COMPLETED_NO);
    }

    // looks for an existing client connection
    connIt = ssl_client_connections.find(listen_point);
    if (connIt != ssl_client_connections.end()){
      conn = (Connection_ref*)(*connIt).second;
      if (conn)
        return (Connection*) (*conn); 
    }
    
//     ////////// -------------
//     // looks for an existing bidirectional connection
//     connIt = bidirectional_connections.find(listen_point);
//     if (connIt != bidirectional_connections.end()){
//       conn = (Connection_ref*)(*connIt).second;
//       if (conn)
//         return (Connection*) (*conn); 

//     }
//     ////////// -------------
    
    // check if is opening now
    ConnectionsOpening::iterator i = connections_opening.find(listen_point);
    
    if (i == connections_opening.end()) {
      opening_lock = new OpeningLock();
      connections_opening[listen_point] = opening_lock;
      open_a_connection = true;
    } else {
      opening_lock = i->second;
    }
  }
  
  Connection* ssl_client_conn = 0;
  if(! open_a_connection) {
    try {
      ssl_client_conn = opening_lock->wait_opening(_orb->conf().socket_connect_timeout);
    } catch (const CORBA::COMM_FAILURE& ce) {
      TIDThr::Synchronized synchro(*this);
      // Delete current lock to allow open new connection at the next retry.
      // Current lock was created at previous retry, which raised TRANSIENT
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "Socket connection timeout exceeded waiting to connecting with ";
        msg << listen_point.toString();
        msg << ". Released lock over it. ";
        _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
      }
      connections_opening.erase(listen_point);
      throw CORBA::TRANSIENT(ce.what());
    }
  } else {
    try {
      ssl_client_conn = open_ssl_client_connection(listen_point, policy_context);
    } catch (const CORBA::COMM_FAILURE& ce) {
      TIDThr::Synchronized synchro(*this);   
      opening_lock->set_error(ce);
      connections_opening.erase(listen_point);
      throw;
    }
      
    {
      TIDThr::Synchronized synchro(*this);
      
      new_connection(ssl_client_conn);
      
      // add the new connection to the client_connections
      ssl_client_connections[listen_point]= (Connection_ref*) new Connection_ref(ssl_client_conn);
      
      
      opening_lock->set_opened(ssl_client_conn);
      connections_opening.erase(listen_point);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << ssl_client_conn->toString() << " open!";
        _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
      }
      
    }
  }
  return ssl_client_conn;
}
