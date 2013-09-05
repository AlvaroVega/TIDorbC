//////////////////////////////////////////////////////////////////////////////////
//
// File:        Corbaloc.C
// Description: 
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

#include "TIDorb/core/comm/iiop.h"

#include <string>
#include <list>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
using std::list;
#endif




TIDorb::core::iop::IOR* TIDorb::core::comm::iiop::Corbaloc::get_IOR(const char* corbaloc_URL,const char* iface)
  throw (CORBA::ORB::InvalidName)
{
  string url = corbaloc_URL;

  if (url.find(TIDorb::core::util::URL::TIDORB_PROTOCOL) == string::npos) {
    throw CORBA::ORB::InvalidName(); //"Invalid TIDorb corbaloc"
  }

  string corbaloc_str = url.substr(strlen(TIDorb::core::util::URL::TIDORB_PROTOCOL));

  TIDorb::core::comm::iiop::ObjectKey*    objectkey = NULL;
  TIDorb::core::comm::iiop::ListenPoint*  listen_point = NULL;
  TIDorb::core::comm::iiop::Version       version;
  TIDorb::core::iop::VectorTaggedProfile  profiles;

  // Look for slash
  string::size_type slash_position = corbaloc_str.find('/');
  string addr_str = corbaloc_str.substr(0, slash_position);


  // Look for path
  string path_str = corbaloc_str.substr(slash_position + 1);
  objectkey = parse_poa_path(path_str);

  TIDorb::core::iop::VectorTaggedComponent components;
  components.push_back(new TIDorb::core::iop::ORBComponent(*(TIDorb::core::ConfORB::ORB_TYPE)));
  bool other_addr = false;
  string next_str;

  do {

    // Look for a corbaloc composed by several listen points
    string::size_type next_addr_position = addr_str.find(",iiop");
    other_addr = (next_addr_position != string::npos);   
    
    if (other_addr) {
      next_str = addr_str.substr(next_addr_position+6);
      addr_str = addr_str.substr(0, next_addr_position);
    }

    // Look for version token
    string version_str, listenpoint_str;
    
    string::size_type at_position = addr_str.find('@');
    if (at_position == string::npos) {
      version_str = "1.0";
      listenpoint_str = addr_str;
    } else {
      version_str = addr_str.substr(0, at_position);
      listenpoint_str = addr_str.substr(at_position + 1);
    }

    // Parse version and listen_point
    version = Corbaloc::parse_version(version_str);
    listen_point = Corbaloc::parse_listenpoint(listenpoint_str,iface);

    // Create Profiles
    profiles.push_back(new TIDorb::core::comm::iiop::ProfileIIOP(version, *listen_point,
                                                                 objectkey, components));
    delete listen_point;

    if (other_addr)
      addr_str = next_str;
  
  } while (other_addr);

  return new TIDorb::core::iop::IOR("IDL:omg.org/CORBA/Object:1.0", &profiles);
}





TIDorb::core::comm::iiop::ObjectKey* 
TIDorb::core::comm::iiop::Corbaloc::parse_poa_path(const string& path)
  throw (CORBA::ORB::InvalidName)
{
  CORBA::String_var path_esc = TIDorb::util::TranslateURLEscapes::put_escapes(path.c_str());

  string path_str = (const char*) path_esc;

  if (path_str.find("//") !=  string::npos)
    throw CORBA::ORB::InvalidName(); //"Invalid POA Name"

  size_t tokens = 1;

  //string::size_type last = path_str.rfind('/');
  string::size_type curr = path_str.find('/');

  while (curr != string::npos) {
    tokens++;
    curr = path_str.find('/', curr);
  }

  TIDorb::core::poa::POAPath poa_path;
  poa_path.length(tokens - 1);

  string::size_type tok_start = 0;
  string::size_type tok_end   = path_str.find('/');

  CORBA::String_var aux = ""; //0;

  for(size_t i = 0; i < tokens - 1; i++) {
    aux = TIDorb::util::TranslateURLEscapes::resolve_escapes(
            path_str.substr(tok_start, tok_end - tok_start).c_str());

    poa_path[i] = TIDorb::util::TranslateURLEscapes::translate_unicode(aux);

    tok_start = tok_end + 1;
    tok_end = path_str.find('/', tok_start);
  }

  CORBA::String_var oid_str =
    TIDorb::util::TranslateURLEscapes::resolve_escapes(path_str.substr(tok_start).c_str());

  TIDorb::core::poa::OID* oid = new TIDorb::core::poa::OID();

  try {
    oid->fromString(oid_str);
  } catch(const CORBA::Exception& e) {
    throw CORBA::ORB::InvalidName();//"Invalid OID");
  }

  TIDorb::core::poa::POAKey* poakey =
    new TIDorb::core::poa::POAKey(poa_path, 0, oid);

  return new TIDorb::core::comm::iiop::ObjectKey(poakey);
}




