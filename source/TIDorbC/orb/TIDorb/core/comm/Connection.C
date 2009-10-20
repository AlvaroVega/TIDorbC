/////////////////////////////////////////////////////////////////////////
//
// File:        Connection.C
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

#include "TIDorb/core/comm.h"

#include <memory>

#ifndef MIN
#define MIN(a,b) (((a)<(b)) ? (a):(b))
#endif


#undef ERROR




TIDorb::core::comm::Connection::Connection(ConnectionManager* mngr)
  throw (TIDThr::SystemException, CORBA::SystemException)
  : my_port(0),
    lock_list(mngr->get_lock_pool()),
    uncompleted_messages(mngr->get_uncompleted_messages()),
    str(NULL),
    _orb(mngr->orb()), 
    conf(mngr->orb()->conf())

{
  manager= mngr;
  orb_ver = conf.GIOPVersion;
  max_response_blocked_time = conf.max_blocked_time;
  qos_enabled = conf.qos_enabled;
  assume_ziop_server = conf.assume_ziop_server;

  bidirectional_service = NULL;
  send_bidirectional_service = false;

  current_request_id = 0;  
  
  send_header_buffer = new TIDorb::core::cdr::BufferCDR(TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);
  receive_header_buffer = new TIDorb::core::cdr::BufferCDR(TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);

  // pra@tid.es - FT extensions
  heartbeat_time = 0;
  heartbeart_req_id = TIDorb::core::util::Counter::RESERVED;
  // end FT extensions

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  _group = new TIDThr::ThreadGroup(NULL, "", &attr);
//FRAN
  // it can be destroyed because ThreadGroup copies it inside the constructor
  pthread_attr_destroy(&attr);
//EFRAN
}




TIDorb::core::comm::Connection::~Connection() throw (TIDThr::SystemException)
{  
  CORBA::string_free(str);

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << "Deleting Connection";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }
}

/**
 * Start the connection thread execution.
 */
void TIDorb::core::comm::Connection::init()
{
  typedef TIDThr::HandleT<ConnectionThread> ConnectionThreadHandle;

  try {
    //ConnectionThread* conn_thread = new ConnectionThread(this, _group);
    ConnectionThreadHandle conn_thread = new ConnectionThread(this, _group);
    conn_thread->start();

  } catch(const exception& thw) {
    TIDorb::util::StringBuffer buffer;
    buffer << "Can't create thread: " << thw.what();
    if (_orb->trace != NULL) 
      _orb->print_trace(TIDorb::util::TR_ERROR, buffer.str().data());
    
    throw CORBA::NO_RESOURCES(buffer.str().data());
  }
}




TIDorb::core::comm::ConnectionDescriptor TIDorb::core::comm::Connection::get_descriptor()
{
  return ConnectionDescriptor(hashCode());
}




bool TIDorb::core::comm::Connection::equals(Connection* obj)
{
  return (my_port == ((Connection*)obj)->my_port);
}




/**
 * Add a new listen point for whose this connection can be uses to sending request.
 * This will be usefull if this connection is bidirectional.
 * @param listen_point the new listen point of an bidirectional connection
 */
void TIDorb::core::comm::Connection::add_listen_point(const TIDorb::core::comm::iiop::ListenPoint& listen_point)
{
  addresses.insert(listen_point);
}



/**
 * Changes the connection mode to BIDIRECTIONAL_MODE. This method is called by
 * the connection if it detects that a referenced object is defined in a
 * "Bidirectional POA". The contest will be sent in the next response.
 * @param context the list containing the <code>BiDirServiceContext</code>
 */
void TIDorb::core::comm::Connection::set_bidirectional_mode(TIDorb::core::comm::iiop::ServiceContextList* context)
{
  TIDThr::Synchronized sync(*this);

  if (mode != BIDIRECTIONAL_MODE) {
    mode = BIDIRECTIONAL_MODE;
    CORBA::string_free(str); // reset the string
    str = NULL;
  }

  bidirectional_service = context;

  send_bidirectional_service = true;

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Setting Bidirectional mode";
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }
}




/**
 * Changes the connection mode to BIDIRECTIONAL_MODE. This method is called by
 * the connection if it detects a bidirectional context in a request or response
 * sent by the peer.
 * @param context the bidirectional context
 */
void TIDorb::core::comm::Connection::set_bidirectional_mode_by_peer(const TIDorb::core::comm::iiop::BiDirServiceContext* context)
{
  if (mode != BIDIRECTIONAL_MODE) {
    mode = BIDIRECTIONAL_MODE;
    CORBA::string_free(str); // reset the string
    str = NULL;
  }

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Setting Bidirectional mode by peer";
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  // TODO: Connection::set_bidirectional_mode_by_peer() debe marcar heartbeat_enabled

  CORBA::ULong i;
  for(i= 0; i < context->listen_points.size(); i++) {
    add_listen_point(context->listen_points[i]);
    manager->add_bidirectional_connection(context->listen_points[i], this);

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << " will be connected to ";
      msg << context->listen_points[i].toString();
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  }
}




/**
 * Test whether the connection is still open or not.
 * @return <code>true</code> if open or <code>false</code> if not.
 */
bool TIDorb::core::comm::Connection::is_open()
{
  return state.is_open();
}




/**
 * @return the communication error (if exists)
 */
const CORBA::COMM_FAILURE* TIDorb::core::comm::Connection::get_connection_error()
{
  return state.get_error();
}




/**
 * @return <code>true</code> if the communication is running in client mode
 */
