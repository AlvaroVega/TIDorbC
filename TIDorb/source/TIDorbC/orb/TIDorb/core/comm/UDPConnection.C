//////////////////////////////////////////////////////////////////////////////////
//
// File:        UDPConnection.C
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

#include<memory>

#ifndef MIN
#define MIN(a,b) (((a)<(b)) ? (a):(b))
#endif


#undef ERROR




TIDorb::core::comm::UDPConnection::UDPConnection(ConnectionManager* mngr, TIDSocket::DatagramSocket* s)
  throw (TIDThr::SystemException, CORBA::SystemException)
  : TIDorb::core::comm::Connection(mngr), collections(conf.max_uncompleted_messages)
{
  datagram_max_size = conf.udp_fragment_size;
  datagram_buffer   = (unsigned char*) malloc(datagram_max_size);
  datagram_size     = 0;
  datagram_offset   = 0;

  miop_packet_buffer = new TIDorb::core::cdr::BufferCDR(datagram_max_size);
  receive_miop_header_buffer = 
    new TIDorb::core::cdr::BufferCDR(TIDorb::core::comm::miop::MIOPPacket::HEADER_SIZE);

  try {
    socket = s;

    socket->setSendBufferSize(conf.udp_buffer_size);
    socket->setReceiveBufferSize(conf.udp_buffer_size);
    socket->setSoTimeout(conf.socket_timeout);

  } catch(const TIDSocket::Exception& e) {
    throw CORBA::OBJECT_NOT_EXIST(e.what(), 0, CORBA::COMPLETED_NO);
  }

  delay.tv_sec  = 0L;
  delay.tv_nsec = conf.udp_send_delay * 1000L;
}
  



TIDorb::core::comm::UDPConnection::~UDPConnection() throw (TIDThr::SystemException)
{
  free(datagram_buffer);
  delete socket;

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Removing all incompleted MIOP Packet Collections";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }
  vector<TIDorb::core::comm::miop::MIOPPacketCollection*>* elements = 
    collections.elements();
  vector<TIDorb::core::comm::miop::MIOPPacketCollection*>::iterator i = 
    elements->begin();
  while (i != elements->end()){
    for (size_t j = 0; j < (*i)->number_chunks; j++)
      if ((*i)->chunks[j] != NULL)
        delete (*i)->chunks[j];
    i++;
  }
  collections.clear();

  elements->clear();
  delete elements;

}




CORBA::ULongLong TIDorb::core::comm::UDPConnection::hashCode()
{
  //return socket->getLocalPort();
  return ((CORBA::ULongLong)this);
}




const char* TIDorb::core::comm::UDPConnection::toString()
{

  if (str == NULL) {
    TIDThr::Synchronized sync(*this);

    if (str != NULL) // May be changed after return of synchro
      return str;

    TIDorb::util::StringBuffer buffer;
    switch(mode) {
      case CLIENT_MODE:
        buffer << "UDP client" << " thread(" << TIDThr::Thread::getCurrentThreadId() << ")";
        break;
      case SERVER_MODE:
        buffer << "UDP server" << " thread(" << TIDThr::Thread::getCurrentThreadId() << ")";
        break;
      default:
        buffer << "Invalid UDP" << " thread(" << TIDThr::Thread::getCurrentThreadId() << ")"; /*unreachable*/
    }
    //buffer << " connection at " << my_port; /*pra@tid.es socket->getLocalPort();*/
    buffer << " connection " << hashCode() << ends;

    str = CORBA::string_dup(buffer.str().data());
  }

  return str;
}




/**
 * Creates a new connection in SERVER_MODE.
 */
