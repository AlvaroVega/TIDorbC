/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPLocateRequestMessage.h

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

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_GIOPLOCATEREQUESTMESSAGE_H_
#define _TIDORB_CORE_COMM_IIOP_GIOPLOCATEREQUESTMESSAGE_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class GIOPLocateRequestMessage : public GIOPFragmentedMessage
{
  public:

        GIOPLocateRequestMessage(const GIOPHeader& header);

        GIOPLocateRequestMessage(const Version& ver, RequestId id);

        TargetAddress* extract_address();

        void insert_address(TIDorb::core::TIDORB* orb,
                            const ObjectKey& key);

        void insert_address(TIDorb::core::TIDORB* orb,
                            const TIDorb::core::iop::IOR& ior,
                            AddressingDisposition disposition);

        ~GIOPLocateRequestMessage() {};

	void* _impl();
	const char* _typeid();

};
} //iiop
} //comm
} //core
} //TIDorb
#endif

