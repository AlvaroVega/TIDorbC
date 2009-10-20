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

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_BAD_TYPECODE_CREATOR_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_BAD_TYPECODE_CREATOR_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {



class BAD_TYPECODE_Creator : public virtual SystemExceptionCreator {
    
  public:

   BAD_TYPECODE_Creator(){}
   virtual ~BAD_TYPECODE_Creator(){}

  CORBA::SystemException* create(CORBA::ULong minor, CORBA::CompletionStatus  completion)
  {
    return new CORBA::BAD_TYPECODE(minor,completion);
  }
  CORBA::SystemException* copy(const CORBA::SystemException& se)
  {
    try {
      return new CORBA::BAD_TYPECODE(dynamic_cast<const CORBA::BAD_TYPECODE&> (se));
    } catch (const bad_cast& bd) {return NULL;}
  }

};

} // exceptions
} // util
} // core
} // TIDorb

#endif 
