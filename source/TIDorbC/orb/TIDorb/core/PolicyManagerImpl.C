/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyManagerImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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

#include "TIDorb/core.h"
#include "TIDorb/core/poa/policies.h"




TIDorb::core::PolicyManagerImpl::PolicyManagerImpl(TIDorb::core::TIDORB* orb)
  : m_orb(orb), m_conf(const_cast<TIDorb::core::ConfORB&>(m_orb->conf()))
{
}




CORBA::PolicyList* TIDorb::core::PolicyManagerImpl::get_policy_overrides(
                                                              const CORBA::PolicyTypeSeq& ts)
{
  return m_conf.getPolicyContext().getPolicies(ts);
}




void TIDorb::core::PolicyManagerImpl::set_policy_overrides(const CORBA::PolicyList& policies,
                                                           CORBA::SetOverrideType set_add)
  throw (CORBA::InvalidPolicies)
{
  m_conf.getPolicyContext().setPolicies(policies, set_add);
}
                     


TIDorb::core::PolicyContext& TIDorb::core::PolicyManagerImpl::getPolicyContext()
{
  return m_conf.getPolicyContext();
}
