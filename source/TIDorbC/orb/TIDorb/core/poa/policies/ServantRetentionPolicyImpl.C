/////////////////////////////////////////////////////////////////////////
//
// File:        ServantRetentionPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa/policies.h"


TIDorb::core::poa::policies::ServantRetentionPolicyImpl::ServantRetentionPolicyImpl(PortableServer::ServantRetentionPolicyValue value) 
{
  destroyed = false; 
  _value = value;
};

TIDorb::core::poa::policies::ServantRetentionPolicyImpl::~ServantRetentionPolicyImpl() {
};
    
PortableServer::ServantRetentionPolicyValue TIDorb::core::poa::policies::ServantRetentionPolicyImpl::value() throw() {
	return _value;
};
  
CORBA::PolicyType TIDorb::core::poa::policies::ServantRetentionPolicyImpl::policy_type() throw() {
	return PortableServer::SERVANT_RETENTION_POLICY_ID;
};
  
CORBA::Policy_ptr TIDorb::core::poa::policies::ServantRetentionPolicyImpl::copy() throw() {
  return new ServantRetentionPolicyImpl(_value);
};
  	
CORBA::Boolean TIDorb::core::poa::policies::ServantRetentionPolicyImpl::_is_a(const char* repositoryIdentifier)
{
	if(repositoryIdentifier == NULL)
		throw CORBA::BAD_PARAM("Null string reference");

	if(strcmp(repositoryIdentifier,"IDL:omg.org/PortableServer/ServantRetentionPolicy:1.0")==0)
		return true;
	
	return ::CORBA::LocalObject::_is_a(repositoryIdentifier);

};
  
CORBA::Boolean TIDorb::core::poa::policies::ServantRetentionPolicyImpl::_is_equivalent(CORBA::Object_ptr other)
{
  	if(other == NULL)
		throw CORBA::BAD_PARAM("Null Object reference");
	
	PortableServer::ServantRetentionPolicy* aux_policy = dynamic_cast<PortableServer::ServantRetentionPolicy*>(other);
	
	if (aux_policy)
		return true;
	else 
		return false;			
	
};
       
void TIDorb::core::poa::policies::ServantRetentionPolicyImpl::destroy() throw() {
};
                                             

