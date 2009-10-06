//////////////////////////////////////////////////////////////////////////////////
//
// File:        iop.h
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

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_IOP_H_
#define _TIDORB_CORE_IOP_H_

#include "TIDorb/core/iop/TaggedComponent.h"
#include "TIDorb/core/iop/TaggedProfile.h"
#include "TIDorb/core/iop/TAG_INTERNET_IOP.h"
#include "TIDorb/core/iop/TAG_MULTIPLE_COMPONENTS.h"
#include "TIDorb/core/iop/TAG_ORB_TYPE.h"
#include "TIDorb/core/iop/TAG_POLICIES.h"

// pra@tid.es - MIOP extensions
#include "TIDorb/core/iop/TAG_UIPMC.h"
#include "TIDorb/core/iop/TAG_GROUP.h"
#include "TIDorb/core/iop/TAG_GROUP_IIOP.h"
// end MIOP extensions

// pra@tid.es - FT extensions
#include "TIDorb/core/iop/TAG_ALTERNATE_IIOP_ADDRESS.h"
#include "TIDorb/core/iop/TAG_FT_HEARTBEAT_ENABLED.h"
// end FT extensions

#include "TIDorb/core/iop/TaggedComponentReader.h"
#include "TIDorb/core/iop/ORBComponent.h"
#include "TIDorb/core/iop/ProfileMultipleComponents.h"
#include "TIDorb/core/iop/TaggedProfileReader.h"
#include "TIDorb/core/iop/IOR.h"


#endif
