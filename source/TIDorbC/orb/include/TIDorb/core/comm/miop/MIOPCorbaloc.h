///////////////////////////////////////////////////////////////////////////
//
// File:        MIOPCorbaloc.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
  static TIDorb::core::iop::IOR* get_IOR(const char* corbaloc_URL)
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
