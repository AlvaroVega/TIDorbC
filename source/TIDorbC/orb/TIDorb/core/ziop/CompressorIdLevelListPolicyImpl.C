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
// Copyright 2007 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