bool TIDorb::core::comm::Connection::is_client_connection()
{
  return mode == CLIENT_MODE;
}




/**
 * @return <code>true</code> if the communication is running in client mode
 */
bool TIDorb::core::comm::Connection::is_server_connection()
{
  return mode == SERVER_MODE;
}




/**
 * @return <code>true</code> if the communication is running in client mode
 */
bool TIDorb::core::comm::Connection::is_bidirectional_connection()
{
  return mode == BIDIRECTIONAL_MODE;
}




/**
 * The connection has received a close connection.
 */
void TIDorb::core::comm::Connection::close_by_pair()
{

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << " closed by pair";
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  if(state.set_closing()) {
    manager->closing(this);
    close();
  }

}




/**
 * The connection will be closed by the connection manager.
 * @see TIDorb::core::comm::ConnectionManager
 */
void TIDorb::core::comm::Connection::close_by_manager()
{
  if(state.set_closing()) {
    if(mode != CLIENT_MODE)
      send_close();

    size_t lock_list_size = lock_list.size();
    if(lock_list_size != 0){
      if (lock_list_size == conf.max_connections)
        state.set_error(CORBA::COMM_FAILURE("Connection closed (maximum number of connections reached).",
                                            0, CORBA::COMPLETED_NO));
      else // Closed by HeartbeatThread
        state.set_error(CORBA::COMM_FAILURE("Connection closed.",
                                            0, CORBA::COMPLETED_MAYBE));
    }
    
    close();

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << " closed by Connection Manager";
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  }
}




/**
 * The connection is closed due to the socket is broken.
 * @param error the broken communication error
 */
void TIDorb::core::comm::Connection::close_by_broken_connection(const CORBA::COMM_FAILURE& the_error)
{
  if(state.set_error(the_error)) {
    manager->closing(this);
    close();

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << " is closing: " << the_error;
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  }
}




void TIDorb::core::comm::Connection::close_by_error(const CORBA::COMM_FAILURE& the_error)
{
  if(state.set_error(the_error)) {
    manager->closing(this);
    close();

    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << toString() << ": " << the_error;
      _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
    }
  }
}




void TIDorb::core::comm::Connection::send_close()
{
  try {
    TIDorb::core::comm::iiop::GIOPHeader header(orb_ver,
                                                TIDorb::core::comm::iiop::CloseConnection);
    send_message(header);
  } catch (const exception& e) {
  }
}




void TIDorb::core::comm::Connection::send_error()
{
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Sending a GIOP Message error";
    _orb->print_trace(TIDorb::util::TR_DEBUG, msg.str().data());
  }

  TIDorb::core::comm::iiop::GIOPHeader header(orb_ver,
                                              TIDorb::core::comm::iiop::MessageError);
  send_message(header);
}




void TIDorb::core::comm::Connection::send_header(const TIDorb::core::comm::iiop::GIOPHeader& header)
{
  TIDThr::Synchronized synchro(write_mutex);

  TIDorb::core::cdr::CDROutputStream os(NULL, send_header_buffer);
  header.write(os);

  write((unsigned char*) (send_header_buffer->get_chunk(0)->get_buffer()),
        TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE,
        (size_t) TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);
}




void TIDorb::core::comm::Connection::receive_header(TIDorb::core::comm::iiop::GIOPHeader& header)
{
  read((unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()),
       TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE,
       (size_t) TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);

  TIDorb::core::cdr::CDRInputStream input(NULL, receive_header_buffer);
  header.read(input);  
}




void TIDorb::core::comm::Connection::send_message(const TIDorb::core::comm::iiop::GIOPHeader& header)
{
  send_header(header);
}




void TIDorb::core::comm::Connection::send_message(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                                  const TIDorb::core::comm::iiop::RequestId request_id)
{
  if(state.is_open())
    manager->use(this);

  // pra@tid.es - El metodo GIOPMessage::send(Connection) se ha movido a Connection
  //message.send(this);

  const TIDorb::core::comm::iiop::Version& header_version = message.getHeader().getVersion();

  if (header_version >= TIDorb::core::comm::iiop::Version::VERSION_1_2)
    write_Version_1_2(message, request_id);
  else if (header_version == TIDorb::core::comm::iiop::Version::VERSION_1_1)
    write_Version_1_1(message, request_id);
  else
    write_Version_1_0(message, request_id);

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString();
    msg << ": " << message;
    msg << " has been sent!";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }
}




