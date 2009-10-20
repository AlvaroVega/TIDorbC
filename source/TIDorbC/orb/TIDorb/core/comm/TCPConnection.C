//////////////////////////////////////////////////////////////////////////////////
//
// File:        TCPConnection.C
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
#include <memory>

#undef ERROR

TIDorb::core::comm::TCPConnection::TCPConnection(ConnectionManager* mngr,
                                                 TIDSocket::Socket* socketed)
  throw (TIDThr::SystemException, CORBA::SystemException)
  : TIDorb::core::comm::Connection(mngr), in(NULL), out(NULL)
{
  try {
    socket = socketed;

    //Cannot be used to identify connections in server mode
    //my_port = socket->getLocalPort();

    socket->setReuseAddress(true);

    socket->setTcpNoDelay(conf.tcp_nodelay);
    socket->setSendBufferSize(conf.tcp_buffer_size);
    socket->setReceiveBufferSize(conf.tcp_buffer_size);
    socket->setSoTimeout(conf.socket_timeout);
    socket->setSoLinger((conf.socket_linger != -1),conf.socket_linger);
    in = socket->getInputStream();
    out = socket->getOutputStream();
  } catch(const TIDSocket::IOException& ioe) {
    // throw CORBA::OBJECT_NOT_EXIST(ioe.what(),0, CORBA::COMPLETED_NO));
    throw CORBA::INTERNAL("IOException", 0, CORBA::COMPLETED_NO);
  }

   //jagd  
  _root_POA = dynamic_cast<TIDorb::core::poa::POAImpl*>(_orb->init_POA());
  _poamanager = _root_POA->the_POAManager();
}




TIDorb::core::comm::TCPConnection::~TCPConnection() throw (TIDThr::SystemException)
{
  delete in;
  delete out;
//FRAN
  delete socket;
//EFRAN

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Deleting TCPConnection";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

}




/**
 * Connection hash code.
 */
CORBA::ULongLong TIDorb::core::comm::TCPConnection::hashCode()
{
  //return my_port;
  //return socket->hashCode();
//MLG  
  //return socket->getLocalPort();
  return ((CORBA::ULongLong)this);
//EMLG  
}




const char* TIDorb::core::comm::TCPConnection::toString()
{

  if (str == NULL) {
    
    TIDThr::Synchronized sync(*this);

    if (str != NULL) // May be changed after return of synchro
      return str;

    TIDorb::util::StringBuffer buffer;
    switch(mode) {
      case CLIENT_MODE: 
        {
          buffer << "Client";
          buffer << " connection at " << initial_point.toString() << ends;
          break;
        }
      case SERVER_MODE:
        {
          buffer << "Server";
          TIDSocket::InetSocketAddress* addr = 
            (TIDSocket::InetSocketAddress*)(socket->getRemoteSocketAddress());
          buffer << " connection at ListenPoint(" << *addr << ")" << ends;
          delete addr;
          break;
        }
      default:        
        {
          buffer << "Bidirectional";
          buffer << " connection at " << initial_point.toString() << ends;
        }
    }
      
    str = CORBA::string_dup(buffer.str().data());
  }

  return str;
}




/**
 * Creates a new connection in SERVER_MODE.
 * @param mngr the manager
 * @param sock <code>Socket</code> created in the <code>accept</code> method
 * in a <code>ServerSocket</code> object.
 */
