/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop.h

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

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_CORE_COMM_IIOP_H_
#define _TIDORB_CORE_COMM_IIOP_H_


#include "TIDorb/core/comm/iiop/RequestId.h"
#include "TIDorb/core/comm/iiop/LocateReplyStatusType.h"
#include "TIDorb/core/comm/iiop/ReplyStatusType.h"
#include "TIDorb/core/comm/iiop/MsgType.h"
#include "TIDorb/core/comm/iiop/AddressingDisposition.h"
#include "TIDorb/core/comm/iiop/ListenPoint.h"

#include "TIDorb/core/comm/iiop/ObjectKey.h"

#include "TIDorb/core/comm/iiop/ServiceId.h"
#include "TIDorb/core/comm/iiop/ServiceContext.h"
#include "TIDorb/core/comm/iiop/UserExceptionEncoder.h"

#include "TIDorb/core/comm/iiop/BI_DIR_IIOP.h"

#include "TIDorb/core/comm/iiop/GIOPHeader.h"
#include "TIDorb/core/comm/iiop/GIOPMessage.h"

#include "TIDorb/core/comm/iiop/ProfileIIOP.h"
#include "TIDorb/core/comm/iiop/IORAddressingInfo.h"
#include "TIDorb/core/comm/iiop/TargetAddress.h"
#include "TIDorb/core/comm/iiop/BiDirServiceContext.h"
#include "TIDorb/core/comm/iiop/InvocationPoliciesContext.h"
#include "TIDorb/core/comm/iiop/ServiceContextReader.h"
#include "TIDorb/core/comm/iiop/ServiceContextList.h"
#include "TIDorb/core/comm/iiop/GIOPCancelRequestMessage.h"
#include "TIDorb/core/comm/iiop/GIOPFragmentMessage.h"
#include "TIDorb/core/comm/iiop/GIOPFragmentedMessage.h"
#include "TIDorb/core/comm/iiop/GIOPLocateReplyMessage.h"
#include "TIDorb/core/comm/iiop/GIOPLocateRequestMessage.h"
#include "TIDorb/core/comm/iiop/GIOPReplyMessage.h"
#include "TIDorb/core/comm/iiop/GIOPRequestMessage.h"

#include "TIDorb/core/comm/iiop/Corbaloc.h"
#include "TIDorb/core/comm/iiop/IIOPCorbaloc.h"



#endif
