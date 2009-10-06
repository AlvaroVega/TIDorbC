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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
