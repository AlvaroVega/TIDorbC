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


