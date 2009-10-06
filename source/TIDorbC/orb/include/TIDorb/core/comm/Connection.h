#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_Connection_H_
#define _TIDORB_core_comm_Connection_H_ 1


/**
 * TIDorb Connection: sends and receives messages under the GIOP protocol.
 *
 * Copyright 2000 Telefonica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. Caceres
 * @version 1.0
 */

#include <sys/types.h>
#include "CORBA.h"
#include "FT.h"

#include "TIDorb/core/comm/Lock.h"
#include "TIDorb/core/comm/LockPool.h"
#include "TIDorb/core/comm/LockList.h"
#include "TIDorb/core/comm/FragmentedMessagesRepository.h"

#include <set>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

namespace TIDorb {
namespace core {
namespace comm {


class ConnectionDescriptor {

  public:
    ConnectionDescriptor() : m_value(0) {}
    ConnectionDescriptor(CORBA::ULong val) : m_value(val){}
    ConnectionDescriptor(const ConnectionDescriptor& other) : m_value(other.m_value){}

    ConnectionDescriptor& operator= (CORBA::ULong val)
    {
      m_value = val;
      return *this;
    }

    ConnectionDescriptor& operator= (const ConnectionDescriptor& other)
    {
      m_value = other.m_value;
      return *this;
    }

    bool operator== (const ConnectionDescriptor& other) const
      {return m_value == other.m_value;}
    bool operator> (const ConnectionDescriptor& other) const
      {return m_value > other.m_value;}
    bool operator >= (const ConnectionDescriptor& other) const
      {return m_value >= other.m_value;}
    bool operator< (const ConnectionDescriptor& other) const
      {return m_value < other.m_value;}
    bool operator <= (const ConnectionDescriptor& other) const
      {return m_value <= other.m_value;}
    bool operator!= (const ConnectionDescriptor& other) const
      {return m_value != other.m_value;}

    CORBA::ULongLong hashCode() {return m_value;}

  protected:
    CORBA::ULong m_value;
};


class Connection : public TIDThr::RecursiveMutex, public TIDorb::core::util::RemovableObject
{
  friend class TIDorb::core::comm::ziop::ZIOPMessage;

  public:
  
    typedef set<TIDorb::core::comm::iiop::ListenPoint> ListenPointSet;
    
    /**
     * Client connection mode.
     */
    static const int CLIENT_MODE = 0;

    /**
     * Server connection mode.
     */
    static const int SERVER_MODE = 1;

    /**
     * Bidirectional connection mode.
     */
    static const int BIDIRECTIONAL_MODE = 2;

    TIDThr::ThreadGroupHandle _group;

    /**
     * Connection mode.
     */
    int mode;

    /**
     * Connection state.
     */
    ConnectionState state;

    /**
     * Pending request invoked
     */
     //jagd no hace falta mutex para proteger el contador 
     //TIDorb::core::util::Counter requests_invoked;
    unsigned long requests_invoked;
     
    /**
     * Pending requests in POA.
     */
    //jagd aunque protejaramos el mutex quien llama a request in POA
    //no esta sincronizado asi que se produce el mismo efecto. 
    //TIDorb::core::util::Counter requests_in_POA;
    unsigned long requests_in_POA;

    /**
     * Current id count. The ids will be given sequentialy.
     */
    TIDorb::core::util::Counter id_count;

    /**
     * Connection identifier: the local port related to the connection socket.
     */
    int my_port;


    /**
     * ORB GIOP version.
     */
    TIDorb::core::comm::iiop::Version orb_ver;

    /**
     * Maximum time where a request is waiting for a response.
     */
    int max_response_blocked_time;

    /**
     * QoS enabled.
     */
    bool qos_enabled;

    /**
     * ZIOP: assume ZIOP on server side.
     */
    bool assume_ziop_server;


    /**
     * Server Object Listen Points assotiated.
     */        
     
     ListenPointSet addresses;
     
    //TIDorb::core::util::HashedLinkedList<TIDorb::core::comm::iiop::ListenPoint,
    //                                     TIDorb::core::comm::iiop::ListenPoint>* addresses;

    /**
     * First client address.
     */
    TIDorb::core::comm::iiop::ListenPoint initial_point;

    /**
     *  The ConnectionManager.
     */
//MLG: En un principio siempre hay manager    
    //ConnectionManager_ref manager;
    ConnectionManager* manager;
//EMLG    

    /**
     *  Lock list where the pending requests are waiting for response.
     */
    LockList lock_list;

    /**
     *  Table with the uncompleted messages that has more fragments.
     */
     
     FragmentedMessagesRepository& uncompleted_messages;
    
    /**
     * Maintains the current request id for fragmented message in GIOP 1.1
     */
    TIDorb::core::comm::iiop::RequestId current_request_id;

