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

// MIOP extensions
#include "TIDorb/core/iop/TAG_UIPMC.h"
#include "TIDorb/core/iop/TAG_GROUP.h"
#include "TIDorb/core/iop/TAG_GROUP_IIOP.h"
// end MIOP extensions

// FT extensions
#include "TIDorb/core/iop/TAG_ALTERNATE_IIOP_ADDRESS.h"
#include "TIDorb/core/iop/TAG_FT_HEARTBEAT_ENABLED.h"
// end FT extensions

#include "TIDorb/core/iop/TAG_SSL_SEC_TRANS.h"
#include "TIDorb/core/iop/TAG_CSI_SEC_MECH_LIST.h"

#include "TIDorb/core/iop/TaggedComponentReader.h"
#include "TIDorb/core/iop/ORBComponent.h"
#include "TIDorb/core/iop/ProfileMultipleComponents.h"
#include "TIDorb/core/iop/TaggedProfileReader.h"
#include "TIDorb/core/iop/IOR.h"


#endif
