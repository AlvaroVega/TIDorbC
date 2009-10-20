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

