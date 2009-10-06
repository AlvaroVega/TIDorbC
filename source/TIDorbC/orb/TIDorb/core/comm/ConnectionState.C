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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"


TIDorb::core::comm::ConnectionState::ConnectionState()
  throw (TIDThr::SystemException)
{
  state = OPEN_STATE;
  connection_error = NULL;
} 

//FRAN
TIDorb::core::comm::ConnectionState::~ConnectionState()
  throw (TIDThr::SystemException)
{
  if (connection_error){
      delete connection_error;
      connection_error = NULL;
  }
}
//EFRAN

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
  //jagd
  //TIDThr::Synchronized synchro(*this);
  
  return state == OPEN_STATE;
}
  

bool TIDorb::core::comm::ConnectionState::set_error(const CORBA::COMM_FAILURE& the_error)
{
  TIDThr::Synchronized synchro(*this);
  
  if(state == ERROR_STATE) {
    return false;
  } else if(state == CLOSING_STATE) {
    state = ERROR_STATE;
//FRAN
    if (connection_error){
       delete connection_error;
    }
//EFRAN
    connection_error = new CORBA::COMM_FAILURE(the_error);
    return false;
  } else { // OPEN_STATE
    state = ERROR_STATE;
//FRAN
    if (connection_error){
       delete connection_error;
    }
//EFRAN
    connection_error = new CORBA::COMM_FAILURE(the_error);
    return true;
  }
}

short TIDorb::core::comm::ConnectionState::get_value()
{
  //jagd lector no necesita mutex
  // TIDThr::Synchronized synchro(*this);
    
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
  //jagd lector no necesita mutex
  //TIDThr::Synchronized synchro(*this);
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
  
  //jagd lector no necesita mutex
  //TIDThr::Synchronized synchro(*this);

  switch (state){
    case OPEN_STATE:
      return;
    case CLOSING_STATE:
      throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(CORBA::COMM_FAILURE("Connection is closed."));
    case ERROR_STATE:
     throw TIDorb::core::comm::RECOVERABLE_COMM_FAILURE(*connection_error);
  }
}
