/////////////////////////////////////////////////////////////////////////
//
// File:        URL.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/util/URL.h"


const char* TIDorb::core::util::URL::CORBANAME_PROTOCOL = "corbaname:";
const char* TIDorb::core::util::URL::FILE_PROTOCOL      = "file://";
const char* TIDorb::core::util::URL::RIR_PROTOCOL       = "corbaloc:rir:";
const char* TIDorb::core::util::URL::TIDORB_PROTOCOL    = "corbaloc:tidorb:";
const char* TIDorb::core::util::URL::IIOP_PROTOCOL_1    = "corbaloc::";
const char* TIDorb::core::util::URL::IIOP_PROTOCOL_2    = "corbaloc:iiop:";

// pra@tid.es - MIOP extensions
const char* TIDorb::core::util::URL::MIOP_PROTOCOL      = "corbaloc:miop:";
// end MIOP extensions
