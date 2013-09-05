//////////////////////////////////////////////////////////////////////////////////
//
// File:        InvocationPoliciesContext.C
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/InvocationPoliciesContext.h"



TIDorb::core::comm::iiop::InvocationPoliciesContext::InvocationPoliciesContext()
  throw (TIDThr::SystemException)
  : ServiceContext(IOP::INVOCATION_POLICIES)
{
  m_policies = NULL;
}


TIDorb::core::comm::iiop::InvocationPoliciesContext::InvocationPoliciesContext(
                                                  TIDorb::core::PolicyContext* policies)
  throw (TIDThr::SystemException)
  : ServiceContext(IOP::INVOCATION_POLICIES)

{
  m_policies = policies;
}


TIDorb::core::comm::iiop::InvocationPoliciesContext::~InvocationPoliciesContext()
  throw (TIDThr::SystemException)
{
  if (m_policies != NULL)
    delete m_policies;
}

TIDorb::core::PolicyContext* TIDorb::core::comm::iiop::InvocationPoliciesContext::getPolicies()
{
  return m_policies;
}


void TIDorb::core::comm::iiop::InvocationPoliciesContext::partial_read(
                                                  TIDorb::core::cdr::CDRInputStream& input)
{
  m_policies = new TIDorb::core::PolicyContext(NULL);
  
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();
  
  try {

    m_policies->partialRead(*encapsulation);
  
    delete encapsulation;
    
  }catch (...) {
    delete encapsulation;
    throw;
  }

}

	
void TIDorb::core::comm::iiop::InvocationPoliciesContext::write(
                                                 TIDorb::core::cdr::CDROutputStream& output) const
{

  output.write_ulong(_context_id);
  
  TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
  
  try {

    if(m_policies == NULL) {
      encapsulation->write_ulong((CORBA::ULong) 0);
    } else {
      m_policies->write(*encapsulation);    
    }
    
    output.write_buffer(*(encapsulation->get_buffer())); 

    delete encapsulation;
  	
  } catch (...) {
    delete encapsulation;
    throw;
  }
}
	
