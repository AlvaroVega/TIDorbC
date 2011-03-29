/////////////////////////////////////////////////////////////////////////
//
// File:        QOPPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     June, 2009
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

#include "TIDorb/core/security/QOPPolicyImpl.h"



TIDorb::core::security::QOPPolicyImpl::QOPPolicyImpl(Security::QOP qop)
{
  destroyed = false;
  m_qop = qop;
  this->TIDThr::RefCounter::_add_ref();
}


TIDorb::core::security::QOPPolicyImpl::~QOPPolicyImpl() 
  throw (TIDThr::SystemException)
{

}

Security::QOP TIDorb::core::security::QOPPolicyImpl::qop()
{
  return m_qop;
}

CORBA::PolicyType TIDorb::core::security::QOPPolicyImpl::policy_type()
{
  return Security::SecQOPPolicy;
}

CORBA::Policy_ptr TIDorb::core::security::QOPPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new QOPPolicyImpl(m_qop);
}

void TIDorb::core::security::QOPPolicyImpl::destroy()
{
  destroyed = true;
}
