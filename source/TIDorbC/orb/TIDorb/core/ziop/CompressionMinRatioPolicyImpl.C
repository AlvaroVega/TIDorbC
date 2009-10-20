/////////////////////////////////////////////////////////////////////////
//
// File:        CompressionMinRatioPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     May, 2008
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

#include "TIDorb/core/ziop/CompressionMinRatioPolicyImpl.h"

TIDorb::core::ziop::CompressionMinRatioPolicyImpl::CompressionMinRatioPolicyImpl(
                                                ZIOP::CompressionMinRatioPolicyValue ratio)
  : m_ratio(ratio)
{
  this->TIDThr::RefCounter::_add_ref();
  destroyed = false;
}


TIDorb::core::ziop::CompressionMinRatioPolicyImpl::~CompressionMinRatioPolicyImpl() 
  throw (TIDThr::SystemException)
{
}

  
ZIOP::CompressionMinRatioPolicyValue
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::ratio()
{
  return m_ratio;
}
  

CORBA::PolicyType 
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::policy_type()
{
  return ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID;
}


CORBA::Policy_ptr 
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();
  
  return new CompressionMinRatioPolicyImpl(m_ratio);
}
    

void TIDorb::core::ziop::CompressionMinRatioPolicyImpl::destroy()
{
  destroyed = true;
}
    

TIDorb::core::ziop::CompressionMinRatioPolicyImpl* 
TIDorb::core::ziop::CompressionMinRatioPolicyImpl::read(
                                          TIDorb::core::cdr::CDRInputStream& input)
{
  ZIOP::CompressionMinRatioPolicyValue value;
  ZIOP::_CompressionMinRatioPolicyValueHelper::read(input, value);
  // TODO: Check value
  return new TIDorb::core::ziop::CompressionMinRatioPolicyImpl(value);
}
