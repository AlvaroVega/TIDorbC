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
// Copyright 2005 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
	