const TIDorb::core::comm::iiop::Version& 
TIDorb::core::comm::iiop::Corbaloc::parse_version(const string& v)
  throw (CORBA::ORB::InvalidName)
{
  unsigned int major, minor;

  string::size_type dot_position = v.find('.');

  if((dot_position == string::npos) || (dot_position != v.rfind('.')))
    throw CORBA::ORB::InvalidName();

  string major_str, minor_str;

  major_str = v.substr(0, dot_position);
  minor_str = v.substr(dot_position + 1);

  major = atoi(major_str.c_str());
  minor = atoi(minor_str.c_str());

  if (major==1) {
    if (minor==0) return TIDorb::core::comm::iiop::Version::VERSION_1_0;
    else if (minor==1) return TIDorb::core::comm::iiop::Version::VERSION_1_1;
    else if (minor==2) return TIDorb::core::comm::iiop::Version::VERSION_1_2;
  }

  throw CORBA::ORB::InvalidName();
}


// //
// // Original
// //
// TIDorb::core::comm::iiop::ListenPoint* 
// TIDorb::core::comm::iiop::Corbaloc::parse_listenpoint(const string& listenp)
//   throw (CORBA::ORB::InvalidName)
// {
//   string host, port_str;
//   int port;

//   string::size_type colon_position = listenp.find(':');
//   if (colon_position == string::npos) {
//     host = listenp;
//     port = 2809;
//   }

//   if (colon_position != listenp.rfind(':'))
//     throw CORBA::ORB::InvalidName();

//   host = listenp.substr(0, colon_position);
//   port = atoi(listenp.substr(colon_position + 1).c_str());

//   return new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port);
// }


//
// MIOP url incoming/outgoing network interfaces extension for INDRA-SACTA
//
TIDorb::core::comm::iiop::ListenPoint* 
TIDorb::core::comm::iiop::Corbaloc::parse_listenpoint(const string& listenp, const string& iface)
  throw (CORBA::ORB::InvalidName)
{
  string host, port_str;
  string incoming_interface, outgoing_interface;
  int port = 2809;


  string::size_type colon_position = listenp.find(':');
  //cerr << "colon_position " << colon_position << endl;
  if (colon_position == string::npos) {
    host = listenp;
    port = 2809;
    //cerr << "case 1: host " << host << endl;
    //cerr << "case 1: port " << port << endl;
    return  new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port);
  } else {
    //host = listenp.substr(0, colon_position);
    //mcpg - begin
    string::size_type ipv6_separator_begin = listenp.find('[');
    string::size_type ipv6_separator_end;
  
    if (ipv6_separator_begin != string::npos)
    {
      ipv6_separator_end = listenp.find(']');
      if (ipv6_separator_end == string::npos)
        throw CORBA::ORB::InvalidName();
    }
     
    //string::size_type colon_position2 = listenp.find(':', colon_position + 1); 
    string::size_type colon_position2;
    if (ipv6_separator_begin != string::npos) //IPv6
    {
      host=listenp.substr(ipv6_separator_begin + 1, ipv6_separator_end-1);
      if (strcmp(host.c_str(),"::1") != 0)
      {
        host += "%";
        host += iface;
      }
      colon_position2 = listenp.find(':', ipv6_separator_end+2);
    }
    else
    {
      host = listenp.substr(0, colon_position);
      colon_position2 = listenp.find(':', colon_position + 1); 
    }
    //mcpg - end
    //cerr << "colon_position2 " << colon_position2 << endl;
    if (colon_position2 == string::npos) {
    	if (ipv6_separator_begin != string::npos) //IPv6
    	  port = atoi(listenp.substr(ipv6_separator_end + 2).c_str());
    	else
        port = atoi(listenp.substr(colon_position + 1).c_str());
      //cerr << "case 2: host " << host << endl;
      //cerr << "case 2: port " << port << endl;
      return  new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port);
    } else {
      port = atoi(listenp.substr(colon_position+1, colon_position2).c_str());

      string::size_type colon_position3 = listenp.find(':', colon_position2 + 1);
      //cerr << "colon_position3 " << colon_position3 << endl;
      if (colon_position3 == string::npos) {
        incoming_interface = listenp.substr(colon_position2 + 1);
        //cerr << "case 3: host " << host << endl;
        //cerr << "case 3: port " << port << endl;
        //cerr << "case 3: incoming_interface " << incoming_interface << endl;
        return  new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port, 
                                                          incoming_interface.c_str(), NULL);
      } else {
        incoming_interface = listenp.substr(colon_position2 + 1);

        string::size_type colon_position4 = incoming_interface.find(':', 0);
        string incoming_interface2 = incoming_interface.substr(0, colon_position4);
        //cerr << "colon_position4 " << colon_position4 << endl;

        outgoing_interface = listenp.substr(colon_position3 + 1);
        //cerr << "case 4: host " << host << endl;
        //cerr << "case 4: port " << port << endl;
        //cerr << "case 4: incoming_interface " << incoming_interface2 << endl;
        //cerr << "case 4: outgoing_interface " << outgoing_interface << endl;
        return new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port, 
                                                           incoming_interface2.c_str(),
                                                           outgoing_interface.c_str());
      }
      
    }

  }
  

