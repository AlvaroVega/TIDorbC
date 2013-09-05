/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        Corbaname.h
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

#include "TIDorb/core/util.h"

#ifndef __TIDorb_core_util_Corbaname_H_
#define __TIDorb_core_util_Corbaname_H_

#include "CORBA.h"
#include "CosNaming.h"

namespace TIDorb {
namespace core {
namespace util {


class Corbaname {

  public:
  
    static CORBA::Object_ptr to_object(TIDorb::core::TIDORB* tidorb, 
                                       const char* corbaname_URL) 
      throw (CosNaming::NamingContext::InvalidName,
		         CosNaming::NamingContext::NotFound,
		         CosNaming::NamingContext::CannotProceed,
		         CORBA::ORB::InvalidName);
		         
    static CosNaming::Name* to_name(const char* sn) 
		  throw (CosNaming::NamingContext::InvalidName);
		
		static char* to_string(const CosNaming::Name* n);
	
	private:
	
		static CosNaming::NamingContext_ptr parse_protocol(TIDorb::core::TIDORB* tidorb, 
		                                                   const char* protocol)
		  throw (CosNaming::NamingContext::InvalidName,
		         CORBA::ORB::InvalidName);
  
};

}
}
}

#endif

