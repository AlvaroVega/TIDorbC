/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/Version.h

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

#include "TIDorb/core/comm/iiop_fw.h"

#ifndef _TIDORB_CORE_COMM_IIOP_VERSION_H_
#define _TIDORB_CORE_COMM_IIOP_VERSION_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

/*
 * IIOPVersion: representa las distintas versiones IIOP.
 * Las versiones se utilizan de forma canónica, es decir, 
 * existe sólo una instancia por version, y no se deben destruir. 
 */

class Version {
public:

  Version();
  Version(CORBA::ULong maj, CORBA::ULong min);
  Version(const Version& v);
  
public:

  CORBA::ULong _major;
  CORBA::ULong _minor;
  
  CORBA::ULong getMajor() const;
  
  CORBA::ULong getMinor() const;
  
  Version& operator= (const TIDorb::core::comm::iiop::Version& ver);
  
  bool operator== (const TIDorb::core::comm::iiop::Version& ver) const;
  bool operator<= (const TIDorb::core::comm::iiop::Version& ver) const;
  bool operator< (const TIDorb::core::comm::iiop::Version& ver) const;
  bool operator>= (const TIDorb::core::comm::iiop::Version& ver) const;
  bool operator> (const TIDorb::core::comm::iiop::Version& ver) const;
  bool operator!= (const TIDorb::core::comm::iiop::Version& ver) const;
  
  
  void read(TIDorb::portable::InputStream& input);
  
  void write(TIDorb::portable::OutputStream& output) const;
  
  const char* toString() const;
  
  static const TIDorb::core::comm::iiop::Version VERSION_1_0;
  static const char* version_1_0_name;
  
  static const TIDorb::core::comm::iiop::Version VERSION_1_1;
  static const char* version_1_1_name;
  
  static const TIDorb::core::comm::iiop::Version VERSION_1_2;
  static const char* version_1_2_name;
  
  ~Version () {}
  
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