TIDorb::core::comm::Connection*
TIDorb::core::comm::UDPConnection::server_connection(TIDorb::core::comm::ConnectionManager* mngr,
                                                     const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  TIDSocket::DatagramSocket* socket  = NULL;

  try {

/*  Allow reuse port
    // create the datagram socket
    socket = new TIDSocket::DatagramSocket(listen_point._port);
*/
    // create the datagram socket
    const TIDSocket::SocketAddress* nullAddr = NULL;
    socket = new TIDSocket::DatagramSocket(nullAddr);
    socket->setReuseAddress(true);

    // resolve unicast address
    TIDSocket::InetSocketAddress addr(listen_point._host, listen_point._port);
    if (addr.isUnresolved() || addr.getAddress().isMulticastAddress()) {
      delete socket;
      throw CORBA::COMM_FAILURE("Invalid unicast address", 0, CORBA::COMPLETED_NO);
    }

    socket->bind(&addr);

    // create connection
    TIDorb::core::comm::Connection* conn = new TIDorb::core::comm::UDPConnection(mngr, socket);
    conn->mode = SERVER_MODE;
    conn->add_listen_point(listen_point);
    conn->init();
    return conn;

  } catch(const TIDSocket::Exception& e) {
    delete socket;
    throw CORBA::COMM_FAILURE(e.what(), 0, CORBA::COMPLETED_NO);
  }
}




/**
 * Creates a new connection in CLIENT_MODE.
 */
TIDorb::core::comm::Connection*
TIDorb::core::comm::UDPConnection::client_connection(TIDorb::core::comm::ConnectionManager* mngr,
                                                     const TIDorb::core::comm::miop::ListenPoint& listen_point)
{
  TIDSocket::DatagramSocket* socket  = NULL;

  try {
    // create the datagram socket
    socket = new TIDSocket::DatagramSocket();

    // resolve unicast address
    TIDSocket::InetSocketAddress addr(listen_point._host, listen_point._port);
    if (addr.isUnresolved() || addr.getAddress().isMulticastAddress()) {
      delete socket;
      throw CORBA::COMM_FAILURE("Invalid unicast address", 0, CORBA::COMPLETED_NO);
    }

    // connect datagram socket
    // TODO: revisar si este connect() es realmente innecesario
    // socket->connect(addr); UNNECESSARY

    // create connection
    TIDorb::core::comm::Connection* conn = new TIDorb::core::comm::UDPConnection(mngr, socket);
    conn->mode = CLIENT_MODE;
    conn->initial_point = listen_point;
    conn->add_listen_point(listen_point);
    return conn;

  } catch(const TIDSocket::Exception& e) {
    delete socket;
    throw CORBA::COMM_FAILURE(e.what(), 0, CORBA::COMPLETED_NO);
  }

  return NULL;
}




void TIDorb::core::comm::UDPConnection::close()
{
  try {
    socket->close();
  } catch(const TIDSocket::IOException& ioe) {
  }

  // notify the locks
  
  lock_list.destroy();

  addresses.clear();
   
}




void TIDorb::core::comm::UDPConnection::send_close()
{
  // Nothing to do
}




void TIDorb::core::comm::UDPConnection::send_error()
{
  // Nothing to do
}




void TIDorb::core::comm::UDPConnection::send_oneway_request_async
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior)
        throw (TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,CORBA::SystemException)
{
  send_oneway_request_async(request, ior, TIDorb::core::comm::iiop::ProfileAddr);
}




void TIDorb::core::comm::UDPConnection::send_oneway_request_async
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior,
   TIDorb::core::comm::iiop::AddressingDisposition disposition)
        throw (TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,CORBA::SystemException)
{
  // verify if the request can be sent
  state.verify_request();

  TIDorb::core::comm::iiop::RequestId id = generateId();
  request->set_id(id);

  const TIDorb::core::comm::iiop::Version& version = ior->GIOPVersion();

  // create message
  size_t header_size   = TIDorb::core::comm::miop::MIOPPacket::HEADER_SIZE;
  size_t fragment_size = datagram_max_size - header_size;

  Compression::CompressorIdLevel compressor(0, 0);
  TIDorb::core::PolicyContext* policy_context = NULL;

  if (qos_enabled) {

    policy_context = request->getPolicyContext();

    if (/*ior->is_ZIOP() || */assume_ziop_server) {

      TIDorb::core::PolicyContext* policies_context_ior = NULL;
      policies_context_ior = ior->policies();

      compressor = TIDorb::core::ziop::Ziop::getClientCompressor(
                                                              *policy_context,
                                                              *policies_context_ior,
                                                              assume_ziop_server);

      }

  }


  TIDorb::core::comm::iiop::GIOPRequestMessage message(version, id, fragment_size);

  try {

    //set_service_context_list(&message, policy_context); // needed?

    message.insert_request(request, *ior, disposition);

    if (compressor.compressor_id != Compression::COMPRESSORID_NONE) {

      CORBA::ULong low_value = 
        TIDorb::core::ziop::Ziop::getLowValue(*policy_context);
      
      ZIOP::CompressionMinRatioPolicyValue min_ratio = 
        TIDorb::core::ziop::Ziop::getMinRatio(*policy_context);

      TIDorb::core::comm::ziop::ZIOPMessage ziop_message(message);

      if (ziop_message.perform_compression(_orb, compressor, 
                                           low_value, min_ratio)) 
        send_message(ziop_message, id);
      else 
        send_message(message, id);
    }
    else { 
      send_message(message, id);
    }

  } catch (const CORBA::COMM_FAILURE& comm) {
    throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(comm);
  } catch (const CORBA::SystemException& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "UDPConnection::send_oneway_request_async catch CORBA::SystemException: ";
      msg << ex.what();
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    throw;
  } catch (const TIDThr::SystemException& threx) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected TIDThr exception: " << threx.what();
      msg << " at UDPConnection::send_oneway_request_async";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (const exception& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception: " << ex.what();
      msg << " at UDPConnection::send_oneway_request_async";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception raised.";
      msg << " at UDPConnection::send_oneway_request_async";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }
}



