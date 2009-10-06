/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPLocateRequestMessage.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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

