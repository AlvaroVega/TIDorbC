/////////////////////////////////////////////////////////////////////////
//
// File:        PoliciesComponent.C
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/messaging/PoliciesComponent.h"


TIDorb::core::messaging::PoliciesComponent::PoliciesComponent()
  : TIDorb::core::iop::TaggedComponent(Messaging::TAG_POLICIES),
    m_policies(NULL)
{
  _str = NULL;
}

TIDorb::core::messaging::PoliciesComponent::PoliciesComponent(
                                                 TIDorb::core::PolicyContext* policies)
  : TIDorb::core::iop::TaggedComponent(Messaging::TAG_POLICIES),
    m_policies(policies)
{
  _str = NULL;
}

TIDorb::core::messaging::PoliciesComponent::PoliciesComponent(
                                                 const TIDorb::core::messaging::PoliciesComponent& v)
  : TIDorb::core::iop::TaggedComponent(Messaging::TAG_POLICIES),
    m_policies(v.m_policies)
{
  _str = NULL;
}


TIDorb::core::messaging::PoliciesComponent::~PoliciesComponent()
  throw (TIDThr::SystemException)
{
  CORBA::string_free(_str);

  if (m_policies != NULL)
    delete m_policies;
}


void TIDorb::core::messaging::PoliciesComponent::partial_read(
                                                  TIDorb::core::cdr::CDRInputStream& input){

  CORBA::string_free(_str);
  _str = NULL;

  if (m_policies != NULL)
    delete m_policies;

  m_policies = new PolicyContext();
  
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();

  m_policies->partialRead(*encapsulation);

  delete encapsulation;

}
    
void TIDorb::core::messaging::PoliciesComponent::write(
                                           TIDorb::core::cdr::CDROutputStream& output) const {

  output.write_ulong(Messaging::TAG_POLICIES);               
  
  // enter ecapsulation
  TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();

  if(m_policies == NULL) {
    encapsulation->write_ulong(0);
  } else {
    m_policies->write(*encapsulation);
  }
  
  output.write_buffer(*(encapsulation->get_buffer()));

  delete encapsulation;
  
}
  
TIDorb::core::PolicyContext* TIDorb::core::messaging::PoliciesComponent::getPolicies(){
  return m_policies; 
}



const char* TIDorb::core::messaging::PoliciesComponent::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;

    buffer << "policies "; 
    ((PoliciesComponent*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
