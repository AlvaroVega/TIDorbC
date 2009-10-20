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
