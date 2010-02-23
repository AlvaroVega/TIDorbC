/////////////////////////////////////////////////////////////////////////
//
// File:        InvocationPoliciesContext.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_INVOCATIONPOLICIESCONTEXT_H_
#define _TIDORB_CORE_COMM_IIOP_INVOCATIONPOLICIESCONTEXT_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class InvocationPoliciesContext : public ServiceContext {

public:

  TIDorb::core::PolicyContext* m_policies;

  InvocationPoliciesContext() throw (TIDThr::SystemException);

  InvocationPoliciesContext(TIDorb::core::PolicyContext* policies) 
    throw (TIDThr::SystemException);
 
  ~InvocationPoliciesContext() throw (TIDThr::SystemException);
  
  TIDorb::core::PolicyContext* getPolicies();

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  
  void write(TIDorb::core::cdr::CDROutputStream& output) const;

	
};
} //iiop	
} //comm
} //core
} //TIDorb
#endif

