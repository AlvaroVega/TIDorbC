///////////////////////////////////////////////////////////////////////////
//
// File:        MIOPCorbaloc.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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
///////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/miop.h"


#ifndef _TIDORB_CORE_COMM_MIOP_MIOPCORBALOC_H_
#define _TIDORB_CORE_COMM_MIOP_MIOPCORBALOC_H_


#include <string>
#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::string;
  using std::vector;
#endif


namespace TIDorb {
namespace core {
namespace comm {
namespace miop {

class MIOPCorbaloc
{

public:
  static TIDorb::core::iop::IOR* get_IOR(const char* corbaloc_URL,const char* iface)
    throw(CORBA::ORB::InvalidName);

  static const char* get_group_object_id(const TIDorb::core::comm::miop::ProfileMIOP* profile);

private:
  static void tokenize(const string& str, vector<string>& tokens, const string& delim);

};

} //miop
} //comm
} //core
} //TIDorb

#endif
