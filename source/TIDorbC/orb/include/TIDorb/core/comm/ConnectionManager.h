/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/ConnectionManager.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ConnectionManager_H_
#define _TIDORB_core_comm_ConnectionManager_H_ 1


#include "TIDSocket.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

#include "TIDorb/core/comm/LockPool.h"

/**
 * Manages the opened connections. When a connection is needed, try to reuse an opened one.
 * When there is too many connection opened or there are some connection that have remained
 * idle too long, they will be closed.
 *
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @author Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace comm {

class ConnectionManager : public TIDThr::RecursiveMutex
{
  protected:    	
    TIDorb::core::TIDORB* _orb;

  private:   
  
  
  /**
   * LockPool for connections
   */
   
   LockPool lock_pool;
   
   /**
     *  Table with the uncompleted messages that has more fragments.
     */
     
   FragmentedMessagesRepository uncompleted_messages;
   	
  // pra@tid.es - FT extensions
  friend class HeartbeatThread;
  // end FT extensions

  /**
   * Maintains a <code>UseTable</code> with <code>Connection</code> objects. Then, when
   * the table removes older connections, the manager close them.
   */

  typedef TIDorb::core::util::UseTable<TIDorb::core::comm::ConnectionDescriptor,
                                       TIDorb::core::comm::Connection_ref> ConnectionTable;
  ConnectionTable connections;

  /**
   * Opened client connections. This table allows reuse opened connections
   * for a given listen point.<p>
   * This is a <code>Hashtable</code>, the stored object class is <code>Connection</code>
   * and the keys are <code>ListenPoint</code>s.
   */

  typedef map<TIDorb::core::comm::iiop::ListenPoint,
              TIDorb::core::comm::Connection_ref*> ConnectionMapT;
  typedef ConnectionMapT::iterator ConnectionMapIteratorT;
  ConnectionMapT client_connections;

  /**
   * Opened bidirectional connections. This table allows reuse opened connections
   * for a given listen point.<p>
   * This is a <code>Hashtable</code>, the stored object class is <code>Connection</code>
   * and the keys are <code>ListenPoint</code>s.
   */

   ConnectionMapT bidirectional_connections;

  /**
   * Connections that are opening at this moment
   */
   typedef map<TIDorb::core::comm::iiop::ListenPoint, 
               OpeningLock_ref> ConnectionsOpening;
               
   ConnectionsOpening connections_opening;


   ConnectionMapT miop_connections;


   TIDThr::ThreadGroupHandle _group;
   HeartbeatThread*          _thread;


   protected:
    /**
    * The ORB has been destroyed.
    */
   bool destroyed;

   public:
    ConnectionManager(TIDorb::core::TIDORB* orb,
                      ::FT::HeartbeatPolicyValue& client_heartbeat_policy)
      throw (TIDThr::SystemException);

    ~ConnectionManager() throw (TIDThr::SystemException);
    	
    TIDorb::core::TIDORB* orb() { return _orb; }    	
    
    /**
     *  Gets the LockPool
     */
     
     LockPool& get_lock_pool() { return lock_pool;}
     
     FragmentedMessagesRepository& get_uncompleted_messages()
     {
        return uncompleted_messages;
     }
     
     
  /**
   * Notifies the connection use.
   * @param conn the connection in use
   */
  void use(Connection* conn);

  /**
   * The manager is Notified a connection is been closing.
   * @param conn the connection that is closing.
   */
  void closing(Connection* conn);

  /**
   * When a new Socket is created (a new connection has been accepted by the
   * <code>ServerSocket</code>) a new Connection, in SERVER mode, must be registered.
   * @param socket the new socket.
   */
  void createServerConnection(TIDSocket::Socket* socket);

  /**
   * Saves the <code>ListenPoint</code> associated to a bidirectional connection to
   * be used as a client connection when a connection to the listen point will be required.
   * @param listen_point the <code>ListenPoint</code> where the connection will be seen as a client connection.
   * @param conn the bidirectional connection.
   */

  void add_bidirectional_connection(const TIDorb::core::comm::iiop::ListenPoint& listen_point,
                                    Connection* conn);

  /**
   * Looks for a client connection with the listen point. If it does not exist, then the creates one.
   * @param listen_point the <code>ListenPoint</code> that determines a remote ORB in a Object reference.
   */
  Connection* getClientConnection(const TIDorb::core::comm::iiop::ListenPoint& listen_point,
                                  TIDorb::core::PolicyContext* policy_context);

    /**
   * Looks for a client connection with the listen point. If it does not exist, then the creates one.
   * @param listen_point the <code>ListenPoint</code> that determines a remote ORB in a Object reference.
   */
  void prepareClientConnection(const TIDorb::core::comm::iiop::ListenPoint& listen_point,
                               TIDorb::core::PolicyContext* policy_context);


  // pra@tid.es - MIOP extensions
  Connection* getMIOPClientConnection(const TIDorb::core::comm::miop::ListenPoint& listen_point);
  Connection* getMIOPServerConnection(const TIDorb::core::comm::miop::ListenPoint& listen_point);
  // end MIOP extensions


  protected:
  /**
   * Checks in a new connection for manage it.
   * @param conn the bidirectional connection.
   */
  void new_connection(Connection* conn);

  /**
   *  Creates a new client connection
   */
  Connection* open_client_connection(const TIDorb::core::comm::iiop::ListenPoint& listen_point,
                                     TIDorb::core::PolicyContext* policy_context);

  // pra@tid.es - MIOP extensions
  Connection* open_miop_client_connection(const TIDorb::core::comm::miop::ListenPoint& listen_point);
  Connection* open_miop_server_connection(const TIDorb::core::comm::miop::ListenPoint& listen_point);
  // end MIOP extensions


  /**
   * Removes from the client connection table the listen points associted to a connection.
   * @param conn the connection.
   */
  void remove_listen_points(TIDorb::core::comm::Connection* conn);

  /**
   * Close the connection due to it has been decided that it not has been used for a long time.
   * @param conn the connection.
   */
  void close_connection(Connection* conn);

     public:
  /**
   * @return <code>true</code> if it has not any active connection
   */
  bool active_connections();


  /**
   * Close all connections due to an ORB close session.
   */
  void destroy();
};

} //comm
} //core
} //TIDorb

#endif
