/*/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectDelegate.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/portable.h"

#ifndef _TIDORB_PORTABLE_OBJECT_DELEGATE_H_
#define _TIDORB_PORTABLE_OBJECT_DELEGATE_H_

namespace TIDorb {
namespace portable {

class ObjectDelegate : public RefCounter {
  public:
    virtual ~ObjectDelegate() {}
       
    // ORB
    virtual TIDorb::portable::ORB* orb() = 0;
    
    // The reference has been created by the orb()
    
    virtual ::CORBA::Boolean is_local() = 0; 
    
     // Object existence test
    
    virtual ::CORBA::Boolean non_existent() = 0;     

  protected:
    ObjectDelegate(){};
};
}
}

#endif
