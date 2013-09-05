//////////////////////////////////////////////////////////////////////////////////
//
// File:        SSLServerListener.C
// Description: 
//
// Rel:         01.00
// Created:     Jun, 2009
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


#undef ERROR




TIDorb::core::comm::SSLServerListener::SSLServerListener(ConnectionManager* the_manager,
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
    // Init OpenSSL Library
    TIDSocket::ssl_init();

    // TODO: store reference to SSLContext??
    TIDSocket::SSLContext* sc = 
      TIDSocket::SSLContext::getInstance((TIDSocket::SSLProtocol) conf.ssl_version);

    // Initialize SSL Context conforms ORB configuration
    if ( (strcmp(conf.ssl_private_key, "") != 0) && 
         (strcmp(conf.ssl_certificate, "") != 0) )

      try {
        sc->init(conf.ssl_session_timeout, 
                 conf.ssl_ca, conf.ssl_certificate, conf.ssl_private_key);
      } catch (const TIDSocket::KeyManagementException& ex) {
        _orb->print_trace(TIDorb::util::TR_ERROR, ex.getReason());
        throw CORBA::INITIALIZE(ex.getReason());
      }
    
    TIDSocket::SSLServerSocketFactory* ssf = sc->getServerSocketFactory();
    if (conf.ip_address == NULL){
      server_socket = ssf->createServerSocket();
      server_socket->setReuseAddress(true);
      try{
        TIDSocket::InetSocketAddress inet(conf.ssl_port);
        server_socket->bind((TIDSocket::SocketAddress*) &inet, conf.server_socket_backlog);
      }
      catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
    else {
      inet = TIDSocket::InetAddress::getByName(conf.ip_address);
      server_socket = ssf->createServerSocket();
      server_socket->setReuseAddress(true);
      try {
        TIDSocket::InetSocketAddress addr(conf.ssl_port);
        if (inet) 
          addr = TIDSocket::InetSocketAddress(inet, conf.ssl_port);
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
    //listen_point._ssl_port = server_socket->getLocalPort();

  } catch (const TIDSocket::IOException& e) {

    delete[] localhost;
    delete server_socket;
    delete inet;
    throw CORBA::INITIALIZE("Cannot open SSLServerSocket");
  }
}




TIDorb::core::comm::SSLServerListener::~SSLServerListener()
  throw (TIDThr::SystemException)
{
  if(server_socket)
    delete server_socket;
}




const TIDorb::core::comm::iiop::ListenPoint&
TIDorb::core::comm::SSLServerListener::get_listen_point()
{
  return listen_point;
}




void TIDorb::core::comm::SSLServerListener::shutdown()
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
      msg << "SSLServerListener at " << listen_point.toString() << " shutdown!";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  } catch(...) {
    if (_orb->trace != NULL){
      TIDorb::util::StringBuffer msg;
      msg << "SSLServerListener at " << listen_point.toString() << " shutdown!";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data(),CORBA::UNKNOWN());
    }
  }

}




void TIDorb::core::comm::SSLServerListener::reset_server_socket()
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
    // TODO: store reference to SSLContext??
    //TIDSocket::SSLContext* sc = TIDSocket::SSLContext::getInstance(TIDSocket::SSLv3);
    TIDSocket::SSLContext* sc = 
      TIDSocket::SSLContext::getInstance( (TIDSocket::SSLProtocol) conf.ssl_version);
    
    if ( (strcmp(conf.ssl_private_key, "") != 0) && 
         (strcmp(conf.ssl_certificate, "") != 0) ) 
      try {
        sc->init(conf.ssl_session_timeout, 
                 conf.ssl_ca, conf.ssl_certificate, conf.ssl_private_key);
      } catch (const TIDSocket::KeyManagementException& ex) {
        _orb->print_trace(TIDorb::util::TR_ERROR, ex.getReason());
        throw CORBA::INITIALIZE(ex.getReason());
      }

    TIDSocket::SSLServerSocketFactory* ssf = sc->getServerSocketFactory();

    if(conf.ip_address == NULL) {
      server_socket = ssf->createServerSocket();
      server_socket->setReuseAddress(true);
      try{
        TIDSocket::InetSocketAddress inet(listen_point._port);
        //TIDSocket::InetSocketAddress inet(listen_point._ssl_port);
        server_socket->bind((TIDSocket::SocketAddress*) &inet, conf.server_socket_backlog);
      }
      catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
    else {
      inet = TIDSocket::InetAddress::getByName(conf.ip_address);
      server_socket = ssf->createServerSocket();
      server_socket->setReuseAddress(true);
      try {
        TIDSocket::InetSocketAddress addr(conf.ssl_port);
        if (inet) 
          addr = TIDSocket::InetSocketAddress(inet, conf.ssl_port);
        server_socket->bind((TIDSocket::SocketAddress*) &addr, conf.server_socket_backlog);
      } catch(TIDSocket::IllegalArgumentException& e) {
        throw TIDSocket::IOException(e.what());
      }
    }
  } catch (...) {
    delete inet;
    throw  CORBA::INITIALIZE("SSLServerListener::reset_server_socket(): Cannot restore SSLServerSocket ");
  }

  delete inet;
}




void TIDorb::core::comm::SSLServerListener::run()
{
  bool connected = true;
  // bool shutdown = false;

  while (!do_shutdown){

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer buffer;
      buffer << "Accepting SSL connections at " << listen_point.toString() << "..." << ends;
      _orb->print_trace(TIDorb::util::TR_DEBUG, buffer.str().data());
    }

    while (connected) {

      TIDSocket::Socket* client_socket = NULL; // TIDSocket::SSLSocket

      try {

        if (server_socket != NULL)
          client_socket = server_socket->accept();
        else
          connected = false;

      } catch (const TIDSocket::SocketException& e) {
        connected = false;
        if (_orb->trace != NULL) {
          _orb->print_trace(TIDorb::util::TR_ERROR, "SSLServerSocket closed");
        }

      } catch (const TIDSocket::IOException& e) {
        connected = false;
        if (_orb->trace != NULL) {
          _orb->print_trace(TIDorb::util::TR_ERROR, "SSLServerSocket closed", 
                            CORBA::UNKNOWN(e.getReason() ));
        }

      } catch (...) {
        connected = false;
        if (_orb->trace != NULL) {
          _orb->print_trace(TIDorb::util::TR_ERROR, "Error in SSLServerSocket: ",
                            CORBA::UNKNOWN());
        }
      }

      try {
        if(client_socket != NULL)
          //manager->createSSLServerConnection(TIDSocket::SSLSocket*)client_socket);
          manager->createSSLServerConnection(dynamic_cast<TIDSocket::SSLSocket*>(client_socket));

      } catch (...) {
        delete client_socket;
        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_ERROR, "Error creating SSLServerConnection");
        }
      }
    }

    if(!do_shutdown) {

      // RECONNECT
      if (_orb->trace != NULL){
        _orb->print_trace(TIDorb::util::TR_DEBUG, "Trying to reconnect SSLServerSocket");
      }
      try {

        reset_server_socket();
        connected = true;

        if (_orb->trace != NULL){
          _orb->print_trace(TIDorb::util::TR_DEBUG, "SSLServerSocket reconnected ");
        }

      } catch (...)
        {
          if (_orb->trace != NULL){
            _orb->print_trace(TIDorb::util::TR_DEBUG, "SSLServerSocket reconnect error: ",
                              CORBA::UNKNOWN());
          }

          try {
            Thread::sleep(_orb->conf().server_socket_reconnect);
          } catch (...) {}

        }
    }
  }
}



