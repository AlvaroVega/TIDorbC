/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectGroupFactory.C
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/util.h"
#include "TIDorb/core.h"




char* TIDorb::util::ObjectGroupFactory::get_object_group_url
   (CORBA::ORB_ptr   orb,
    const char*      mcast_address,
    unsigned short   mcast_port,
    const char*      group_iiop_url,
    const char*      group_gateway_url,
    CORBA::ULongLong group_id,
    const char*      group_domain,
    const char*      group_component_version,
    CORBA::ULong     group_ref_version)
{
  if (mcast_address == NULL)
    throw CORBA::BAD_PARAM("multicast address must be supplied");
  if (group_domain == NULL)
    throw CORBA::BAD_PARAM("multicast group domain must be supplied");
  if (group_component_version == NULL)
    throw CORBA::BAD_PARAM("multicast group component version must be supplied");

  string group_iiop_str;
  string group_gateway_str;

  if (group_iiop_url) {
    group_iiop_str = group_iiop_url;
    if (group_iiop_str.find("corbaloc:iiop:") != string::npos) {
      group_iiop_str = group_iiop_str.substr(strlen("corbaloc:"));
    } else if (group_iiop_str.find("corbaloc::") != string::npos) {
      group_iiop_str = string("iiop") + group_iiop_str.substr(strlen("corbaloc:"));
    } else {
      throw CORBA::BAD_PARAM("invalid multicast group IIOP URL");
    }
  }

  if (group_gateway_url) {
    group_gateway_str = group_gateway_url;
    if (group_gateway_str.find(TIDorb::core::util::URL::IIOP_PROTOCOL_1) != string::npos) {
      group_gateway_str = group_gateway_str.substr(strlen("corbaloc:"));
    } else {
      throw CORBA::BAD_PARAM("invalid multicast group gateway URL");
    }
  }

  TIDorb::util::StringBuffer buffer;

  buffer << "corbaloc:miop:1.0" << "@"
         << group_component_version << "-"
         << group_domain << "-"
         << group_id << "-"
         << group_ref_version << "/"
         << mcast_address << ":" << mcast_port;

  if (! group_iiop_str.empty()) {
    buffer << ";" << group_iiop_str;
  }

  if (! group_gateway_str.empty()) {
    buffer << "," << group_gateway_str;
  }  	

  return CORBA::string_dup(buffer.str().data());
}