// //   if (colon_position != listenp.rfind(':'))
// //     throw CORBA::ORB::InvalidName();

//   port = atoi(listenp.substr(colon_position + 1).c_str());

//   string::size_type colon_position2 = listenp.find(':', colon_position + 1);

//   if (colon_position == string::npos) {
//     return new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port);
//   } else {
//     // Read incoming and outgoing interfaces: miop corbaloc propietary extension
//     string::size_type colon_position_end = listenp.find(':', colon_position2 + 1);
//     incoming_interface = listenp.substr(colon_position2+1, colon_position_end + 1);
//     cerr << "incoming_interface = " << incoming_interface << endl; 
//     cerr << "colon_position_end " << colon_position_end << endl;

//     outgoing_interface = listenp.substr(colon_position_end + 1);
//     cerr << "outgoing_interface = " << outgoing_interface << endl; 
//     return new TIDorb::core::comm::iiop::ListenPoint(host.c_str(), port, 
//                                                      incoming_interface.c_str(),
//                                                      outgoing_interface.c_str());
//   }
//   // 
}




char* TIDorb::core::comm::iiop::Corbaloc::to_URL(const TIDorb::core::iop::IOR* ior)
{
  TIDorb::util::StringBuffer corbaloc;

  corbaloc << TIDorb::core::util::URL::TIDORB_PROTOCOL;

  TIDorb::core::comm::iiop::ProfileIIOP* p =
    dynamic_cast <TIDorb::core::comm::iiop::ProfileIIOP*> (ior->get_profile(0));

  const TIDorb::core::comm::iiop::Version&     v = p->getVersion();
  const TIDorb::core::comm::iiop::ListenPoint& l = p->getListenPoint();
  const TIDorb::core::comm::iiop::ObjectKey*   o = p->getObjectKey();
  const TIDorb::core::poa::POAKey*             k = o->get_key();

  if (k->getPOAId() != 0L)
    throw CORBA::BAD_PARAM("Non persistent reference", 0, CORBA::COMPLETED_NO);

  const TIDorb::core::poa::OID& oid = k->getOID();

  corbaloc << v.getMajor() << "." << v.getMinor() << "@";
  corbaloc << l._host << ":"  << l._port << "/";

  CORBA::String_var poa_name;

  for (CORBA::ULong i=0; i< k->numberOfPOAs();i++) {
    poa_name = TIDorb::util::TranslateURLEscapes::stringfy(k->getPOA(i));
    corbaloc << poa_name << "/";
  }

  CORBA::String_var oid_str = oid.toString();
  poa_name = TIDorb::util::TranslateURLEscapes::stringfy(oid_str);

  corbaloc << poa_name;

  return CORBA::string_dup(corbaloc.str().data());
}
