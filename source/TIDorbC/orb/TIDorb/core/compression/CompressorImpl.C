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