void TIDorb::core::comm::UDPConnection::dispatch_request(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message)
{
  // Nothing to do
  delete message;
}




void TIDorb::core::comm::UDPConnection::dispatch_request(TIDorb::core::comm::iiop::GIOPRequestMessage* msg)
{
  auto_ptr<TIDorb::core::comm::iiop::GIOPRequestMessage> message (msg);
  
  // TODO: reimplement extract_request, now it can be only invoked once
  TIDorb::core::ServerRequestImpl* request = message->extract_request();

  const TIDorb::core::comm::iiop::TargetAddress* target = request->get_target();
  
  // Get UIPMC profile
  const TIDorb::core::iop::TaggedProfile* tagged_profile = target->profile();
  const TIDorb::core::comm::miop::ProfileMIOP* profile =
    (const TIDorb::core::comm::miop::ProfileMIOP*)(tagged_profile);

  if (profile == NULL) {
    // Pre-GIOP 1.2 support: 'MIOP' octets followed by encapsulated UIPMC profile
    // TODO: implement UIPMC profile extraction from ObjectKey
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << ": MIOP over pre-GIOP 1.2 not implemented" << ends;
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }
    delete request;
    return;
  }

  TIDorb::core::goa::GOAImpl* root_GOA = NULL;
  try {
    root_GOA =
    dynamic_cast<TIDorb::core::goa::GOAImpl*>(_orb->init_POA());
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << " UDPConnection::dispatch_request uncaught exception getting root_GOA" << ends;
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }

  PortableServer::POAManager_ptr poamanager = root_GOA->the_POAManager();
  TIDorb::core::poa::POAManagerImpl* poamanagerimpl =
    (TIDorb::core::poa::POAManagerImpl*)(poamanager);


  service_context_received(message->get_service_context_list());

  // Get the group object id
  string group_object_id;// = TIDorb::core::comm::miop::MIOPCorbaloc::get_group_object_id(profile);
  try {
    group_object_id = TIDorb::core::comm::miop::MIOPCorbaloc::get_group_object_id(profile);
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << " UDPConnection::dispatch_request uncaught exception getting group_object_id" << ends;
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }

  // List of POAKeys associated to the group object (through different GOAs and ObjectIds)
  TIDorb::core::poa::POAKeyList keys(0);
  root_GOA->group_object_POAKeys(group_object_id, keys);

  if (keys.size() == 0) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << ": Cannot find ObjectIds for Group Object " << group_object_id << ends;
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }
    delete request;
    return;
  }

  // Dispatch request
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Dispatching request id(" << request->get_id()
        << ") operation \"" << request->operation() << "\"" << ends;
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  // Put requests  

  TIDorb::core::ServerRequestImpl* server_request = NULL;
  TIDorb::core::goa::MIOPRequest*  poa_request = NULL;

  for (CORBA::ULong i = 0; i < keys.size(); i++) {
    server_request = new TIDorb::core::ServerRequestImpl(*request);
    poa_request = new TIDorb::core::goa::MIOPRequest(keys[i], //consume
                                                     root_GOA, 
                                                     this, 
                                                     server_request); //consume
    poamanagerimpl->put(poa_request);
  }

  // Finish
  delete request;
}



