/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/MulticastConnection.h
  
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

#ifndef _TIDORB_core_comm_MulticastConnection_H_
#define _TIDORB_core_comm_MulticastConnection_H_ 1


/**
 * TIDorb multicast connection: sends and receives MIOP multicast messages.
 */

#include <sys/types.h>
#include "TIDSocket.h"

namespace TIDorb {
namespace core {
namespace comm {


class MulticastConnection : public UDPConnection
{
  protected:
    /**
     * Register the connection at the connection manager.
     */
    MulticastConnection(ConnectionManager* mngr, TIDSocket::MulticastSocket* s,
                        const char* _outgoing_interface)
      throw (TIDThr::SystemException, CORBA::SystemException);

  public:

    virtual ~MulticastConnection() throw (TIDThr::SystemException);

    const char* toString();

  public:
    /**
     * Creates a new connection in SERVER_MODE.
     */
    static Connection* server_connection(ConnectionManager* mngr, const TIDorb::core::comm::miop::ListenPoint& listen_point);

    /**
     * Creates a new connection in CLIENT_MODE.
     */
    static Connection* client_connection(ConnectionManager* mngr, const TIDorb::core::comm::miop::ListenPoint& listen_point);

};

} //comm
} //core
} //TIDorb

#endif
