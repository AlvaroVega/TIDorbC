//////////////////////////////////////////////////////////////////////////////////
//
// File:        MulticastConnection.C
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




TIDorb::core::comm::MulticastConnection::MulticastConnection(ConnectionManager* mngr, 
                                                             TIDSocket::MulticastSocket* s,
                                                             const char* _outgoing_interface)
  throw (TIDThr::SystemException, CORBA::SystemException)
  : TIDorb::core::comm::UDPConnection(mngr, s)
{
  TIDSocket::InetAddress* outgoing_ip = NULL;

  try {
    // set time-to-live
    s->setTimeToLive(conf.socket_time_to_live);
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << " MulticastConnection: time to live: " <<conf.socket_time_to_live ;
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

    // Check if interface for outgoing MIOP messages has been defined...
    if (_outgoing_interface) { // As a propietary MIOP Corbaloc
      outgoing_ip = TIDSocket::InetAddress::getByName(_outgoing_interface);
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << " MulticastConnection: outgoing_ip " << _outgoing_interface;
        msg << " defined in a propietary MIOP Corbaloc";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      } 
      s->setInterface(*outgoing_ip);
      delete outgoing_ip;
    } else { 
      
      const char* outgoing_ip_name = conf.multicast_outgoing_interface;
      if (outgoing_ip_name) { // As an ORB init param
        outgoing_ip = TIDSocket::InetAddress::getByName(outgoing_ip_name);
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << " MulticastConnection: outgoing_ip " << outgoing_ip_name;
          msg << " defined in an ORB init param";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        } 
        s->setInterface(*outgoing_ip);
        delete outgoing_ip;
      } 
    }

  } catch(const TIDSocket::Exception& e) {
    delete outgoing_ip;
    throw CORBA::OBJECT_NOT_EXIST(e.what(), 0, CORBA::COMPLETED_NO);
  }
}
  



TIDorb::core::comm::MulticastConnection::~MulticastConnection() throw (TIDThr::SystemException)
{
}




const char* TIDorb::core::comm::MulticastConnection::toString()
{

  if (str == NULL) {

    TIDThr::Synchronized sync(*this); 

    if (str != NULL) // May be changed after return of synchro
      return str;

    TIDorb::util::StringBuffer buffer;
    switch(mode) {
      case CLIENT_MODE:
        buffer << "Multicast client" << " thread(" << TIDThr::Thread::getCurrentThreadId() << ")";
        break;
      case SERVER_MODE:
        buffer << "Multicast server" << " thread(" << TIDThr::Thread::getCurrentThreadId() << ")";
        break;
      default:
        buffer << "Invalid multicast" << " thread(" << TIDThr::Thread::getCurrentThreadId() << ")"; /*unreachable*/
    }
    //PRA
    //buffer << " connection at " << my_port; /*pra@tid.es socket->getLocalPort();*/
    //    buffer << " connection at " << socket->getLocalPort();
    buffer << " connection " << hashCode() << ends;
    //EPRA

    str = CORBA::string_dup(buffer.str().data());
  }

  return str;
}




/**
 * Creates a new connection in SERVER_MODE.
 */
TIDorb::core::comm::Connection*
TIDorb::core::comm::MulticastConnection::server_connection(TIDorb::core::comm::ConnectionManager* mngr,
                                                           const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  TIDSocket::MulticastSocket* socket  = NULL;
  TIDSocket::InetAddress* incoming_ip = NULL;
  TIDSocket::NetworkInterface* netIf  = NULL;


  const TIDorb::core::ConfORB& conf = mngr->orb()->conf();
  try {
    // create the multicast socket
    //socket = new TIDSocket::MulticastSocket(listen_point._port); // FIX

    // resolve multicast group address
    TIDSocket::InetSocketAddress addr(listen_point._host, listen_point._port);
    if (addr.isUnresolved() || !addr.getAddress().isMulticastAddress()) {
      //delete socket; // FIX
      throw CORBA::COMM_FAILURE("Invalid multicast group address", 0, CORBA::COMPLETED_NO);
    }
    socket = new TIDSocket::MulticastSocket(&addr); // FIX
 

    // Check if interface for incoming MIOP messages has been defined...
    if (listen_point._incoming_interface){ // As a propietary MIOP Corbaloc
      incoming_ip = TIDSocket::InetAddress::getByName(listen_point._incoming_interface);
      netIf = TIDSocket::NetworkInterface::getByInetAddress(*incoming_ip);
      //cerr << "MulticastConnection: incoming_ip " << listen_point._incoming_interface << endl;
      delete incoming_ip;
      incoming_ip = NULL;
    } else {
      const char* incoming_ip_name = conf.multicast_incoming_interface;
      if (incoming_ip_name) { // As a ORB init param
        incoming_ip = TIDSocket::InetAddress::getByName(incoming_ip_name);
        netIf = TIDSocket::NetworkInterface::getByInetAddress(*incoming_ip);
        //cerr << "MulticastConnection: incoming_ip " << incoming_ip_name << endl;
        delete incoming_ip;
        incoming_ip = NULL;
      } 
    }

    // join to multicast group    
    socket->joinGroup(addr, netIf);
    // cerr << "MulticastConnection::server_conection join to multicast group " << addr.getHostName();
    // cerr << " in "; cerr << netIf->getDisplayName() << " interface "<< endl;
    delete netIf;
    netIf = NULL;

    // create connection
    TIDorb::core::comm::Connection* conn = 
      new TIDorb::core::comm::MulticastConnection(mngr, socket, listen_point._outgoing_interface);
    conn->mode = SERVER_MODE;
    conn->init();

    return conn;

  } catch(const TIDSocket::Exception& e) {
    delete socket;
    delete netIf;
    delete incoming_ip;
    throw CORBA::COMM_FAILURE(e.what(), 0, CORBA::COMPLETED_NO);
  }
}




/**
 * Creates a new connection in CLIENT_MODE.
 */
TIDorb::core::comm::Connection*
TIDorb::core::comm::MulticastConnection::client_connection(TIDorb::core::comm::ConnectionManager* mngr,
                                                           const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  TIDSocket::MulticastSocket* socket  = NULL;

  try {
    // create the multicast socket
    socket = new TIDSocket::MulticastSocket();

    // resolve multicast group address
    TIDSocket::InetSocketAddress addr(listen_point._host, listen_point._port);
    if (addr.isUnresolved() || !addr.getAddress().isMulticastAddress()) {
      delete socket;
      throw CORBA::COMM_FAILURE("Invalid multicast group address", 0, CORBA::COMPLETED_NO);
    }

    // connect to multicast group
    // socket->connect(addr); UNNECESSARY

    // create connection
    TIDorb::core::comm::Connection* conn = 
      new TIDorb::core::comm::MulticastConnection(mngr, socket, listen_point._outgoing_interface);
    conn->mode = CLIENT_MODE;
    conn->initial_point = listen_point;

    return conn;

  } catch(const TIDSocket::Exception& e) {
    delete socket;
    throw CORBA::COMM_FAILURE(e.what(), 0, CORBA::COMPLETED_NO);
  }

  return NULL;
}