TIDorb::core::comm::Connection*
TIDorb::core::comm::TCPConnection::server_connection(TIDorb::core::comm::ConnectionManager* mngr,
                                                     TIDSocket::Socket* socketed)
{
  TIDorb::core::comm::Connection* conn = new TIDorb::core::comm::TCPConnection(mngr, socketed);
  conn->mode = SERVER_MODE;
  conn->init();

  if (conn->orb()->trace != NULL) {
    TIDorb::util::StringBuffer msg;   
    msg << "Creating " << conn->toString();
    conn->orb()->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  return conn;
}




/**
 * Creates a new connection in CLIENT_MODE.
 */
TIDorb::core::comm::Connection*
TIDorb::core::comm::TCPConnection::client_connection(TIDorb::core::comm::ConnectionManager* mngr,
                                                     const TIDorb::core::comm::iiop::ListenPoint& listen_point)
{
  try {
    // create the socket
    TIDSocket::Socket* socketed = new TIDSocket::Socket((const char*) listen_point._host, listen_point._port);
    TIDorb::core::comm::Connection* conn = new TIDorb::core::comm::TCPConnection(mngr, socketed);

    conn->mode = CLIENT_MODE;
    conn->initial_point = listen_point;
    conn->add_listen_point(listen_point);
    conn->init();

    return conn;

  } catch(const TIDSocket::SocketTimeoutException& ioe) {
    throw CORBA::COMM_FAILURE("SocketTimeout",0, CORBA::COMPLETED_NO);
  } catch(const TIDSocket::UnknownHostException& ioe) {
    throw CORBA::TRANSIENT("UnknowHost",0, CORBA::COMPLETED_NO);
  } catch(const TIDSocket::ClosedChannelException& ioe) {
    throw CORBA::COMM_FAILURE("ClosedChannel",0, CORBA::COMPLETED_MAYBE);
  } catch(const TIDSocket::IOException& ioe) {
    //throw CORBA::COMM_FAILURE(ioe.what(),0, CORBA::COMPLETED_MAYBE);
    throw CORBA::TRANSIENT("IOException",0, CORBA::COMPLETED_NO);
  }
}




void TIDorb::core::comm::TCPConnection::close()
{
  try {
    socket->close();
  } catch(const TIDSocket::IOException& ioe) {
  }

  // notify the locks
    
  lock_list.destroy();
  
  addresses.clear();   
}




void TIDorb::core::comm::TCPConnection::dispatch_request(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* msg)
{
  // Ensures that the request will be removed
  auto_ptr<TIDorb::core::comm::iiop::GIOPLocateRequestMessage> message(msg);
    
  // request is in the POA structure
  requests_in_POA++;

  TIDorb::core::comm::iiop::ObjectKey* obj_key = message->extract_address()->getObjectKey();
  TIDorb::core::cdr::CDRInputStream encapsulation (*(obj_key->getMarshaledKey()));

  encapsulation.set_version(TIDorb::core::comm::iiop::Version::VERSION_1_2);

  CORBA::Boolean byte_order;
  encapsulation.read_boolean(byte_order);

  encapsulation.set_byte_order(byte_order);

  encapsulation.fix_starting();

  TIDorb::core::poa::POAKey* key;
  try {
    key = obj_key->get_key();
  } catch (const CORBA::MARSHAL& m) {

    try {   

      CORBA::String_var url;
      url = obj_key->get_url();
      
      CORBA::Object_var obj = NULL;     
      obj = _orb->resolve_initial_references(url.in());            
      
      if ((obj)) {
        send_locate_reply(message->getHeader().getVersion(), message->getRequestId(), obj);
        return;
      }
      
    } catch (...) {
    }

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << ": Cannot get object key (CORBA::MARSHAL)" << ends;
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }

  }



  TIDorb::core::poa::IIOPLocateRequest* request =
    new TIDorb::core::poa::IIOPLocateRequest(key, _root_POA ,this, 
                                             message->getRequestId(), 
                                             message->getHeader().getVersion());

  TIDorb::core::poa::POAManagerImpl* poamanagerimpl = 
    (TIDorb::core::poa::POAManagerImpl*)(_poamanager);

  poamanagerimpl->put(request);
}




