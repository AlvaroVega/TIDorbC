//////////////////////////////////////////////////////////////////////////////////
//
// File:        OpeningLock.C
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

#include "TIDorb/core.h"
#include "TIDorb/core/comm.h"


TIDorb::core::comm::OpeningLock::OpeningLock() throw(TIDThr::SystemException)
: status(ST_OPENING), opened_connection(0){}

TIDorb::core::comm::Connection* 
  TIDorb::core::comm::OpeningLock::wait_opening(CORBA::ULongLong time) throw(CORBA::SystemException)
{
  TIDThr::Synchronized synchro(*this);
  
  if(status == ST_OPENING) {
    try {
        wait(time);
    } catch (const TIDThr::InterruptedException& ioe){}
  }

  if(status == ST_OPENED)
    return opened_connection;

  else if(status == ST_ERROR)
    throw error;
  else
    throw CORBA::COMM_FAILURE("Connection timout");
}

void TIDorb::core::comm::OpeningLock::set_opened(TIDorb::core::comm::Connection* conn) throw()
{
  TIDThr::Synchronized synchro(*this);  
  
  status = ST_OPENED;
  opened_connection = conn;
  notifyAll();
}

void TIDorb::core::comm::OpeningLock::set_error(const CORBA::COMM_FAILURE& failure) throw()
{
  TIDThr::Synchronized synchro(*this);    
  status = ST_ERROR;
  error = failure;
  notifyAll();
}

