/////////////////////////////////////////////////////////////////////////
//
// File:        ConnectionState.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"


TIDorb::core::comm::ConnectionState::ConnectionState()
  throw (TIDThr::SystemException)
{
  state = OPEN_STATE;
  connection_error = NULL;
} 

TIDorb::core::comm::ConnectionState::~ConnectionState()
  throw (TIDThr::SystemException)
{
  if (connection_error){
      delete connection_error;
      connection_error = NULL;
  }
}

bool TIDorb::core::comm::ConnectionState::set_closing()
{
  TIDThr::Synchronized synchro(*this);
  if(state != OPEN_STATE)
    return false;
    
  state = CLOSING_STATE;
  return true;
}

bool TIDorb::core::comm::ConnectionState::is_open()
{
  return state == OPEN_STATE;
}
  

bool TIDorb::core::comm::ConnectionState::set_error(const CORBA::COMM_FAILURE& the_error)
{
  TIDThr::Synchronized synchro(*this);
  
  if(state == ERROR_STATE) {
    return false;
  } else if(state == CLOSING_STATE) {
    state = ERROR_STATE;
    if (connection_error){
       delete connection_error;
    }
    connection_error = new CORBA::COMM_FAILURE(the_error);
    return false;
  } else { // OPEN_STATE
    state = ERROR_STATE;
    if (connection_error){
       delete connection_error;
    }
    connection_error = new CORBA::COMM_FAILURE(the_error);
    return true;
  }
}

short TIDorb::core::comm::ConnectionState::get_value()
{
  return state;
}

const CORBA::COMM_FAILURE* TIDorb::core::comm::ConnectionState::get_error()
{
  return connection_error;
}

/**
 *  Verify if the reply can be sent.
 */
void TIDorb::core::comm::ConnectionState::verify_reply()
{
  switch (state){
    case OPEN_STATE:
      return;
    case CLOSING_STATE:
      throw CORBA::COMM_FAILURE("Connection is closed.");
    case ERROR_STATE:
      connection_error->_raise();
  }
}

/**
 *  Verify if the request can be sent.
 */
void TIDorb::core::comm::ConnectionState::verify_request() throw(TIDorb::core::comm::RECOVERABLE_COMM_FAILURE)
{
  switch (state){
    case OPEN_STATE:
      return;
    case CLOSING_STATE:
      throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(CORBA::COMM_FAILURE("Connection is closed."));
    case ERROR_STATE:
     throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(*connection_error);
  }
}
