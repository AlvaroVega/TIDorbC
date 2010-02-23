/////////////////////////////////////////////////////////////////////////
//
// File:        PoliciesComponent.h
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

#include "TIDorb/core/messaging.h"

#ifndef _TIDORB_CORE_MESSAGING_POLICIES_COMPONENT_H_
#define _TIDORB_CORE_MESSAGING_POLICIES_COMPONENT_H_

#include "CORBA.h"
#include "Messaging.h"


namespace TIDorb {
namespace core {
namespace messaging {

class PoliciesComponent: public virtual TIDorb::core::iop::TaggedComponent
{

private:

  char* _str;

public:

  TIDorb::core::PolicyContext* m_policies;
 
  PoliciesComponent();
  
  PoliciesComponent(TIDorb::core::PolicyContext* policies);

  PoliciesComponent(const PoliciesComponent& v);

  ~PoliciesComponent() throw (TIDThr::SystemException);

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
    
  void write(TIDorb::core::cdr::CDROutputStream& output) const;
  
  TIDorb::core::PolicyContext* getPolicies();
    
  const char* toString() const;

};
}// messaging
}// core
}// TIDorb
#endif
