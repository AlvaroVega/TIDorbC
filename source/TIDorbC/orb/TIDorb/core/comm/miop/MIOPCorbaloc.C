//////////////////////////////////////////////////////////////////////////////////
//
// File:        MIOPCorbaloc.C
// Description: 
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

#include "TIDorb/core/comm/miop.h"




void TIDorb::core::comm::miop::MIOPCorbaloc::tokenize(const string& str, vector<string>& tokens, const string& delim)
{
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delim, 0);

  // Find first "non-delimiter".
  string::size_type pos = str.find_first_of(delim, lastPos);
  while (string::npos != pos || string::npos != lastPos) {
     // Found a token, add it to the vector.
     tokens.push_back(str.substr(lastPos, pos - lastPos));

     // Skip delimiters.  Note the "not_of"
     lastPos = str.find_first_not_of(delim, pos);

     // Find next "non-delimiter"
     pos = str.find_first_of(delim, lastPos);
  }
}




TIDorb::core::iop::IOR*
TIDorb::core::comm::miop::MIOPCorbaloc::get_IOR(const char* corbaloc_URL)
  throw(CORBA::ORB::InvalidName)
{
  string url = corbaloc_URL;
  string corbaloc_str = url.substr(9); // starts after "corbaloc:"

  TIDorb::core::iop::VectorTaggedProfile profiles;

  // get object address list
  vector<string> obj_address_list;
  tokenize(corbaloc_str, obj_address_list, ",");

  // iterate object address list
  vector<string>::iterator iter;
  for (iter = obj_address_list.begin(); iter != obj_address_list.end(); iter++)
  {
    string obj_address = *iter;

    if (obj_address.compare(0, 4, "miop") == 0) {
      // MIOP
      string::size_type sep, pos = strlen("miop:");

      // Extract version string
      string version_str = "1.0";
      if ((sep = obj_address.find('@', pos)) != string::npos) {
      	version_str = obj_address.substr(pos, sep - pos);
      	pos = sep + 1;
      }

      // Extract group_iiop string
      string group_iiop_str = "";
      if ((sep = obj_address.substr(pos).rfind(';')) != string::npos) {
        group_iiop_str = obj_address.substr(pos + sep + 1);
        obj_address.erase(pos + sep);
      }

      // Extract listen_point string
      string listen_point_str = "";
      if ((sep = obj_address.substr(pos).rfind('/')) != string::npos) {
        listen_point_str = obj_address.substr(pos + sep + 1);
        obj_address.erase(pos + sep);
      }

      // Extract group identifier components
      vector<string> items;
      tokenize(obj_address.substr(pos), items, "-");
      if ((items.size() < 3) || (items.size() > 4)) {
        throw CORBA::ORB::InvalidName();
      } 
      string group_component_version_str = items[0];
      string group_domain_str            = items[1];
      string group_id_str                = items[2];
      string group_ref_version_str       = (items.size() == 4) ? items[3] : "";

      // Get version
      TIDorb::core::comm::miop::Version version =
        TIDorb::core::comm::iiop::Corbaloc::parse_version(version_str);

      // Get listen_point
      TIDorb::core::comm::miop::ListenPoint* listen_point = NULL;
      listen_point = TIDorb::core::comm::iiop::Corbaloc::parse_listenpoint(listen_point_str);

      // Get components
      TIDorb::core::iop::VectorTaggedComponent components;

      TIDorb::core::comm::miop::Version comp_version =
        TIDorb::core::comm::iiop::Corbaloc::parse_version(group_component_version_str);

      CORBA::ULongLong group_id = atol(group_id_str.c_str());
      CORBA::ULong group_ref_version = atol(group_ref_version_str.c_str());
      TIDorb::core::iop::TaggedComponent_ref group_info = 
        new GroupInfo(comp_version, group_domain_str.c_str(), group_id, group_ref_version);
      components.push_back(group_info);
      // components.push_back(new GroupInfo(comp_version, group_domain_str.c_str(), group_id, group_ref_version));

      if (! group_iiop_str.empty()) {
        string iiop_corbaloc = string("corbaloc:") + group_iiop_str;
        TIDorb::core::iop::IOR_ref ior = TIDorb::core::comm::iiop::IIOPCorbaloc::get_IOR(iiop_corbaloc.c_str());
        components.push_back(new GroupIIOPProfile(*ior->profile_IIOP()));        
      }

      // Create profile
      //PRA: TIDorb::core::comm::miop::ProfileMIOP* miop_profile =
      TIDorb::core::iop::TaggedProfile_ref miop_profile =
        new TIDorb::core::comm::miop::ProfileMIOP(version, *listen_point, components);
      //EPRA

      profiles.push_back(miop_profile);
      delete listen_point;

    } else if (obj_address.compare(0, 4, "iiop") == 0) {
      // IIOP
      string iiop_corbaloc = string("corbaloc:") + obj_address;
      TIDorb::core::iop::IOR_ref ior = TIDorb::core::comm::iiop::IIOPCorbaloc::get_IOR(iiop_corbaloc.c_str());
      TIDorb::core::comm::iiop::ProfileIIOP* iiop_profile = new TIDorb::core::comm::iiop::ProfileIIOP(*ior->profile_IIOP());
      profiles.push_back(iiop_profile);

    } else {
      throw CORBA::ORB::InvalidName();
    }
  }

  return new TIDorb::core::iop::IOR("IDL:omg.org/CORBA/Object:1.0", &profiles);
}




const char* TIDorb::core::comm::miop::MIOPCorbaloc::get_group_object_id(const TIDorb::core::comm::miop::ProfileMIOP* profile)
{
  char* group_object_id = NULL;

  try {
    const TIDorb::core::iop::VectorTaggedComponent& components = profile->getTaggedComponents();

    TIDorb::core::comm::miop::GroupInfo* group_info = NULL;

    TIDorb::core::iop::VectorTaggedComponent::const_iterator iter;
    for (iter = components.begin(); iter != components.end(); iter++) {
      TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
      if (comp->_tag == TIDorb::core::iop::TAG_GROUP) {
        group_info = dynamic_cast<TIDorb::core::comm::miop::GroupInfo*>(comp);
      }
    }

    if (group_info)
      group_object_id = (char*) group_info->toString();

  } catch (...) {
  }

  return (const char*) group_object_id;
}
