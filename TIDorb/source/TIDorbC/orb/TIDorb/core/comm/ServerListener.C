//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServerListener.C
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

#include "TIDSocket.h"

#undef ERROR




TIDorb::core::comm::ServerListener::ServerListener(ConnectionManager* the_manager,
                                                   TIDThr::ThreadGroup* group)
  throw (TIDThr::SystemException, CORBA::SystemException)
  : TIDThr::Thread(group, NULL, ""), conf(the_manager->orb()->conf())
{
  do_shutdown   = false;
  server_socket = NULL;
  manager       = the_manager;
  _orb          = manager->orb();


  TIDSocket::InetAddress* inet = NULL;
  char* localhost = NULL;

  try {
    if (conf.ip_address == NULL){
      // Bug #343 Allow to reuse port
      server_socket = new TIDSocket::ServerSocket();
      server_socket->setReuseAddress(true);
      try{
        TIDSocket::InetSocketAddress inet(conf.orb_port);
        server_socket->bind((TIDSocket::SocketAddress*) &inet, conf.server_socket_backlog);
      }
      catch(TIDSocket::IllegalArgumentException& e) {
          throw TIDSocket::IOException(e.what());
      }
    }
    else {
      inet = TIDSocket::InetAddress::getByName(conf.ip_address);
      // Bug #343 Allow to reuse port
      server_socket = new TIDSocket::ServerSocket();
      server_socket->setReuseAddress(true);
      try {
        TIDSocket::InetSocketAddress addr(conf.orb_port);
        if (inet) 
          addr = TIDSocket::InetSocketAddress(inet, conf.orb_port);
        server_socket->bind((TIDSocket::SocketAddress*) &addr, conf.server_socket_backlog);
      } catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
      delete inet;
      inet = NULL;
    }

    server_socket->setReceiveBufferSize(conf.tcp_buffer_size);

    if(conf.host != NULL)
      listen_point = TIDorb::core::comm::iiop::ListenPoint(conf.host, server_socket->getLocalPort());
    else{
      inet = TIDSocket::InetAddress::getLocalHost();
      localhost = inet->getHostAddress();
      listen_point = TIDorb::core::comm::iiop::ListenPoint(localhost, server_socket->getLocalPort());
      delete[] localhost;
      delete inet;
      inet = NULL;
    }

  } catch (const TIDSocket::IOException& e) {

    delete[] localhost;
    delete server_socket;
    delete inet;
    throw CORBA::INITIALIZE("Cannot open ServerSocket");
  }
}




TIDorb::core::comm::ServerListener::~ServerListener()
  throw (TIDThr::SystemException)
{
  if(server_socket)
    delete server_socket;
}




const TIDorb::core::comm::iiop::ListenPoint&
  TIDorb::core::comm::ServerListener::get_listen_point()
{
  return listen_point;
}




void TIDorb::core::comm::ServerListener::shutdown()
{
  try {
    TIDThr::Synchronized synchro(*this);
    if(!do_shutdown) {
      do_shutdown = true;
      server_socket->close();
      delete server_socket;
      server_socket = NULL;
    }
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "ServerListener at " << listen_point.toString() << " shutdown!";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  } catch(...) {
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "ServerListener at " << listen_point.toString() << " shutdown!";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(),CORBA::UNKNOWN());
    }
  }

}




void TIDorb::core::comm::ServerListener::reset_server_socket()
{
  try {
    if(server_socket != NULL) {
      server_socket->close();
      delete server_socket;
      server_socket = NULL;
    }
  } catch (...) {
  }

  TIDSocket::InetAddress* inet = NULL;

  try {
    if(conf.ip_address == NULL) {
      server_socket = // Bug #343 Allow to reuse port
        // new TIDSocket::ServerSocket(listen_point._port, _orb->conf().server_socket_backlog);
        new TIDSocket::ServerSocket();
      server_socket->setReuseAddress(true);
      try{
        TIDSocket::InetSocketAddress inet(listen_point._port);
        server_socket->bind((TIDSocket::SocketAddress*) &inet, conf.server_socket_backlog);
      }
      catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
    else {
      inet = TIDSocket::InetAddress::getByName(conf.ip_address);
      server_socket = // Bug #343 Allow to reuse port
        // new TIDSocket::ServerSocket(listen_point._port, _orb->conf().server_socket_backlog, inet);
        new TIDSocket::ServerSocket();
      server_socket->setReuseAddress(true);
      try {
        TIDSocket::InetSocketAddress addr(conf.orb_port);
        if (inet) 
          addr = TIDSocket::InetSocketAddress(inet, conf.orb_port);
        server_socket->bind((TIDSocket::SocketAddress*) &addr, conf.server_socket_backlog);
      } catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
  } catch (...) {
    delete inet;
    throw  CORBA::INITIALIZE("ServerListener::reset_server_socket(): Cannot restore ServerSocket ");
  }

  delete inet;
}




void TIDorb::core::comm::ServerListener::run()
 {
    bool connected = true;
    // bool shutdown = false;

    while (!do_shutdown){

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer buffer;
        buffer << "Accepting connections at " << listen_point.toString() << "..." << ends;
        _orb->print_trace(TIDorb::util::TR_DEBUG, buffer.str().data());
      }

      while (connected) {

        TIDSocket::Socket* client_socket = NULL;

        try {

          if (server_socket != NULL)
            client_socket = server_socket->accept();
          else
            connected = false;

        } catch (const TIDSocket::SocketException& e) {
          connected = false;
          if (_orb->trace != NULL) {
            _orb->print_trace(TIDorb::util::TR_ERROR, "ServerSocket closed");
          }

        } catch (...) {
          connected = false;
          if (_orb->trace != NULL) {
            _orb->print_trace(TIDorb::util::TR_ERROR, "Error in ServerSocket: ",
                              CORBA::UNKNOWN());
          }
        }

        try {
          if(client_socket != NULL)
            manager->createServerConnection(client_socket);

        } catch (...) {

          delete client_socket;

          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_ERROR, "Error creating ServerConnection");
          }
        }
      }

      if(!do_shutdown) {

        // RECONNECT
        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_DEBUG, "Trying to reconnect ServerSocket");
        }
        try {

          reset_server_socket();
          connected = true;

          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_DEBUG, "ServerSocket reconnected ");
          }

        } catch (...)
        {
          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_DEBUG, "ServerSocket reconnect error: ",
                              CORBA::UNKNOWN());
          }

          try {
            Thread::sleep(conf.server_socket_reconnect);
          } catch (...) {}

        }
      }
    }
}



