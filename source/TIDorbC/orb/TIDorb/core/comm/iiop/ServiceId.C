//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServiceId.C
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

#include "TIDorb/core/comm/iiop/ServiceId.h"

const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::TransactionService = 0;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::CodeSets = 1;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::ChainBypassCheck = 2;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::ChainBypassInfo = 3;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::LogicalThreadId = 4;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::BI_DIR_IIOP = 5;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::SendingContextRunTime = 6;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::INVOCATION_POLICIES = 7;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::FORWARDED_ENTITY = 8;
const CORBA::ULong TIDorb::core::comm::iiop::ServiceId::UnknownExceptionInfo = 9;
