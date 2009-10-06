/////////////////////////////////////////////////////////////////////////
//
// File:        ConnectionThread.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/ConnectionThread.h"

TIDorb::core::comm::ConnectionThread::ConnectionThread (Connection* conn, 
                                                        TIDThr::ThreadGroup* group)
  throw (TIDThr::SystemException):
  TIDThr::Thread(group, NULL, "")
{
  connection = conn;
}

void TIDorb::core::comm::ConnectionThread::run() 
{
 	connection->receive_message();
}