void TIDorb::core::comm::Connection::receive_message()
{
//MLG
  while(true) {

    if(state.is_open())
      manager->use(this);
    else
      break;

    try {
      // wait for reply
      TIDorb::core::comm::iiop::GIOPHeader header;
      
      receive_header(header);

      // Attend for compressed messages
      if (header.getCompressed()){
        TIDorb::core::comm::ziop::ZIOPMessage ziop_message(header);
        
        ziop_message.receive_body(this, (unsigned char*) 
                                  (receive_header_buffer->get_chunk(0)->get_buffer()));

        ziop_message.connect_GIOPMessage(this);
        continue;
      }
      


      switch(header.getMsgType()) {
        case TIDorb::core::comm::iiop::Request:
        {
          TIDorb::core::comm::iiop::GIOPRequestMessage* message = 
            new TIDorb::core::comm::iiop::GIOPRequestMessage(header);    
          
          if (message->hasBody()) {
            try {
              message->receive_body(this, (unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()));
            } catch (...) {
              delete message;
              throw;
            }
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
        case TIDorb::core::comm::iiop::Reply:
        {
          TIDorb::core::comm::iiop::GIOPReplyMessage* message = 
            new TIDorb::core::comm::iiop::GIOPReplyMessage(header);    
          
          if (message->hasBody()) {
            try {
              message->receive_body(this, (unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()));
            } catch (...) {
              delete message;
              throw;
            }
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
        case TIDorb::core::comm::iiop::CancelRequest:
        {
          TIDorb::core::comm::iiop::GIOPCancelRequestMessage* message = 
            new TIDorb::core::comm::iiop::GIOPCancelRequestMessage(header);    
          
          if (message->hasBody()) {
            try {
              message->receive_body(this, (unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()));
            } catch (...) {
              delete message;
              throw;
            }
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
        case TIDorb::core::comm::iiop::LocateRequest:
        {
          TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message = 
            new TIDorb::core::comm::iiop::GIOPLocateRequestMessage(header);    
          
          if (message->hasBody()) {
            try {
              message->receive_body(this, (unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()));
            } catch (...) {
              delete message;
              throw;
            }
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
        case TIDorb::core::comm::iiop::LocateReply:
        {
          TIDorb::core::comm::iiop::GIOPLocateReplyMessage* message = 
            new TIDorb::core::comm::iiop::GIOPLocateReplyMessage(header);    
          
          if (message->hasBody()) {
            try {
              message->receive_body(this, (unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()));
            } catch (...) {
              delete message;
              throw;
            }
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
        case TIDorb::core::comm::iiop::Fragment:
        {
          TIDorb::core::comm::iiop::GIOPFragmentMessage* message = 
            new TIDorb::core::comm::iiop::GIOPFragmentMessage(header);    
          
          if (message->hasBody()) {
            try {
              message->receive_body(this, (unsigned char*) (receive_header_buffer->get_chunk(0)->get_buffer()));
            } catch (...) {
              delete message;
              throw;
            }
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
        case TIDorb::core::comm::iiop::CloseConnection:
        {
          close_by_pair();
          return;
        }
        case TIDorb::core::comm::iiop::MessageError:
        {
          close_by_error(CORBA::COMM_FAILURE("Connection closed due to pair message error."));
          return;
        }
      }
    } catch (const CORBA::COMM_FAILURE& comm) {
      close_by_error(comm);
      return;

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
  }
//EMLG  
}




void TIDorb::core::comm::Connection::manage_message(TIDorb::core::comm::iiop::GIOPFragmentMessage* message)
{
  //  TIDorb::core::comm::iiop::RequestIdHolder id;
  TIDorb::core::comm::iiop::RequestId id;

  if(TIDorb::core::comm::iiop::Version::VERSION_1_2 == message->getHeader().getVersion())
    id = message->getRequestId();
  else
    id = current_request_id;

  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": GIOP Fragment Message has been received";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  // add_fragment return the complete message is finished
  
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* fragmented_message = 
    uncompleted_messages.add_fragment(MessageId(this, id), message);  

  if(fragmented_message) {
    
    switch(fragmented_message->getHeader().getMsgType())
    {
      case TIDorb::core::comm::iiop::Request:
        dispatch_request((TIDorb::core::comm::iiop::GIOPRequestMessage*)fragmented_message->_impl());
        break;
      case TIDorb::core::comm::iiop::LocateRequest:
        dispatch_request((TIDorb::core::comm::iiop::GIOPLocateRequestMessage*)fragmented_message->_impl());
        break;

      // CLIENT MESSAGES
      case TIDorb::core::comm::iiop::Reply:
        service_context_received( ((TIDorb::core::comm::iiop::GIOPReplyMessage*)fragmented_message->_impl())->get_service_context_list() );
      case TIDorb::core::comm::iiop::LocateReply:
        current_request_id = 0;
        lock_list.put_reply(id, fragmented_message);                
        break;
      default:
        break;

    } 
  }
}




void TIDorb::core::comm::Connection::manage_message(TIDorb::core::comm::iiop::GIOPReplyMessage* message)
{
  if(mode == SERVER_MODE) {
    close_by_error(CORBA::COMM_FAILURE("Reply has been received by Server Connection"));
    return;
  }

  TIDorb::core::comm::iiop::RequestId id;

  id = message->getRequestId();

  // pra@tid.es - FT extensions
  //if (id.value() == heartbeart_req_id) {
  if (id == heartbeart_req_id) {
    heartbeat_time = TIDorb::core::util::Time::currentTimeMillis() - heartbeat_time;
    heartbeart_req_id = TIDorb::core::util::Counter::RESERVED;
    delete message;
    return;
  }  	
  // end FT extensions

  if(message->getHeader().hasMoreFragments()) {
        

    //version 1.1 fragmentation: fragments are received sequentialy
    if(TIDorb::core::comm::iiop::Version::VERSION_1_1 == message->getHeader().getVersion()) {
      current_request_id = id;
    }
    
    uncompleted_messages.put_message(MessageId(this, id),message);
    
  } else {
    service_context_received(message->get_service_context_list());
    lock_list.put_reply(id, message);
  }
}




void TIDorb::core::comm::Connection::manage_message(TIDorb::core::comm::iiop::GIOPLocateReplyMessage* message)
{
  if(mode == SERVER_MODE) {
    close_by_error(CORBA::COMM_FAILURE("LocateReply has been received by Server Connection"));
    return;
  }

  TIDorb::core::comm::iiop::RequestId id = message->getRequestId();


  if(message->getHeader().hasMoreFragments()) {

    //version 1.1 fragmentation: fragments are received sequentialy
    if(TIDorb::core::comm::iiop::Version::VERSION_1_1 <= message->getHeader().getVersion())
    {
      delete message;
      close_by_error(CORBA::COMM_FAILURE("LocateReply cannot be fragmented in GIOP 1.1.", 0, CORBA::COMPLETED_NO));      
      return;
    }

    uncompleted_messages.put_message(MessageId(this, id),message);
    
  } else {
    lock_list.put_reply(id, message);
  }
}




void TIDorb::core::comm::Connection::manage_message(TIDorb::core::comm::iiop::GIOPCancelRequestMessage* message)
{
  if(mode == CLIENT_MODE) {
    close_by_error(CORBA::COMM_FAILURE("CancelRequest has been received by Client Connection"));
    return;
  }

  TIDorb::core::comm::iiop::RequestId id = message->getRequestId();

  uncompleted_messages.remove(MessageId(this,id));
}




void TIDorb::core::comm::Connection::manage_message(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message)
{
  if(mode == CLIENT_MODE) {
    close_by_error(CORBA::COMM_FAILURE("Request has been received by Client Connection"));
    return;
  }

  if(message->getHeader().hasMoreFragments()) {
    if(TIDorb::core::comm::iiop::Version::VERSION_1_1 <= message->getHeader().getVersion())
    {
      delete message;
      close_by_error(CORBA::COMM_FAILURE("LocateRequest cannot be fragmented in GIOP 1.1.", 0, CORBA::COMPLETED_NO));
      return;
    }
    TIDorb::core::comm::iiop::RequestId id = message->getRequestId();
    uncompleted_messages.put_message(MessageId(this, id),
                                     message);

  } else {
    dispatch_request(message);
  }
}


void TIDorb::core::comm::Connection::manage_message(TIDorb::core::comm::iiop::GIOPRequestMessage* message)
{
  if(mode == CLIENT_MODE) {
    close_by_error(CORBA::COMM_FAILURE("Request has been received by Client Connection"));
    return;
  }

  if(message->getHeader().hasMoreFragments()) {
    TIDorb::core::comm::iiop::RequestId id = message->getRequestId();

    //version 1.1 fragmentation: fragments are received sequentialy
    if(TIDorb::core::comm::iiop::Version::VERSION_1_1 == message->getHeader().getVersion())
    {
      if(current_request_id != 0) { // Fragment message was expected
        delete message;
        close_by_error(CORBA::COMM_FAILURE("Fragment message expected, but Reply received.", 0, CORBA::COMPLETED_NO));
        return;
      }
      current_request_id = id;
    }

    uncompleted_messages.put_message(MessageId(this, id),
                                     message);
 
  } else {
    dispatch_request(message);
  }
}

bool 
TIDorb::core::comm::Connection::send_locate_request(TIDorb::core::iop::IOR* ior,
                                                    const TIDorb::core::PolicyContext& policy_context)
//PRA
throw (TIDorb::core::comm::RECOVERABLE_COMM_FAILURE, 
       TIDorb::core::ForwardRequest,CORBA::SystemException)
//EPRA
{
  return send_locate_request(ior, TIDorb::core::comm::iiop::KeyAddr, policy_context);
}




bool TIDorb::core::comm::Connection::send_locate_request
(TIDorb::core::iop::IOR* ior,
 TIDorb::core::comm::iiop::AddressingDisposition disposition,
 const TIDorb::core::PolicyContext& policy_context)
  throw(TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,
        CORBA::SystemException)
{

  CORBA::ULongLong timeout = max_response_blocked_time;

  if (qos_enabled)
    timeout = TIDorb::core::messaging::QoS::checkRequestTime(_orb, policy_context);

  // verify if the request can be sent
  state.verify_request();

  const TIDorb::core::comm::iiop::Version& version = ior->GIOPVersion();
  TIDorb::core::comm::iiop::RequestId id = generateId();

  // create message
  TIDorb::core::comm::iiop::GIOPLocateRequestMessage message(version, id);

  Lock& lock = lock_list.activate_lock(id);
  
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Sending LocationRequest " << id;
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  message.insert_address(manager->orb(), *ior, disposition);
  
  send_message(message, id);
  
  //jagd requests_invoked.inc();
  requests_invoked++;

  // block the thread waiting for response
  lock.wait_for_completion(timeout);

  
  //requests_invoked.dec();
  requests_invoked--;
    
  if (lock.is_completed()) {
    
    // deletes the message at the end
    auto_ptr<TIDorb::core::comm::iiop::GIOPFragmentedMessage>
        fragmented_message(lock.consume_reply());
        
    lock_list.deactivate_lock(id);
    
    if(fragmented_message.get() == NULL) {
      throw CORBA::INTERNAL("No LocateReply message", 0, CORBA::COMPLETED_NO);
    }
    if(fragmented_message->getHeader().getMsgType()!= TIDorb::core::comm::iiop::LocateReply) {
      throw CORBA::MARSHAL("No LocateReply message received",  0, CORBA::COMPLETED_NO);
    }

//MLG
    TIDorb::core::comm::iiop::GIOPLocateReplyMessage* reply_message =
      (TIDorb::core::comm::iiop::GIOPLocateReplyMessage*)fragmented_message->_impl();
//EMLG

    switch(reply_message->reply_status()) {
      case TIDorb::core::comm::iiop::UNKNOWN_OBJECT:      
        return false;
      case TIDorb::core::comm::iiop::OBJECT_HERE:
        return true;
      case TIDorb::core::comm::iiop::OBJECT_FORWARD:        
        throw TIDorb::core::ForwardRequest(reply_message->extract_forward());
      case TIDorb::core::comm::iiop::OBJECT_FORWARD_PERM:
        // EMLG
        throw TIDorb::core::ForwardRequest(reply_message->extract_forward_perm());
      case TIDorb::core::comm::iiop::LOC_SYSTEM_EXCEPTION:
      {
        CORBA::SystemException* exception = reply_message->extract_system_exception();
        exception->_raise();
        delete exception;
        break;
      }
      case TIDorb::core::comm::iiop::LOC_NEEDS_ADDRESSING_MODE:
        return send_locate_request(ior,reply_message->extract_addressing_disposition(),
                                   policy_context);
    }

  } else {
    
    lock_list.deactivate_lock(id);
    
    switch(state.get_value()) {
      case ConnectionState::CLOSING_STATE:
        throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(CORBA::COMM_FAILURE("Connection closed by pair", 0,
                                                           CORBA::COMPLETED_NO));
      case ConnectionState::ERROR_STATE:
        state.get_error()->_raise();
      default:
        throw CORBA::NO_RESPONSE();
    }
  }

  return false;
}




void TIDorb::core::comm::Connection::send_oneway_request_async
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior)
        throw (TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,CORBA::SystemException)
{
  send_oneway_request_async(request, ior, TIDorb::core::comm::iiop::KeyAddr);
}




void TIDorb::core::comm::Connection::send_oneway_request_async
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

  Compression::CompressorIdLevel compressor(0, 0);
  TIDorb::core::PolicyContext* policy_context = NULL;

  if (qos_enabled) {

    policy_context = request->getPolicyContext();
    
    if (ior->is_ZIOP() || assume_ziop_server) {

      TIDorb::core::PolicyContext* policies_context_ior = NULL;
      policies_context_ior = ior->policies();

      compressor = TIDorb::core::ziop::Ziop::getClientCompressor(
                                                              *policy_context,
                                                              *policies_context_ior,
                                                              assume_ziop_server);

    }
    
  }


  // create message
  TIDorb::core::comm::iiop::GIOPRequestMessage message(version, id);

  try {

    set_service_context_list(&message, NULL);

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
      msg << "Connection::send_oneway_request_async catch CORBA::SystemException: ";
      msg << ex.what();
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    throw;
  } catch (const TIDThr::SystemException& threx) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected TIDThr exception: " << threx.what();
      msg << " at Connection::send_oneway_request_sync";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (const exception& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception: " << ex.what();
      msg << " at Connection::send_oneway_request_sync";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception raised.";
      msg << " at Connection::send_oneway_request_sync";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }
}




void TIDorb::core::comm::Connection::send_oneway_request_sync
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior)
//PRA
        throw(TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException)
//EPRA
{
  send_oneway_request_sync(request, ior, TIDorb::core::comm::iiop::KeyAddr);
}




void TIDorb::core::comm::Connection::send_oneway_request_sync
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior,
   TIDorb::core::comm::iiop::AddressingDisposition disposition)
        throw(TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException)
{
  try {

  CORBA::ULongLong timeout = max_response_blocked_time;
  Compression::CompressorIdLevel compressor(0, 0);
  TIDorb::core::PolicyContext* policy_context = NULL;

  if (qos_enabled) {

    policy_context = request->getPolicyContext();

    if (policy_context != NULL)
      timeout = TIDorb::core::messaging::QoS::checkRequestTime(_orb, *policy_context);

    if (ior->is_ZIOP() || assume_ziop_server) {

      TIDorb::core::PolicyContext* policies_context_ior = NULL;
      policies_context_ior = ior->policies();

      compressor = TIDorb::core::ziop::Ziop::getClientCompressor(
                                                              *policy_context,
                                                              *policies_context_ior,
                                                              assume_ziop_server);

    }

  }
  
  // verify if the request can be sent
  state.verify_request();

  TIDorb::core::comm::iiop::RequestId id = request->get_id();
  
  Lock& lock = lock_list.activate_lock(id);
  
  const TIDorb::core::comm::iiop::Version& version = ior->GIOPVersion();

  // create message
  TIDorb::core::comm::iiop::GIOPRequestMessage message(version, id);

  try {

    set_service_context_list(&message, policy_context);

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
  }

  //jagd requests_invoked.inc();
  requests_invoked++;

  // block the thread waiting for response
  lock.wait_for_completion(timeout);
  
  //jagd requests_invoked.dec();
  requests_invoked--;


  if (lock.is_completed()) {
    // deletes the message at the end
    auto_ptr<TIDorb::core::comm::iiop::GIOPFragmentedMessage>
        fragmented_message(lock.consume_reply());
        
        lock_list.deactivate_lock(id);

    if (fragmented_message.get() == NULL)
      throw CORBA::INTERNAL("No Reply message");
    if (fragmented_message->getHeader().getMsgType() != TIDorb::core::comm::iiop::Reply) {        
        throw CORBA::MARSHAL("No LocateReply message received");
    }

//MLG
    TIDorb::core::comm::iiop::GIOPReplyMessage* reply_message =
      (TIDorb::core::comm::iiop::GIOPReplyMessage*)fragmented_message->_impl();
//EMLG
      
    switch (reply_message->reply_status()) {
    	
      case TIDorb::core::comm::iiop::NO_EXCEPTION: // traza
      case TIDorb::core::comm::iiop::USER_EXCEPTION: // traza
      case TIDorb::core::comm::iiop::SYSTEM_EXCEPTION: // traza        
        break; // nothing to do, it is a oneway request
      case TIDorb::core::comm::iiop::LOCATION_FORWARD:
        throw TIDorb::core::ForwardRequest(reply_message->extract_forward());
      case TIDorb::core::comm::iiop::LOCATION_FORWARD_PERM:
        throw TIDorb::core::ForwardRequest(reply_message->extract_forward());
      case TIDorb::core::comm::iiop::NEEDS_ADDRESSING_MODE:
        send_oneway_request_sync(request,ior, reply_message->extract_addressing_disposition());
    }
  } else {
    lock_list.deactivate_lock(id);
  }

  } catch (const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& ex ) {
    throw;
  } catch (const TIDorb::core::ForwardRequest& ex ) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Connection::send_oneway_request_sync catch TIDorb::core::ForwardRequest: ";
      msg << ex.what();
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    throw;
  } catch (const CORBA::SystemException& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Connection::send_oneway_request_sync catch CORBA::SystemException: ";
      msg << ex.what();
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    throw;
  } catch (const TIDThr::SystemException& threx) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected TIDThr exception: " << threx.what();
      msg << " at Connection::send_oneway_request_sync";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (const exception& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception: " << ex.what();
      msg << " at Connection::send_oneway_request_sync";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception raised.";
      msg << " at Connection::send_oneway_request_sync";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }

}




void TIDorb::core::comm::Connection::send_request
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior)
        throw(TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException)
{
  send_request(request, ior, TIDorb::core::comm::iiop::KeyAddr);
}




void TIDorb::core::comm::Connection::send_request
  (TIDorb::core::RequestImpl* request,
   TIDorb::core::iop::IOR* ior,
   TIDorb::core::comm::iiop::AddressingDisposition disposition)
        throw(TIDorb::core::comm::RECOVERABLE_COMM_FAILURE,
              TIDorb::core::ForwardRequest,CORBA::SystemException)
{
  try {

  TIDThr::Synchronized sync(*this);




  // verify if the request can be sent
  state.verify_request();

  const TIDorb::core::comm::iiop::Version& ior_ver = ior->GIOPVersion();
  
  CORBA::ULongLong timeout = max_response_blocked_time;
  Compression::CompressorIdLevel compressor(0, 0);
  TIDorb::core::PolicyContext* policy_context = NULL;

  if (qos_enabled) {
    
    policy_context = request->getPolicyContext();
    
    if ( policy_context != NULL) 
      timeout = TIDorb::core::messaging::QoS::checkRequestTime(_orb, *policy_context); 

    if (ior->is_ZIOP() || assume_ziop_server) {

      TIDorb::core::PolicyContext* policies_context_ior = NULL;
      policies_context_ior = ior->policies();

      compressor = TIDorb::core::ziop::Ziop::getClientCompressor(
                                                              *policy_context,
                                                              *policies_context_ior,
                                                              assume_ziop_server);

    }
    
  }


  TIDorb::core::comm::iiop::RequestId id = generateId();
  
  request->set_id(id);

  TIDorb::core::comm::iiop::GIOPRequestMessage message(MIN(ior_ver,orb_ver), id);

  Lock& lock = lock_list.activate_lock(id);
 
  if (_orb->trace != NULL) {
    TIDorb::util::StringBuffer msg;
    msg << toString() << ": Sending request id(" << id
        << ") operation \"" << request->operation() << "\"";
    _orb->print_trace(TIDorb::util::TR_DEEP_DEBUG, msg.str().data());
  }

  try {

    set_service_context_list(&message, policy_context);

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
  }

  request->set_completed(CORBA::COMPLETED_MAYBE);

  //jagd requests_invoked.inc();
  requests_invoked++;

  // block the thread waiting for response
  lock.wait_for_completion(timeout); 
  
  //jagd requests_invoked.dec();
  requests_invoked--;
  
  if(lock.is_completed()) {

    // deletes the message at the end
    auto_ptr<TIDorb::core::comm::iiop::GIOPFragmentedMessage>
        fragmented_message(lock.consume_reply());
    
    lock_list.deactivate_lock(id);
    
    if(fragmented_message.get() == NULL) {
      throw CORBA::INTERNAL("No Reply message");
    }
    if(fragmented_message->getHeader().getMsgType() != TIDorb::core::comm::iiop::Reply) {
      throw CORBA::MARSHAL("No Reply message received");
    }
//MLG
    TIDorb::core::comm::iiop::GIOPReplyMessage* reply_message =
     (TIDorb::core::comm::iiop::GIOPReplyMessage*)fragmented_message->_impl();
//EMLG
    request->set_completed(CORBA::COMPLETED_YES);

    switch (reply_message->reply_status()) {
      case TIDorb::core::comm::iiop::NO_EXCEPTION:
        reply_message->extract_arguments(request);
        break;
      case TIDorb::core::comm::iiop::USER_EXCEPTION:
        {
        CORBA::Any_var user_exc = 
          reply_message->extract_user_exception(request->exceptions());
        request->set_user_exception(*user_exc);
        //request->set_user_exception(*reply_message->extract_user_exception(request->exceptions()));
        }
        break;
      case TIDorb::core::comm::iiop::SYSTEM_EXCEPTION:
        request->set_system_exception(reply_message->extract_system_exception());
        break;
      case TIDorb::core::comm::iiop::LOCATION_FORWARD:
        throw TIDorb::core::ForwardRequest(reply_message->extract_forward());
      case TIDorb::core::comm::iiop::LOCATION_FORWARD_PERM:
        throw TIDorb::core::ForwardRequest(reply_message->extract_forward_perm());
      case TIDorb::core::comm::iiop::NEEDS_ADDRESSING_MODE:
        send_request(request, ior, reply_message->extract_addressing_disposition());
    }

  } else {
    
      lock_list.deactivate_lock(id);

    switch(state.get_value()) {
      case ConnectionState::CLOSING_STATE:
        request->set_completed(CORBA::COMPLETED_MAYBE);
        throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(CORBA::COMM_FAILURE("Connection closed by pair"));
      case ConnectionState::ERROR_STATE:
        state.get_error()->_raise();
      default:
        throw CORBA::NO_RESPONSE();
    }
  }

  } catch (const TIDorb::core::comm::RECOVERABLE_COMM_FAILURE& ex ) {
    throw;
  } catch (const TIDorb::core::ForwardRequest& ex ) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Connection::send_request catch TIDorb::core::ForwardRequest: ";
      msg << ex.what();
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    throw;
  } catch (const CORBA::SystemException& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Connection::send_request catch CORBA::SystemException: ";
      msg << ex.what();
      _orb->print_trace(TIDorb::util::TR_USER, msg.str().data());
    }
    throw;
  } catch (const TIDThr::SystemException& threx) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected TIDThr exception: " << threx.what();
      msg << " at Connection::send_request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (const exception& ex) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception: " << ex.what();
      msg << " at Connection::send_request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  } catch (...) {
    if (_orb->trace != NULL) {
      TIDorb::util::StringBuffer msg;
      msg << "Unexpected exception raised.";
      msg << " at Connection::send_request";
      _orb->print_trace(TIDorb::util::TR_ERROR, msg.str().data());
    }
  }

}




void TIDorb::core::comm::Connection::send_reply(TIDorb::core::ServerRequestImpl* request,
                                                TIDorb::core::PolicyContext* policy_context)
{
  //jagd requests_in_POA.dec();
  requests_in_POA--;

  // verify if the request can be sent
  state.verify_reply();

  TIDorb::core::comm::iiop::RequestId id = request->get_id();

  // create message
  TIDorb::core::comm::iiop::GIOPReplyMessage message(request->getVersion(), id);

  if(send_bidirectional_service) {
    message.set_service_context_list(bidirectional_service);
    bidirectional_service = NULL;
    send_bidirectional_service = false;
  }

  Compression::CompressorIdLevel compressor = request->get_compressor();

  message.insert_result_request(_orb, request);  

  if (compressor.compressor_id != Compression::COMPRESSORID_NONE) {
    
    CORBA::ULong low_value = 0;
    ZIOP::CompressionMinRatioPolicyValue min_ratio = 0;

    if (policy_context != NULL) {
      low_value = TIDorb::core::ziop::Ziop::getLowValue(policy_context);
      min_ratio = TIDorb::core::ziop::Ziop::getMinRatio(policy_context); 
      compressor.compression_level = 
        TIDorb::core::ziop::Ziop::getCompressionLevel(compressor.compressor_id,
                                                      policy_context);
    }

    TIDorb::core::comm::ziop::ZIOPMessage ziop_message(message);
    
    if (ziop_message.perform_compression(_orb, compressor, 
                                         low_value, min_ratio)) 
      send_message(ziop_message, id);
    else
      send_message(message, id);

  } else {

    send_message(message, id);

  }
}




void TIDorb::core::comm::Connection::send_locate_reply
  (const TIDorb::core::comm::iiop::Version& version,
   TIDorb::core::comm::iiop::RequestId id,
   bool here)
{
  //requests_in_POA.dec();
  requests_in_POA--;

  // verify if the request can be sent
  state.verify_reply();

  // create message
  TIDorb::core::comm::iiop::GIOPLocateReplyMessage message(version, id);

  if(here)
    message.insert_object_here(_orb);
  else
    message.insert_unknown_object(_orb);

  send_message(message, id);
}




void TIDorb::core::comm::Connection::send_locate_reply
  (const TIDorb::core::comm::iiop::Version& version,
   TIDorb::core::comm::iiop::RequestId id,
   CORBA::Object_ptr obj)
{
  //jagd requests_in_POA.dec();
  requests_in_POA--;

  // verify if the request can be sent
  state.verify_reply();

  // create message
  TIDorb::core::comm::iiop::GIOPLocateReplyMessage message(version, id);

  message.insert_forward(_orb, obj);

  send_message(message, id);
}




void TIDorb::core::comm::Connection::send_locate_reply
  (const TIDorb::core::comm::iiop::Version& version,
   TIDorb::core::comm::iiop::RequestId id,
   const CORBA::SystemException& excep)
{
  //jagd requests_in_POA.dec();
  requests_in_POA--;

  // verify if the request can be sent
  state.verify_reply();

  // create message
  TIDorb::core::comm::iiop::GIOPLocateReplyMessage message(version, id);

  message.insert_system_exception(_orb, excep);

  send_message(message, id);
}




//
// pra@tid.es - FT extensions
//

void TIDorb::core::comm::Connection::send_heartbeat() throw(CORBA::SystemException)
{
  // Heartbeat request id
  heartbeart_req_id = generateId();

  // create message
  TIDorb::core::comm::iiop::GIOPRequestMessage message(
    TIDorb::core::comm::iiop::Version::VERSION_1_0, heartbeart_req_id);

  if(send_bidirectional_service) {
    message.set_service_context_list(bidirectional_service);
    bidirectional_service = NULL;
    send_bidirectional_service = false;
  }

  message.insert_heartbeat_request(_orb, heartbeart_req_id);

  heartbeat_time = TIDorb::core::util::Time::currentTimeMillis();
  send_message(message, heartbeart_req_id);
}

//
// end FT extensions
//



void TIDorb::core::comm::Connection::set_service_context_list(
                                           TIDorb::core::comm::iiop::GIOPRequestMessage* message,
                                           TIDorb::core::PolicyContext* policy_context)
{

  // Look for all Service Context
  TIDorb::core::comm::iiop::ServiceContextList* contexts = NULL;
  bool yielded = false;

  // Bidirectional Service Context
  if(send_bidirectional_service) {
    contexts = bidirectional_service;
    bidirectional_service = NULL;
    send_bidirectional_service = false;
  }

  
  // Invocation Service Context
  TIDorb::core::comm::iiop::InvocationPoliciesContext* invocationPolicies = NULL;

  if (policy_context != NULL) {
    invocationPolicies = policy_context->getInvocationPolicyServiceContext();        
  
    if(invocationPolicies != NULL) {
      
      if ( contexts == NULL) {
        contexts = new TIDorb::core::comm::iiop::ServiceContextList();
        yielded = true;
      }
      
      contexts->add(invocationPolicies);
    }
  }

  // Any more?
  
  if (contexts != NULL) {
    message->set_service_context_list(contexts, yielded);
  }        

}



void TIDorb::core::comm::Connection::service_context_received
  (const TIDorb::core::comm::iiop::ServiceContextList* services)
{
  if(services == NULL)
    return;

  for(CORBA::ULong i = 0; i < services->components.size(); i++) {
    const TIDorb::core::comm::iiop::ServiceContext* context = services->components[i];
    if ((context != NULL) &&  (context->_context_id == TIDorb::core::comm::iiop::BI_DIR_IIOP)) {
      //jagd
      //set_bidirectional_mode_by_peer(dynamic_cast<const TIDorb::core::comm::iiop::BiDirServiceContext*>(context));
      set_bidirectional_mode_by_peer((const TIDorb::core::comm::iiop::BiDirServiceContext*)(context));
    }
  }
}

bool TIDorb::core::comm::Connection::has_pending_requests()
{
    TIDThr::Synchronized synchro(*this);
    
    if(mode == CLIENT_MODE) {
      //jagd return requests_invoked.non_zero();
      return requests_invoked;
    } else {
      //return (requests_invoked.non_zero()) || (requests_in_POA.non_zero());
      return ((requests_invoked) || (requests_in_POA));
    }  
}




TIDorb::core::comm::iiop::RequestId TIDorb::core::comm::Connection::generateId()
{
  return (TIDorb::core::comm::iiop::RequestId) id_count.post_inc();
}




void TIDorb::core::comm::Connection::read(unsigned char* buffer, size_t buffer_size, size_t length)
{
  read(buffer, buffer_size, 0, length);
}






void TIDorb::core::comm::Connection::write(unsigned char* buffer, size_t buffer_size, size_t length)
{
  write(buffer, buffer_size, 0, length);
}




void TIDorb::core::comm::Connection::write_Version_1_0(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                                       const TIDorb::core::comm::iiop::RequestId request_id)

{
  write_Version_1_1(message, request_id); // message has only 1 chunk
}




void TIDorb::core::comm::Connection::write_Version_1_1(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                                       const TIDorb::core::comm::iiop::RequestId request_id)
{
  TIDorb::core::cdr::BufferCDR* buffer = message.get_message_buffer();
  size_t num_chunks = buffer->get_num_available_chunks();

  TIDorb::core::cdr::ChunkCDR* chunk = NULL;

  {
    TIDThr::Synchronized synchro(write_mutex);

    for(size_t i = 0; i < num_chunks; i++)
    {
      chunk = buffer->get_chunk(i);
      write((unsigned char*)chunk->get_buffer(), chunk->get_length(), 0, chunk->get_available_bytes());

      if (_orb->trace != NULL) {
        TIDorb::util::StringBuffer msg;
        msg << "GIOP message chunk sent - HEXDUMP " << chunk->get_available_bytes() << " bytes";
        _orb->print_trace(TIDorb::util::TR_DUMP, msg.str().data());
        _orb->print_dump(TIDorb::util::TR_DUMP, (const unsigned char*)chunk->get_buffer(), 
                         chunk->get_available_bytes());
      }
    }
  }
}




void TIDorb::core::comm::Connection::write_Version_1_2(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                                       const TIDorb::core::comm::iiop::RequestId request_id)
{
  write_Version_1_1(message, request_id);
}




TIDorb::core::cdr::ChunkCDR*
TIDorb::core::comm::Connection::receive_chunk
  (const TIDorb::core::comm::iiop::GIOPHeader& header,
   const CORBA::Octet* header_bytes)
{
  CORBA::ULong body_size = header.getSize();
  CORBA::ULong chunk_size = body_size + TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE;
  TIDorb::core::cdr::ChunkCDR* chunk = new TIDorb::core::cdr::ChunkCDR(chunk_size);

  unsigned char* buffer = (unsigned char*) chunk->get_buffer();

  // copy the header
  memcpy(buffer, header_bytes, TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);

  read(buffer, chunk_size, TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE, body_size);

  chunk->set_available_bytes(chunk_size);

  return chunk;
}
