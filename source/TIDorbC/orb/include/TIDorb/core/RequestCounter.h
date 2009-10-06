//////////////////////////////////////////////////////////////////////////////////
//
// File:        RequestCounter.h
// Description: RequestCounter implementation.
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

#ifndef _TIDORB_CORE_REQUEST_COUNTER_H
#define _TIDORB_CORE_REQUEST_COUNTER_H

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class RequestCounter: public virtual TIDThr::RecursiveMutex {

private:
  CORBA::Long m_serial;

public:
  
  RequestCounter();
  ~RequestCounter() throw (TIDThr::SystemException) {}
  
  CORBA::Long next();

};


} //core
} //TIDorb

#endif