void TIDorb::core::comm::TCPConnection::dispatch_request(TIDorb::core::comm::iiop::GIOPRequestMessage* msg)
{
  // Ensures that the GIOPRequestMessage will be removed
  auto_ptr<TIDorb::core::comm::iiop::GIOPRequestMessage> message(msg);
  
  TIDorb::core::ServerRequestImpl* request = message->extract_request();

  // pra@tid.es - FT extensions
  if (strcmp(request->operation(), TIDorb::core::comm::FT::FT::OPERATION_NAME)==0) {
    send_reply(request);
    delete request;
    return;
  }
  // end FT extensions


  TIDorb::core::comm::iiop::ObjectKey* obj_key = request->get_target()->getObjectKey();

  TIDorb::core::cdr::CDRInputStream encapsulation (*(obj_key->getMarshaledKey()));

  // Key is encapsuled

  encapsulation.set_version(TIDorb::core::comm::iiop::Version::VERSION_1_2);

  CORBA::Boolean byte_order;
  encapsulation.read_boolean(byte_order);

  encapsulation.set_byte_order(byte_order);

  encapsulation.fix_starting();

  /* jagd lo movemos de sitio
  if (request->with_response())
  {
    requests_in_POA.inc();

    // is a url??
    try
    {
      // TODO cambiar a tratamiento de TIDorbJ 
  	 
      CORBA::String_var url;
//PRA
//FRAN
//      encapsulation.read_string(url.out());
//      encapsulation.read_string(url);
      encapsulation.read_string(url.out());
//EFRAN           
//EPRA
      CORBA::Object_var obj = NULL;

//FRAN
      obj = _orb->resolve_initial_references(url);      
//EFRAN

      //jagd
      //if (! CORBA::is_nil(obj))
      if ((obj))
      {
         request->set_forward(obj);
         send_reply(request);
         return;
      }

    } catch (...) {
    }
  }

    //TODO: revisar esta solucion temporal al OBJECT_NOT_EXIST de un corbaloc
    //PRA
   */ 
    //
    //TIDorb::core::poa::POAKey_ref key = obj_key->get_key();
    //
    
    //encapsulation.rewind();
    //obj_key->getMarshaledKey()->rewind();
    if (request->with_response())
    {
      //jagd requests_in_POA.inc();
      requests_in_POA++;
    }   

  
    TIDorb::core::poa::POAKey* key;
    try {
      key = obj_key->get_key();
    } catch (const CORBA::MARSHAL& m) {

      //jagd a?adimos las corbaloc si ha fallado al desaplanar el key
        if (request->with_response())
        {
          //requests_in_POA.inc();
      
          // is a url??
          try
          {
            // TODO cambiar a tratamiento de TIDorbJ
      
            CORBA::String_var url;
            // encapsulation.read_string(url.out()); // Fix bug #353 OBJECT_NOT_EXISTS dispaching request with corbaloc instead of POAkey
            url = obj_key->get_url();

            CORBA::Object_var obj = NULL;     
            obj = _orb->resolve_initial_references(url);
      
            //jagd
            //if (! CORBA::is_nil(obj))
            if ((obj))
            {
               request->set_forward(obj);
               send_reply(request);
               delete request;
               return;
            }
      
          } catch (...) {
          }
         
        }     



      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << toString() << ": Cannot get object key (CORBA::MARSHAL)" << ends;
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
      
      request->set_system_exception(CORBA::OBJECT_NOT_EXIST());
      send_reply(request);
      delete request;
      return;
    }
    //EPRA
    
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << toString() << ": Dispatching request id(" << request->get_id()
            << ") operation \"" << request->operation() << "\"" << ends;
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
    
      service_context_received(message->get_service_context_list());
   
      //jagd 
      //TIDorb::core::poa::POAImpl* root_POA= dynamic_cast<TIDorb::core::poa::POAImpl*>(_orb->init_POA());
    
      TIDorb::core::poa::IIOPRequest* poa_request =
        new TIDorb::core::poa::IIOPRequest(key, _root_POA, this, request);
    
      // QueueRequest (poa_request) take responsability about free InvocationPolicies)
      poa_request->setPolicyContext(message->getRequestInvocationPolicies());

      //jagd PortableServer::POAManager_ptr poamanager = root_POA->the_POAManager();
      //jagd
      //TIDorb::core::poa::POAManagerImpl* poamanagerimpl = dynamic_cast<TIDorb::core::poa::POAManagerImpl*>(poamanager);
      TIDorb::core::poa::POAManagerImpl* poamanagerimpl = (TIDorb::core::poa::POAManagerImpl*)(_poamanager);
      poamanagerimpl->put(poa_request);
}




void TIDorb::core::comm::TCPConnection::read(unsigned char* buffer, size_t buffer_size,
                                             size_t offset, size_t length)
{
  try {
    int numReadNow = 0;

    while (length > 0) {

      numReadNow = in->read(buffer,buffer_size, offset,length);

      if (numReadNow < 0) {
        throw CORBA::COMM_FAILURE("Broken Connection",0,CORBA::COMPLETED_NO);
      }

//MLG
      if (numReadNow == 0) {
        throw CORBA::COMM_FAILURE("Close Connection by pair",0,CORBA::COMPLETED_MAYBE);
      }
//EMLG

      offset += numReadNow;
      length -= numReadNow;

    }

  } catch(const TIDSocket::Exception& e) {
    CORBA::COMM_FAILURE connection_error(e.what(), 0, CORBA::COMPLETED_NO);
    close_by_broken_connection(connection_error);
    throw connection_error;
  }
}




void TIDorb::core::comm::TCPConnection::write(unsigned char* buffer, size_t buffer_size,
                                              size_t offset, size_t length)
{
  try {
//MLG
        size_t written = 0;
  	int chunk_written = 0;
  	
  	while (written < length)
  	{
  		chunk_written = out->write((const unsigned char*) buffer, buffer_size, offset, length-written);
		offset += chunk_written;
		written += chunk_written;
  	}
//EMLG  	
  } catch(const TIDSocket::IOException& ioe) {
    CORBA::COMM_FAILURE connection_error( "IOException", 0, CORBA::COMPLETED_NO);
                                         //ioe.toString(), 0, CORBA::COMPLETED_NO);
    close_by_broken_connection(connection_error);
    throw connection_error;
  }
}
