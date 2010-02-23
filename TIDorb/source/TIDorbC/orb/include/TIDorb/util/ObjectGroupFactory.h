/*/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectGroupFactory.h
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
/////////////////////////////////////////////////////////////////////////*/

#ifndef __TIDORB_UTIL_OBJECTGROUPFACTORY_H_
#define __TIDORB_UTIL_OBJECTGROUPFACTORY_H_

#include "CORBA.h"

namespace TIDorb {
namespace util {


class ObjectGroupFactory {

  public:
    static char* get_object_group_url
        (CORBA::ORB_ptr   orb,
         const char*      mcast_address,
         unsigned short   mcast_port,
         const char*      group_iiop_url,	// NULL: no Group IIOP profile
         const char*      group_gateway_url,	// NULL: no Group Gateway
         CORBA::ULongLong group_id,
         const char*      group_domain,
         const char*      group_component_version = "1.0",
         CORBA::ULong     group_ref_version = 0);
	
};
}
}

#endif
