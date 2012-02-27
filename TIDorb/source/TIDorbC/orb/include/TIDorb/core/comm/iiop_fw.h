/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop_fw.h

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

#include "TIDorb/core/comm_fw.h"

#ifndef _TIDORB_CORE_COMM_IIOP_FW_H_
#define _TIDORB_CORE_COMM_IIOP_FW_H_


#include <vector>
#include <set>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
  using std::set;
#endif


// Forward Declarations

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {
  
  class ListenPoint;
  typedef vector<ListenPoint> VectorListenPoint;
  typedef set<ListenPoint> ListenPointSet;
 
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

