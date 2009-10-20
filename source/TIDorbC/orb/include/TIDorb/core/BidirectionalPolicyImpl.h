//////////////////////////////////////////////////////////////////////////////////
//
// File:        BidirectionalPolicyImpl.h
// Description: BiDirPolicy::BidirectionalPolicy implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_BidirectionalPolicyImpl_H_
#define _TIDORB_CORE_BidirectionalPolicyImpl_H_


/**
 * Implementation of BidirectionalPolicy
 * @version 1.0
*/
namespace TIDorb {
namespace core {

class BidirectionalPolicyImpl : public BiDirPolicy::BidirectionalPolicy,
                                public CORBA::LocalObject
{
private:

	BiDirPolicy::BidirectionalPolicyValue _value;
	bool destroyed;
	
public:
  
  BidirectionalPolicyImpl(BiDirPolicy::BidirectionalPolicyValue value);
  
  //CORBA::Short value();

  BiDirPolicy::BidirectionalPolicyValue value() throw (CORBA::SystemException);
  
  //CORBA::Long policy_type();
  
  CORBA::PolicyType policy_type() throw (CORBA::SystemException);
  
  CORBA::Policy_ptr copy() throw(CORBA::SystemException);
  
  void destroy() throw();
  
  static BidirectionalPolicyImpl* read(TIDorb::core::cdr::CDRInputStream& input);

};
}
}
#endif
