/////////////////////////////////////////////////////////////////////////
//
// File:        BidirectionalPolicyImpl.C
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

#include "TIDorb/core/BidirectionalPolicyImpl.h"
  
TIDorb::core::BidirectionalPolicyImpl::BidirectionalPolicyImpl(BiDirPolicy::BidirectionalPolicyValue value) {
	_value = value;
	destroyed = false;
};
    
BiDirPolicy::BidirectionalPolicyValue TIDorb::core::BidirectionalPolicyImpl::value() throw(CORBA::SystemException)
{
	if (destroyed) 
		throw CORBA::OBJECT_NOT_EXIST();

    	return _value;
};
  
CORBA::PolicyType TIDorb::core::BidirectionalPolicyImpl::policy_type() throw(CORBA::SystemException)
{
	if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();

  return BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE;
};
  
CORBA::Policy_ptr TIDorb::core::BidirectionalPolicyImpl::copy() throw(CORBA::SystemException)
{
	if (destroyed) 
  		throw CORBA::OBJECT_NOT_EXIST();

    	return new BidirectionalPolicyImpl(_value);
};
  
void TIDorb::core::BidirectionalPolicyImpl::destroy() throw()
{
	destroyed = true;
};

TIDorb::core::BidirectionalPolicyImpl* 
TIDorb::core::BidirectionalPolicyImpl::read(TIDorb::core::cdr::CDRInputStream& input)
{

  BiDirPolicy::BidirectionalPolicyValue value;
  BiDirPolicy::_BidirectionalPolicyValueHelper::read(input, value);

  if ((value != BiDirPolicy::BOTH) && 
      (value != BiDirPolicy::NORMAL)) {
    throw CORBA::MARSHAL();
  }
  
  return new TIDorb::core::BidirectionalPolicyImpl(value);        
}
