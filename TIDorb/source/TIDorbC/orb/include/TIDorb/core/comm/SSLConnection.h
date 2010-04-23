/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/SSLConnection.h
  
 Revisions:
 
 Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/ssliop.h"

#ifndef _TIDORB_core_comm_SSLConnection_H_
#define _TIDORB_core_comm_SSLConnection_H_ 1


/**
 * TIDorb SSL Connection: sends and receives IIOP Messages under GIOP protocol.
 *
 * Copyright 2009 Telefonica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Alvaro Vega 
 * @version 1.0
 */

#include <sys/types.h>
#include "TIDSocket.h"

namespace TIDorb {
namespace core {
namespace comm {


class SSLConnection : public Connection
{
protected:
  /**
   * Connection Socket.
   */
  TIDSocket::SSLSocket* socket;
  
  TIDorb::core::poa::POAImpl*    _root_POA;
  PortableServer::POAManager_ptr _poamanager;
  
public:
  
  /**
   * Connection Socket InputStream.
   */
  TIDSocket::InputStream* in;
  
  /**
   * Connection Socket OutputStream.
   */
  TIDSocket::OutputStream* out;
  

protected:
  /**
   * Register the connection at the connection manager and initialize the
   * connection socket with the ORB configuration values.
   * @see TIDorb::core::ConfORB
   */
  SSLConnection(ConnectionManager* mngr, TIDSocket::SSLSocket* socketed)
    throw (TIDThr::SystemException, CORBA::SystemException);
  
public:
  
  virtual ~SSLConnection() throw (TIDThr::SystemException);
  
  /**
   * Connection hash code.
   */
  CORBA::ULongLong hashCode();
  
  const char* toString();
  
  void close();
  
  void dispatch_request(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message);
  void dispatch_request(TIDorb::core::comm::iiop::GIOPRequestMessage* message);
  
  
  void read(unsigned char* buffer, size_t buffer_size, size_t offset, size_t length);
  void write(unsigned char* buffer, size_t buffer_size, size_t offset, size_t length);
  
  void set_verify_mode(int mode);

public:
  /**
   * Creates a new connection in SERVER_MODE.
   * @param mngr the manager
   * @param sock <code>Socket</code> created in the <code>accept</code> method
   * in a <code>ServerSocket</code> object.
   */
  static Connection* server_connection(ConnectionManager* mngr, 
                                       TIDSocket::SSLSocket* socketed);

  /**
   * Creates a new connection in CLIENT_MODE.
   */
  static Connection* client_connection(ConnectionManager* mngr, 
                                       const TIDorb::core::comm::iiop::ListenPoint& listen_point);
  
};

} //comm
} //core
} //TIDorb

#endif