    /**
     * Bidirectional Service Context. It will be sent in the next Request/Response.
     */
    TIDorb::core::comm::iiop::ServiceContextList* bidirectional_service;

    /**
     * Says if the bidirectional context has to been sent or not.
     */
    bool send_bidirectional_service;

    /**
     * Write messages in socket mutex.
     */
    TIDThr::Mutex write_mutex;

    /**
     * Connection buffer for writing headers.
     */
    TIDorb::core::cdr::BufferCDR_ref send_header_buffer;

    /**
     * Connection buffer for receiving headers.
     */
    TIDorb::core::cdr::BufferCDR_ref receive_header_buffer;

    /**
     * The toString() return value.
     */
    char* str;


    const TIDorb::core::ConfORB& conf;

  protected:
    Connection(ConnectionManager* mngr)
      throw (TIDThr::SystemException, CORBA::SystemException);
	  TIDorb::core::TIDORB* _orb;		

  public:
    virtual ~Connection() throw (TIDThr::SystemException);

    TIDorb::core::TIDORB* orb()
		{
		  return _orb;
		}

    /**
     * Start the connection thread execution.
     */
    void init();

    /**
     * Connection hash code.
     */
    virtual CORBA::ULongLong hashCode() = 0;

    ConnectionDescriptor get_descriptor();
    bool equals(Connection* obj);

    virtual const char* toString() = 0;

    /**
     * Add a new listen point for whose this connection can be used to sending request.
     * This will be usefull if this connection is bidirectional.
     *@param listen_point the new listen point of an bidirectional connection
     */
    void add_listen_point(const TIDorb::core::comm::iiop::ListenPoint& listen_point);

    /**
     * @return the listen points associated to this connection
     */
    const ListenPointSet& get_listen_points()
    {
        return addresses;
    }

    /**
     * Changes the connection mode to BIDIRECTIONAL_MODE. This method is called by
     * the connection if it detects that a referenced object is defined in a
     * "Bidirectional POA". The contest will be sent in the next response.
     * @param context the list containing the <code>BiDirServiceContext</code>
     */
    void set_bidirectional_mode(TIDorb::core::comm::iiop::ServiceContextList* context);

    /**
     * Changes the connection mode to BIDIRECTIONAL_MODE. This method is called by
     * the connection if it detects a bidirectional context in a request or response
     * sent by the peer.
     * @param context the bidirectional context
     */
    void set_bidirectional_mode_by_peer(const TIDorb::core::comm::iiop::BiDirServiceContext* context);

    /**
     * Test whether the connection is still opened or not.
     * @return <code>true</code> if open or <code>false</code> if not.
     */
    bool is_open();

    /**
     * @return the communication error (if exits)
     */
    const CORBA::COMM_FAILURE* get_connection_error();

    /**
     * @return <code>true</code> if the communication is running in client mode
     */
    bool is_client_connection();

    /**
     * @return <code>true</code> if the communication is running in client mode
     */
    bool is_server_connection();

    /**
     * @return <code>true</code> if the communication is running in client mode
     */
    bool is_bidirectional_connection();

    /**
     * The connection has received a close connection.
     */
    void close_by_pair();

    /**
     * The connection will be closed by the connection manager.
     * @see TIDorb::core::comm::ConnectionManager
     */
    void close_by_manager();

    /**
     * The connection is closed due to the socket is broken.
     * @param error the broken communication error
     */
    void close_by_broken_connection(const CORBA::COMM_FAILURE& error);

    void close_by_error(const CORBA::COMM_FAILURE& error);

    virtual void send_close();

    virtual void send_error();

    void send_header(const TIDorb::core::comm::iiop::GIOPHeader& header);

  protected:
    void receive_header(TIDorb::core::comm::iiop::GIOPHeader& header);
    void send_message(const TIDorb::core::comm::iiop::GIOPHeader& header);

  public:
    virtual void receive_message();
    void send_message(const TIDorb::core::comm::iiop::GIOPMessage& message,
                      const TIDorb::core::comm::iiop::RequestId request_id);

    TIDorb::core::cdr::ChunkCDR* receive_chunk(const TIDorb::core::comm::iiop::GIOPHeader& header,
                                               const CORBA::Octet* header_bytes);

  protected:
    void manage_message(TIDorb::core::comm::iiop::GIOPFragmentMessage* message);
    void manage_message(TIDorb::core::comm::iiop::GIOPReplyMessage* message);
    void manage_message(TIDorb::core::comm::iiop::GIOPLocateReplyMessage* message);
    void manage_message(TIDorb::core::comm::iiop::GIOPCancelRequestMessage* message);
    void manage_message(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message);
    void manage_message(TIDorb::core::comm::iiop::GIOPRequestMessage* message);

