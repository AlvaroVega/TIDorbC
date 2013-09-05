/////////////////////////////////////////////////////////////////////////
//
// File:        ConstructionPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     September, 2009
//
// Revised:    
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"
#include "TIDorb/core/poa/policies.h"




TIDorb::core::ConstructionPolicyImpl::ConstructionPolicyImpl()
{
  destroyed = false;
  this->TIDThr::RefCounter::_add_ref();
}


TIDorb::core::ConstructionPolicyImpl::~ConstructionPolicyImpl()
  throw (TIDThr::SystemException)
{
  destroyed = false;
}

CORBA::PolicyType TIDorb::core::ConstructionPolicyImpl::policy_type()
{
  return CORBA::SecConstruction;
}

 
void TIDorb::core::ConstructionPolicyImpl::make_domain_manager(
                                                   CORBA::InterfaceDef_ptr object_type,
                                                   CORBA::Boolean constr_policy)
{
  CORBA::InterfaceDef::FullInterfaceDescription_var idef =
    object_type->describe_interface();
  
  _constr[idef->id.in()] = constr_policy;
}

CORBA::Boolean 
TIDorb::core::ConstructionPolicyImpl::constr_policy(CORBA::Object_ptr object)
{
  ConstrMapTable::iterator i = 
    _constr.find (object->_get_interface()->describe_interface()->id.in());
  if (i != _constr.end())
    return (*i).second;
  return false;
}


CORBA::Policy_ptr 
TIDorb::core::ConstructionPolicyImpl::copy() throw(CORBA::SystemException)
{
 if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new ConstructionPolicyImpl();
}
 
 
void TIDorb::core::ConstructionPolicyImpl::destroy()
{
  destroyed = true;
}


