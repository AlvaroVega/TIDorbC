//////////////////////////////////////////////////////////////////////////////////
//
// File:        RECOVERABLE_COMM_FAILURE.C
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

#include "TIDorb/core/comm.h"


TIDorb::core::comm::RECOVERABLE_COMM_FAILURE::RECOVERABLE_COMM_FAILURE(const CORBA::COMM_FAILURE& failure) throw()
:  comm_failure(failure)
{ }

const char* TIDorb::core::comm::RECOVERABLE_COMM_FAILURE::what() const throw()
{
  return "RECOVERABLE_COMM_FAILURE";
}
