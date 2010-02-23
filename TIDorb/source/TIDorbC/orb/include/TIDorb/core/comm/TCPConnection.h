/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/TCPConnection.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_TCPConnection_H_
#define _TIDORB_core_comm_TCPConnection_H_ 1


/**
 * TIDorb IIOP Connection: sends and receives IIOP Messages under GIOP protocol.
 *
 * Copyright 2000 Telefonica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. Caceres
 * @version 1.0
 */

#include <sys/types.h>
#include "TIDSocket.h"

namespace TIDorb {
namespace core {
namespace comm {


class TCPConnection : public Connection
{
  protected:
    /**
     * Connection Socket.
     */
    TIDSocket::Socket* socket;

    //jagd
    TIDorb::core::poa::POAImpl* _root_POA;
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
    TCPConnection(ConnectionManager* mngr, TIDSocket::Socket* socketed)
      throw (TIDThr::SystemException, CORBA::SystemException);

  public:

    virtual ~TCPConnection() throw (TIDThr::SystemException);

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

  public:
    /**
     * Creates a new connection in SERVER_MODE.
     * @param mngr the manager
     * @param sock <code>Socket</code> created in the <code>accept</code> method
     * in a <code>ServerSocket</code> object.
     */
    static Connection* server_connection(ConnectionManager* mngr, TIDSocket::Socket* socketed);

    /**
     * Creates a new connection in CLIENT_MODE.
     */
    static Connection* client_connection(ConnectionManager* mngr, const TIDorb::core::comm::iiop::ListenPoint& listen_point);

};

} //comm
} //core
} //TIDorb

#endif
