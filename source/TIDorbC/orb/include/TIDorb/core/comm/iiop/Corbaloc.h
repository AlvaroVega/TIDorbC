/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/Corbaloc.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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

  static TIDorb::core::iop::IOR* get_IOR(const char* corbaloc_URL) 
    throw (CORBA::ORB::InvalidName);
    
  static char* to_URL(const TIDorb::core::iop::IOR* ior);

  static const TIDorb::core::comm::iiop::Version& parse_version(const string& v) 
    throw (CORBA::ORB::InvalidName);

  static TIDorb::core::comm::iiop::ListenPoint* parse_listenpoint(const string& listenp) 
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