    virtual void dispatch_request(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message) = 0;
    virtual void dispatch_request(TIDorb::core::comm::iiop::GIOPRequestMessage* message) = 0;

    virtual void close() = 0;

  public:
    bool send_locate_request(TIDorb::core::iop::IOR* ior,
                             const TIDorb::core::PolicyContext& policy_context)
//PRA
      throw (RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException);
//EPRA

  protected:
    bool send_locate_request(TIDorb::core::iop::IOR* ior,
                             TIDorb::core::comm::iiop::AddressingDisposition disposition,
                             const TIDorb::core::PolicyContext& policy_context)
      throw (RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException);

  public:
    virtual void send_oneway_request_async(TIDorb::core::RequestImpl* request,
                                           TIDorb::core::iop::IOR* ior)
      throw (RECOVERABLE_COMM_FAILURE,CORBA::SystemException);

    virtual void send_oneway_request_sync(TIDorb::core::RequestImpl* request,
                                          TIDorb::core::iop::IOR* ior)
//PRA
      throw (RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException);
//EPRA

  protected:
    virtual void send_oneway_request_async(TIDorb::core::RequestImpl* request,
                                           TIDorb::core::iop::IOR* ior,
                                           TIDorb::core::comm::iiop::AddressingDisposition disposition)
      throw (RECOVERABLE_COMM_FAILURE,CORBA::SystemException);

    virtual void send_oneway_request_sync(TIDorb::core::RequestImpl* request,
                                          TIDorb::core::iop::IOR* ior,
                                          TIDorb::core::comm::iiop::AddressingDisposition disposition)
      throw (RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException);

  public:
     void send_request(TIDorb::core::RequestImpl* request,
                       TIDorb::core::iop::IOR* ior)
       throw (RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest,CORBA::SystemException);

  protected:
     void send_request(TIDorb::core::RequestImpl* request,
                       TIDorb::core::iop::IOR* ior,
                       TIDorb::core::comm::iiop::AddressingDisposition disposition)
       throw (RECOVERABLE_COMM_FAILURE,TIDorb::core::ForwardRequest, CORBA::SystemException);

  public:
     void send_reply(ServerRequestImpl* request, TIDorb::core::PolicyContext* policy_context = NULL);
     void send_locate_reply(const TIDorb::core::comm::iiop::Version& version, TIDorb::core::comm::iiop::RequestId id, bool here);
     void send_locate_reply(const TIDorb::core::comm::iiop::Version& version, TIDorb::core::comm::iiop::RequestId id, CORBA::Object* obj);
     void send_locate_reply(const TIDorb::core::comm::iiop::Version& version, TIDorb::core::comm::iiop::RequestId id, const CORBA::SystemException& excep);

     // pra@tid.es - FT extensions
     CORBA::ULongLong                    heartbeat_time;
     TIDorb::core::comm::iiop::RequestId heartbeart_req_id;

     void send_heartbeat() throw(CORBA::SystemException);
     // end FT extensions


     void set_service_context_list(TIDorb::core::comm::iiop::GIOPRequestMessage* message,
                                   TIDorb::core::PolicyContext* policy_context);

     void service_context_received(const TIDorb::core::comm::iiop::ServiceContextList* services);

     bool canBeRemoved()
 		 {
			  return !has_pending_requests();
		 }

     bool has_pending_requests();

     TIDorb::core::comm::iiop::RequestId generateId();

     void read(unsigned char* buffer, size_t buffer_size, size_t length);
     virtual void read(unsigned char* buffer, size_t buffer_size, size_t offset, size_t length) = 0;

     void write(unsigned char* buffer, size_t buffer_size, size_t length);
     virtual void write(unsigned char* buffer, size_t buffer_size, size_t offset, size_t length) = 0;

/*
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAMBIAR PARA MANDAR UN MENSAJE CON VARIOS CHUNKS
*/
  public:
    virtual void write_Version_1_0(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                   const TIDorb::core::comm::iiop::RequestId request_id);
    virtual void write_Version_1_1(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                   const TIDorb::core::comm::iiop::RequestId request_id);
    virtual void write_Version_1_2(const TIDorb::core::comm::iiop::GIOPMessage& message,
                                   const TIDorb::core::comm::iiop::RequestId request_id);
};


//PRA
class Connection_ref : public TIDThr::HandleT<Connection>,
                       public TIDorb::core::util::RemovableObject
{
  public:
    Connection_ref(Connection* conn = NULL)
    {
        assign(conn);
    }

    bool canBeRemoved()
    {
        return getT()->canBeRemoved();
    }
    virtual ~Connection_ref() {}
};
//EPRA


} //comm
} //core
} //TIDorb

#endif
