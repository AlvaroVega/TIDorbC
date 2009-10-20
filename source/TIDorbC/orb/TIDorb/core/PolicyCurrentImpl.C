/////////////////////////////////////////////////////////////////////////
//
// File:        PolicyCurrentImpl.C
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


TIDorb::core::PolicyCurrentImpl::PolicyCurrentImpl(
                                       TIDorb::core::PolicyContextManagerImpl* manager)
  : m_manager(manager)
{

}

TIDorb::core::PolicyCurrentImpl:: ~PolicyCurrentImpl()
{

}
  
CORBA::PolicyList* 
TIDorb::core::PolicyCurrentImpl::get_policy_overrides(const CORBA::PolicyTypeSeq& ts)
{
  TIDorb::core::PolicyContext* context = 
    m_manager->tryToGetThreadContext(TIDThr::Thread::currentThread());
  
  if (context == NULL)
    return NULL;
  
  return context->getPolicies(ts);
}
  

void TIDorb::core::PolicyCurrentImpl::set_policy_overrides(
                                                const CORBA::PolicyList& policies,
                                                CORBA::SetOverrideType set_add)
  throw (CORBA::InvalidPolicies)
{
  TIDorb::core::PolicyContext* context =
    m_manager->getThreadContext(TIDThr::Thread::currentThread());
  
  context->setPolicies(policies, set_add);
}
  

TIDorb::core::PolicyContext* TIDorb::core::PolicyCurrentImpl::getPolicyContext()
{
  return m_manager->tryToGetThreadContext(TIDThr::Thread::currentThread());
}
