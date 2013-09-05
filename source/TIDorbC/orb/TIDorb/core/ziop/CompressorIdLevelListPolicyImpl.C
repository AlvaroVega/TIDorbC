/////////////////////////////////////////////////////////////////////////
//
// File:        CompressorIdLevelListPolicyImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2007
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

#include "TIDorb/core/ziop/CompressorIdLevelListPolicyImpl.h"



TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::CompressorIdLevelListPolicyImpl(
                                          Compression::CompressorIdLevelList ids)
  : m_compressors(ids)
{
  this->TIDThr::RefCounter::_add_ref();
  destroyed = false;
}

TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::~CompressorIdLevelListPolicyImpl() 
  throw (TIDThr::SystemException)
{

}
  

Compression::CompressorIdLevelList*
TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::compressor_ids()
{
  return new Compression::CompressorIdLevelList(m_compressors);
}


CORBA::PolicyType 
TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::policy_type()
{
  return ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID;
}

CORBA::Policy_ptr 
TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::copy() 
  throw(CORBA::SystemException)
{
  if (destroyed) 
    throw CORBA::OBJECT_NOT_EXIST();

  return new CompressorIdLevelListPolicyImpl(m_compressors);
}

void TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::destroy()
{
  destroyed = true;
}

TIDorb::core::ziop::CompressorIdLevelListPolicyImpl* 
TIDorb::core::ziop::CompressorIdLevelListPolicyImpl::read(
                                         TIDorb::core::cdr::CDRInputStream& input)
{
  Compression::CompressorIdLevelList_var compressors =
    new Compression::CompressorIdLevelList();
  compressors->length(0);

  // MARSHAL: Bug in TIDIdlc??
  // Compression::_CompressorIdLevelListHelper::read(input, *compressors);

  CORBA::ULong _length;
  input.read_ulong(_length);
  compressors->length(_length);
  for (CORBA::ULong i=0; i < _length; i++) {
    input.read_ushort((*compressors)[i].compressor_id);
    input.read_ushort((*compressors)[i].compression_level);
  }
  return new CompressorIdLevelListPolicyImpl(*compressors);
}
