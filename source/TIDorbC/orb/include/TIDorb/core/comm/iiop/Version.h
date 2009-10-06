/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/Version.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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
