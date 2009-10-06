//////////////////////////////////////////////////////////////////////////////////
//
// File:        CompressorImpl.C
// Description: Compression::Compressor implementation.
//
// Rel:         01.00
// Created:     October, 2007
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
//
// Revised:
//
// Copyright 2007 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/compression/CompressorImpl.h"


TIDorb::core::compression::CompressorImpl::CompressorImpl(
                                      TIDorb::core::TIDORB* orb,
                                      Compression::CompressionLevel level,
                                      Compression::CompressorFactory_ptr factory)
  throw (TIDThr::SystemException)
  : m_level(level)
{
  this->TIDThr::RefCounter::_add_ref();
  m_factory = Compression::CompressorFactory::_duplicate(factory);

  m_compressed_bytes = 0;
  m_uncompressed_bytes = 0;
  m_ratio = 0;
}


TIDorb::core::compression::CompressorImpl::~CompressorImpl() 
  throw (TIDThr::SystemException)
{
  CORBA::release(m_factory);
}


  
Compression::CompressorFactory_ptr 
TIDorb::core::compression::CompressorImpl::compressor_factory()
{
  return Compression::CompressorFactory::_duplicate(m_factory);
}
    

Compression::CompressionLevel 
TIDorb::core::compression::CompressorImpl::compression_level()
{
  return m_level;
}


CORBA::ULongLong 
TIDorb::core::compression::CompressorImpl::compressed_bytes()
{ 
  return m_compressed_bytes;
}  


CORBA::ULongLong 
TIDorb::core::compression::CompressorImpl::uncompressed_bytes()
{ 
  return m_uncompressed_bytes;
}


Compression::CompressionRatio TIDorb::core::compression::CompressorImpl::compression_ratio()
{ 
  return m_ratio;
}
