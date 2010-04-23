/////////////////////////////////////////////////////////////////////////
//
// File:        SecurityChecker.h
// Description:
//              
// Rel:         01.00
// Created:     September, 2009
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security.h"

#ifndef _TIDORB_CORE_SECURITY_SECURITY_CHECKER_H_
#define _TIDORB_CORE_SECURITY_SECURITY_CHECKER_H_

namespace TIDorb {
namespace core {
namespace security {


/*
 * SecurityChecker at CORBASEC level 2
 */

class SecurityChecker {

public:

  static int get_verify_mode(const TIDorb::core::PolicyContext& context);

  static void checkQOP(TIDorb::core::TIDORB* orb, 
                       const TIDorb::core::PolicyContext& context, 
                       Security::AssociationOptions sec_options);

  static void checkEstablishTrust(TIDorb::core::TIDORB* orb, 
                                  const TIDorb::core::PolicyContext& context, 
                                  CSIIOP::CompoundSecMechList* mechs);

};

}// security
}// core
}// TIDorb
#endif