void TIDorb::core::comm::UDPConnection::receive_message(){

  TIDorb::core::comm::miop::UniqueId unique_id = 0;
 
  TIDorb::core::comm::miop::MIOPPacketCollection* collection = NULL;

  while(true) {

    if(state.is_open())
      manager->use(this);
    else
      break;

    try {    
      bool completed_message = false;

      // 1 - Read until compose a MIOP Packet Collection 
      // TODO: UseTable with miop_max_uncomppleted_messages (now using iiop_max_...)

      while (!completed_message){
        // 1.1 - Read MIOP header
        read((unsigned char*) (receive_miop_header_buffer->get_chunk(0)->get_buffer()),
             TIDorb::core::comm::miop::MIOPPacket::HEADER_SIZE,0,
             (size_t) TIDorb::core::comm::miop::MIOPPacket::HEADER_SIZE);
        TIDorb::core::cdr::CDRInputStream input(NULL, receive_miop_header_buffer);
        CORBA::Long miop_word; 
        CORBA::Boolean _byte_order = TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER;
        input.read_long(miop_word);

        if ( (_byte_order && (miop_word!=1347373389)) ||
             (!_byte_order && (miop_word!=1296650064))) {

          if (_orb->trace != NULL) {
            TIDorb::util::StringBuffer msg;
            msg << "Bad MIOP header read " << miop_word;
            _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
          }
          // Fix bug #423 TIDorb close socket connection after receive a bad MIOP header
          // Discard rest of the datagram: reset datagram buffer
          datagram_size     = 0;
          datagram_offset   = 0;
          receive_miop_header_buffer->recycle();
          continue;
        }

        CORBA::Octet version;
        input.read_octet(version);                            //hdr_version
        CORBA::Octet flags;
        input.read_octet(flags);                              //flags
        
        if (version == 16) { // MIOP Version 1.0
          _byte_order = ((flags & 0x1) != 0);
        }
        input.set_byte_order(_byte_order);

        CORBA::UShort packet_length;
        input.read_ushort(packet_length);                     //packet_length
        CORBA::ULong packet_number;
        input.read_ulong(packet_number);                      //packet_number
        CORBA::ULong number_packets;
        input.read_ulong(number_packets);                     //number_of_packets
        CORBA::ULong unique_id_len;
        input.read_ulong(unique_id_len);
        
        CORBA::Octet* id = (CORBA::Octet*) &unique_id; // unique_id
        
        // Internally, due to performance reasons, unique_id only has 8 bytes
        int j = 0;
        while (j < unique_id_len && j < TIDorb::core::cdr::CDR::ULONGLONG_SIZE) {
          input.read_octet(id[j]);
          j++;
        }
        while (j < unique_id_len) {
          input.skip_octet();
          j++;
        }

        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "Receiving MIOP packet header with info: ";
          msg << packet_length <<" "<< packet_number <<" "<< number_packets <<" ";
          msg << unique_id_len <<" "<< unique_id;
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        
        // 1.2 - Read rest of MIOP Packet in a chunk
        TIDorb::core::cdr::ChunkCDR* chunk = new TIDorb::core::cdr::ChunkCDR(packet_length);
        read((unsigned char*) (chunk->get_buffer()), packet_length, 0, packet_length);
        chunk->set_available_bytes(packet_length); 

        if (! (packet_number < number_packets)){
          if (_orb->trace != NULL) {
            TIDorb::util::StringBuffer msg;
            msg << "Bad MIOP packet read. Discarding chunk of " << packet_length << " bytes";
            _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
          }
          delete chunk;
          receive_miop_header_buffer->recycle();
          continue;
        }


        // 1.3 - Insert in to collections
        collection = collections.get(unique_id);
        if (collection != NULL){           // Add the new chunk

          // Check:
          // - It is not duplicate packet
          // - The number of packets matches
          if ( (collection->chunks[packet_number] == NULL) && 
               (collection->number_chunks == number_packets) ) { 
            collection->number_chunks_inserted++;
            collection->chunks[packet_number] = chunk;
          }
          else{ // Not valid
            if (_orb->trace != NULL) {
              TIDorb::util::StringBuffer msg;
              msg << "Duplicated MIOP packet. Discarding...";
              _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
            }
            delete chunk;
          }
        }
        else{
          // Create a new collection and insert chunk in it
          collection = new TIDorb::core::comm::miop::MIOPPacketCollection;
          collection->number_chunks = number_packets;
          collection->chunks = new TIDorb::core::cdr::ChunkCDR*[number_packets];

          // Initialize chunks vector with NULL value
          for (CORBA::ULong i = 0; i < number_packets; i++)
            collection->chunks[i] = NULL; 

          collection->chunks[packet_number] = chunk;
          collections.append(unique_id, collection);
          collection->number_chunks_inserted = 1;

          // Free removed_objects of collections
          vector<TIDorb::core::comm::miop::MIOPPacketCollection*>* removed = 
            collections.getRemovedObjects();
          if (removed != NULL) {
            if (_orb->trace != NULL) {
              TIDorb::util::StringBuffer msg;
              msg << "Releasing old removed incompleted MIOP packet collections";
              _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
            }
            vector<TIDorb::core::comm::miop::MIOPPacketCollection*>::iterator i = 
              removed->begin();
            vector<TIDorb::core::comm::miop::MIOPPacketCollection*>::iterator end = 
              removed->end();
            while (i != end){
              for (size_t j = 0; j < (*i)->number_chunks; j++)
                if ((*i)->chunks[j] != NULL)
                  delete (*i)->chunks[j];
              delete[] (*i)->chunks;
              delete (*i);
              i++;
            }
            removed->clear();
            delete removed;  
          } // if (removed != NULL) 
        } // if (collection != NULL)

        // 1.4 - Check if current chunk completed a MIOP PacketCollection 
        if (collection->number_chunks_inserted == collection->number_chunks) {
          completed_message = true;
        }

        receive_miop_header_buffer->recycle();
        
      } // while (!completed_message)

      // Here, we have a completed  GIOP message in collection
      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "Processing a completed MIOP packet collection id=";
        msg << unique_id; 
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }
      
      // 2 - Create a GIOP::Mensagge with header and related chunks 
      // 2.1 - Compose bufferCDR for message
      TIDorb::core::cdr::BufferCDR_ref buffer 
        = new TIDorb::core::cdr::BufferCDR(collection->chunks[0]);
      for (CORBA::ULong i = 1; i < collection->number_chunks; i++)
        buffer->add_chunk(collection->chunks[i]);
      
      // 2.2 - Compose GIOP header
      TIDorb::core::cdr::CDRInputStream *input = 
        new TIDorb::core::cdr::CDRInputStream(this->orb(), buffer);
      TIDorb::core::comm::iiop::GIOPHeader header;
      header.read(*input);


      // Attend for compressed messages
      if (header.getCompressed()){
        TIDorb::core::comm::ziop::ZIOPMessage ziop_message(header);
        
        ziop_message.receive_body(this, (unsigned char*) 
                                  (receive_header_buffer->get_chunk(0)->get_buffer()));

        ziop_message.connect_GIOPMessage(this);
        continue;
      }


      switch(header.getMsgType()) {
      case TIDorb::core::comm::iiop::Request: {

        TIDorb::core::comm::iiop::GIOPRequestMessage* message = 
          new TIDorb::core::comm::iiop::GIOPRequestMessage(header);
        
        if (message->hasBody()){
          message->set_body(buffer, input);
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "GIOP message chunk received - HEXDUMP ";
          msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
          _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
          _orb->print_dump(TIDorb::util::TR_DUMP, 
                           (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                           message->get_message_buffer()->get_chunk(0)->get_length());
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << toString() << ": " << *message << " has been received!";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        // 3 - Manage message
        manage_message(message);
        break;
      }  
      case TIDorb::core::comm::iiop::Reply: {

        TIDorb::core::comm::iiop::GIOPReplyMessage* message = 
          new TIDorb::core::comm::iiop::GIOPReplyMessage(header);
        
        if (message->hasBody()){
          message->set_body(buffer,input);
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "GIOP message chunk received - HEXDUMP ";
          msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
          _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
          _orb->print_dump(TIDorb::util::TR_DUMP, 
                           (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                           message->get_message_buffer()->get_chunk(0)->get_length());
        }

        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << toString() << ": " << *message << " has been received!";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        manage_message(message);
        break;
      }  
      case TIDorb::core::comm::iiop::CancelRequest: {
        TIDorb::core::comm::iiop::GIOPCancelRequestMessage* message = 
          new TIDorb::core::comm::iiop::GIOPCancelRequestMessage(header);    
        
        if (message->hasBody()) {
          message->set_body(buffer,input);
        }

        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "GIOP message chunk received - HEXDUMP ";
          msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
          _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
          _orb->print_dump(TIDorb::util::TR_DUMP, 
                           (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                           message->get_message_buffer()->get_chunk(0)->get_length());
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << toString() << ": " << *message << " has been received!";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        
        manage_message(message);
        break;
      }  
      case TIDorb::core::comm::iiop::LocateRequest: {

        TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message = 
          new TIDorb::core::comm::iiop::GIOPLocateRequestMessage(header);
        
        if (message->hasBody()){
          message->set_body(buffer,input);
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "GIOP message chunk received - HEXDUMP ";
          msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
          _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
          _orb->print_dump(TIDorb::util::TR_DUMP, 
                           (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                           message->get_message_buffer()->get_chunk(0)->get_length());
        }

        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << toString() << ": " << *message << " has been received!";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }

        manage_message(message);
        break;
      }  
      case TIDorb::core::comm::iiop::LocateReply: {

        TIDorb::core::comm::iiop::GIOPLocateReplyMessage* message = 
          new TIDorb::core::comm::iiop::GIOPLocateReplyMessage(header);
        
        if (message->hasBody()){
          message->set_body(buffer,input);
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << toString() << ": " << *message << " has been received!";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }

        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "GIOP message chunk received - HEXDUMP ";
          msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
          _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
          _orb->print_dump(TIDorb::util::TR_DUMP, 
                           (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                           message->get_message_buffer()->get_chunk(0)->get_length());
        }

        manage_message(message);
        break;
      }  
      case TIDorb::core::comm::iiop::Fragment: {
        TIDorb::core::comm::iiop::GIOPFragmentMessage* message = 
          new TIDorb::core::comm::iiop::GIOPFragmentMessage(header);    
          
        if (message->hasBody()) {
           message->set_body(buffer,input);
        }       

        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << "GIOP message chunk received - HEXDUMP ";
          msg << message->get_message_buffer()->get_chunk(0)->get_length() << " bytes";
          _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
          _orb->print_dump(TIDorb::util::TR_DUMP, 
                           (const unsigned char*)(message->get_message_buffer()->get_chunk(0)->get_buffer()), 
                           message->get_message_buffer()->get_chunk(0)->get_length());
        }
        
        if (_orb->trace != NULL) {
          TIDorb::util::StringBuffer msg;
          msg << toString() << ": " << *message << " has been received!";
          _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
        }
        
        manage_message(message);
        break;
      }  
      case TIDorb::core::comm::iiop::CloseConnection: {
        close_by_pair();
        return;
      }
      case TIDorb::core::comm::iiop::MessageError: {
        close_by_error(CORBA::COMM_FAILURE("Connection closed due to pair message error."));
        return;
      }

      } // switch(header->getMsgType())

      delete[] collection->chunks;
      delete collection;
      collections.remove(unique_id);

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "MIOP packet collection id=" << unique_id;
        msg << " processed";
        _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
      }

      
    } catch (const CORBA::COMM_FAILURE& comm) {
      close_by_error(comm);
      return;
    // Fix bug [#440] TIDorb close socket connection after obtain MARSHAL processing 
    // a completed MIOP packet
    } catch (const CORBA::MARSHAL& marshal) {
      if (_orb->trace != NULL) {
        _orb->print_trace(TIDorb::util::TR_ERROR, toString(), marshal);
      }
      if (collection) {
        delete[] collection->chunks;
        delete collection;
      }
      collections.remove(unique_id);
      receive_miop_header_buffer->recycle();
      continue;

    } catch (const CORBA::Exception& corba_ex) {
      if (_orb->trace != NULL) {
        _orb->print_trace(TIDorb::util::TR_ERROR, toString(), corba_ex);
      }
      close_by_error(CORBA::COMM_FAILURE(corba_ex._name()));
      return;

    } catch (const exception& excp) {
      if (_orb->trace != NULL) {
        _orb->print_trace(TIDorb::util::TR_ERROR, toString(), excp);
      }
      close_by_error(CORBA::COMM_FAILURE(excp.what()));
      return;
    }
    
  } // while(true) 

}


void TIDorb::core::comm::UDPConnection::read(unsigned char* buffer, size_t buffer_size,
                                             size_t offset, size_t length)
{
  try {
    while (length > 0) {

      // Copy datagram bytes available in read buffer
      size_t available = datagram_size - datagram_offset;
      if (available) {
        size_t to_copy = MIN(length, available);
        memcpy(buffer + offset, datagram_buffer + datagram_offset, to_copy);

        offset += to_copy;
        length -= to_copy;

        datagram_offset += to_copy;

      } else {

        // Read another datagram
        TIDSocket::DatagramPacket datagram(datagram_buffer, datagram_max_size,
                                           0, datagram_max_size);
        socket->receive(datagram);

        datagram_size   = datagram.getLength();
        if (datagram_size < 0) {
          throw CORBA::COMM_FAILURE("read error", 0, CORBA::COMPLETED_NO);
        }
      
        datagram_offset = 0;

      }
    }

  } catch(const TIDSocket::Exception& e) {
    CORBA::COMM_FAILURE connection_error(e.what(), 0, CORBA::COMPLETED_NO);
    // TODO: ?seguro que este close()?
    close_by_broken_connection(connection_error);
    throw connection_error;
  }

}




void TIDorb::core::comm::UDPConnection::write(unsigned char* buffer, 
                                              size_t buffer_size, 
                                              size_t offset, size_t length)
{
  try {
    size_t numWrittenNow = 0;
    
    TIDSocket::InetSocketAddress addr(initial_point._host, initial_point._port);
    TIDSocket::DatagramPacket datagram(buffer, buffer_size, offset, length, addr);

    while (length > 0) {
      
      socket->send(datagram);

      numWrittenNow = datagram.getLength();      
      if (numWrittenNow < 0) {
        throw CORBA::COMM_FAILURE("write error", 0, CORBA::COMPLETED_NO);
      }

      offset += numWrittenNow;
      length -= numWrittenNow;

      datagram.setData(buffer, buffer_size, offset, length); 
    }

  } catch(const TIDSocket::Exception& e) {
    CORBA::COMM_FAILURE connection_error(e.what(), 0, CORBA::COMPLETED_NO);
    close_by_broken_connection(connection_error);
    throw connection_error;
  }
}




void TIDorb::core::comm::UDPConnection::write_Version_1_1(
                         const TIDorb::core::comm::iiop::GIOPMessage& message,
                         const TIDorb::core::comm::iiop::RequestId request_id)
{
  TIDorb::core::cdr::BufferCDR* buffer = message.get_message_buffer();
  size_t num_chunks = buffer->get_num_available_chunks();

  TIDorb::core::cdr::ChunkCDR* chunk = NULL;

  {
    TIDThr::Synchronized synchro(write_mutex);

    // Start a new MIOP Packet Collection
    TIDorb::core::comm::miop::MIOPPacket packet(
                                  TIDorb::core::comm::miop::Version::VERSION_1_0,
                                  // TODO: add timestamp or MAC address
//                                   (~((TIDorb::core::comm::miop::UniqueId) request_id) + 
//                                    _orb->getTimeStamp()), 
                                  (TIDorb::core::comm::miop::UniqueId) request_id,
                                  miop_packet_buffer,
                                  num_chunks);

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Sending MIOP packet collection id=";
      msg << request_id <<" chunks="<< num_chunks;
      _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
    }
    
    for (size_t i = 0; i < num_chunks; i++) {
      chunk = buffer->get_chunk(i);
      size_t packet_size = packet.preparePacket(
                             (unsigned char*) chunk->get_buffer(),
                             chunk->get_length(), i,
                             chunk->get_available_bytes());

      write(packet.getPacketBuffer(), packet_size, 0, packet_size);


      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk sent - HEXDUMP " << packet_size << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, 
                         (const unsigned char*)packet.getPacketBuffer(), 
                         packet_size);
      }

      // Delay
      if (delay.tv_nsec) nanosleep(&delay, NULL);
    }
  }
}
