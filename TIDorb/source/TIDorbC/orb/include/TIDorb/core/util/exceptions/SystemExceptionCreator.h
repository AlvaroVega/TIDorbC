/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        
// Description: 
//
// Rel:         01.00
// Created:     
// Author:      
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_CREATOR_H_

#include "CORBA.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
   using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
  using std::string;
#endif

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {

class SystemExceptionCreator {

  public:

  virtual ~SystemExceptionCreator(){}

  virtual CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus completion) = 0;
  virtual CORBA::SystemException* copy(const CORBA::SystemException& se) = 0;
  protected:
   SystemExceptionCreator(){}
 
};

typedef map<string, SystemExceptionCreator*> SystemExceptionCreatorTable;


} // exceptions
} // util
} // core
} // TIDorb

#endif 
