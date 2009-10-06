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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
