/////////////////////////////////////////////////////////////////////////
//
// File:        EstablishTrustPolicyImpl.C
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

#include "TIDorb/core/security/EstablishTrustPolicyImpl.h"



TIDorb::core::security::EstablishTrustPolicyImpl::EstablishTrustPolicyImpl(
                                          Security::EstablishTrust establish_trust)
{
  destroyed = false;
  m_establish_trust = establish_trust;
  this->TIDThr::RefCounter::_add_ref();
}


TIDorb::core::security::EstablishTrustPolicyImpl::~EstablishTrustPolicyImpl() 
  throw (TIDThr::SystemException)
{

}

Security::EstablishTrust TIDorb::core::security::EstablishTrustPolicyImpl::trust()
{
  return m_establish_trust;
}

CORBA::PolicyType TIDorb::core::security::EstablishTrustPolicyImpl::policy_type()
{
  return Security::SecEstablishTrustPolicy;
}

CORBA::Policy_ptr TIDorb::core::security::EstablishTrustPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed)
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new EstablishTrustPolicyImpl(m_establish_trust);
}

void TIDorb::core::security::EstablishTrustPolicyImpl::destroy()
{
  destroyed = true;
}
