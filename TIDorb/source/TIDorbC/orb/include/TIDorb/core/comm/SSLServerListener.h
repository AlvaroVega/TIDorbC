/* -----------------------------------------------------------------------------

File: TIDorb/core/comm/ServerListener.h

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
//
------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_SSLServerListener_H_
#define _TIDORB_core_comm_SSLServerListener_H_ 1


#include "TIDSocket.h"


namespace TIDorb {
namespace core {
namespace comm {

class SSLServerListener : public TIDThr::Thread
  // TODO: study if could be derived from ServerListener class
{

private:


  // TODO: SSLContext initialized

  TIDSocket::SSLServerSocket* server_socket;
  ConnectionManager_ref manager;
  
  bool do_shutdown;
  
  /**
   * Listen point where the SSLServerSocket will be listening. This point(host, port)
   * will be used to create de local IOR's.
   */
  TIDorb::core::comm::iiop::ListenPoint listen_point;
  
  TIDorb::core::TIDORB_ref _orb;

  const TIDorb::core::ConfORB& conf;
  
public:
  
  SSLServerListener(ConnectionManager* the_manager, 
                    TIDThr::ThreadGroup* group)
    throw (TIDThr::SystemException, CORBA::SystemException);

  ~SSLServerListener() 
  throw (TIDThr::SystemException);
  
  void run();
  
  void shutdown();
  
  const TIDorb::core::comm::iiop::ListenPoint& get_listen_point();
  
  TIDorb::core::iop::IOR* createIOR(const char* id, 
                                    TIDorb::core::poa::POAKey* key);

  
protected:
  
  void reset_server_socket();
};
  
}
}
}

#endif


