/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/Corbaloc.h

 Revisions:

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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef __TIDorb_core_comm_iiop_Corbaloc_H_
#define __TIDorb_core_comm_iiop_Corbaloc_H_


#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::string;
#endif


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {


class Corbaloc {

public:

  static TIDorb::core::iop::IOR* get_IOR(const char* corbaloc_URL,const char* iface) 
    throw (CORBA::ORB::InvalidName);
    
  static char* to_URL(const TIDorb::core::iop::IOR* ior);

  static const TIDorb::core::comm::iiop::Version& parse_version(const string& v) 
    throw (CORBA::ORB::InvalidName);

  static TIDorb::core::comm::iiop::ListenPoint* parse_listenpoint(const string& listenp, const string& iface) 
    throw (CORBA::ORB::InvalidName);

protected:
	
  static TIDorb::core::comm::iiop::ObjectKey* parse_poa_path(const string& poa_path) 
    throw (CORBA::ORB::InvalidName);

};

}
}
}
}

#endif
