//////////////////////////////////////////////////////////////////////////////////
//
// File:        HeartbeatThread.C
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

#include "TIDorb/core/comm/HeartbeatThread.h"



 
TIDorb::core::comm::HeartbeatThread::HeartbeatThread
  (ConnectionManager* mngr,
   TIDThr::ThreadGroup* group,
   ::TimeBase::TimeT heartbeat_interval,
   ::TimeBase::TimeT heartbeat_timeout)
  throw (TIDThr::SystemException)
  : TIDThr::Thread(group, NULL, "")
{
  conn_mngr = mngr;
  interval_millis = heartbeat_interval / 10000L;
  timeout_millis  = heartbeat_timeout  / 10000L;
}




void TIDorb::core::comm::HeartbeatThread::run() 
{
  TIDThr::Synchronized synchro(*this);
 
  while (true)
  {
    wait(interval_millis);

    ConnectionManager::ConnectionMapIteratorT it;
    ConnectionManager::ConnectionMapIteratorT end = conn_mngr->client_connections.end();

    for (it = conn_mngr->client_connections.begin(); it != end; it++) {
      if ((*it).first._heartbeat_enabled) {
        Connection_ref* conn = (Connection_ref*) (*it).second;
        (*conn)->send_heartbeat();
      }
    }
    
    // Wait for response
    wait(timeout_millis);

    // Close invalid connections
    for (it = conn_mngr->client_connections.begin(); it != end; it++) {
      if ((*it).first._heartbeat_enabled) {
        Connection_ref* conn = (Connection_ref*) (*it).second;
        if (((*conn)->heartbeat_time) > timeout_millis) {
          conn_mngr->close_connection((Connection*) (*conn));
        }
      }
    }


  }
}
