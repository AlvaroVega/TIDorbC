/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        InitialReference.h
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

#ifndef __TIDorb_core_util_InitialReference_H_
#define __TIDorb_core_util_InitialReference_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {


class InitialReference {

  public:
    
    InitialReference();
    InitialReference(const char* name, const char* url);
    InitialReference(const InitialReference& v);
    
    const char* get_name() const
    {
      return object_name;
    }
    
    const char* get_url() const
    {
	    return object_url;  
    }
    
    void parse(const char* value) throw(CORBA::SystemException);
    

  protected:
  
    CORBA::String_var object_name;
    CORBA::String_var object_url;       
};

}
}
}

#endif
