/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop_fw.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm_fw.h"

#ifndef _TIDORB_CORE_COMM_IIOP_FW_H_
#define _TIDORB_CORE_COMM_IIOP_FW_H_


#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif


// Forward Declarations

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {
  
  class ListenPoint;
  typedef vector<ListenPoint> VectorListenPoint;
 
  class ObjectKey;
  typedef TIDThr::HandleT<ObjectKey> ObjectKey_ref;
  
  class ServiceId;
  
  class ServiceContext;
  typedef TIDThr::HandleT<ServiceContext> ServiceContext_ref;
  typedef vector<TIDorb::core::comm::iiop::ServiceContext_ref> VectorServiceContext;
   
  class Version;
  class UserSystemExceptionEncode;
  
  class ProfileIIOP;
  typedef TIDThr::HandleT<ProfileIIOP> ProfileIIOP_ref;
  
  class IORAddressingInfo;
    
  class TargetAddress;
    
  class BiDirServiceContext;
  class InvocationPoliciesContext;
  class ServiceContextReader;
  class ServiceContextList;
  class GIOPHeader;
  
  class GIOPMessage;  
  class GIOPCancelRequestMessage;  
  class GIOPFragmentMessage;
  class GIOPFragmentedMessage;
  class GIOPLocateReplyMessage;
  class GIOPLocateRequestMessage;
  class GIOPReplyMessage;   
  class GIOPRequestMessage;
   
  class MessageFactory;
}
}
}
}

#include "TIDorb/core/comm/iiop/Version.h"

#endif

