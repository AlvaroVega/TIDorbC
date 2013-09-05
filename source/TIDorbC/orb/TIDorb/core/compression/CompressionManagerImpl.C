//////////////////////////////////////////////////////////////////////////////////
//
// File:        CompressionManagerImpl.C
// Description: CompressionManager implementation.
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


#include "TIDorb/core/compression/CompressionManagerImpl.h"


TIDorb::core::compression::CompressionManagerImpl::CompressionManagerImpl(
                                                        TIDorb::core::TIDORB* orb)
  : m_orb(orb)
{

  // Creates and registers default provided compressor: ZLib
  Compression::CompressorId zlib_id = Compression::COMPRESSORID_ZLIB;

  Compression::CompressorFactory_ptr zlib_factory = 
    new TIDorb::core::compression::CompressorFactoryImpl(m_orb, zlib_id);
  m_factories[zlib_id] = zlib_factory;
}


TIDorb::core::compression::CompressionManagerImpl::~CompressionManagerImpl()
{

  CompressorFactoryMapT::iterator it;
  CompressorFactoryMapT::iterator end = m_factories.end();
  
  for (it = m_factories.begin(); it != end; it++) {
    CORBA::release((*it).second);
  }
  m_factories.clear();

}

 
void TIDorb::core::compression::CompressionManagerImpl::register_factory(
                            Compression::CompressorFactory_ptr compressor_factory)
  throw (Compression::FactoryAlreadyRegistered)
{
  TIDThr::Synchronized synchro(m_recursive_mutex);
 
  Compression::CompressorId comp_id = compressor_factory->compressor_id();

  CompressorFactoryMapT::iterator it = m_factories.find(comp_id);
  
  if (it == m_factories.end()){
    m_factories[comp_id] = compressor_factory;
  }
  else
    throw Compression::FactoryAlreadyRegistered();

}
  
  
void TIDorb::core::compression::CompressionManagerImpl::unregister_factory(
                                          Compression::CompressorId compressor_id)
  throw (Compression::UnknownCompressorId)
{
  TIDThr::Synchronized synchro(m_recursive_mutex);

  CompressorFactoryMapT::iterator it = m_factories.find(compressor_id);
  
  if (it != m_factories.end()){
    m_factories.erase(it);
  }
  else
    throw Compression::UnknownCompressorId();
}
  
  
Compression::CompressorFactory_ptr 
TIDorb::core::compression::CompressionManagerImpl::get_factory(
                                          Compression::CompressorId compressor_id)
  throw (Compression::UnknownCompressorId)
{
  TIDThr::Synchronized synchro(m_recursive_mutex);
  
  CompressorFactoryMapT::iterator it = m_factories.find(compressor_id);
  
  if (it != m_factories.end()){
    Compression::CompressorFactory_ptr factory = 
      Compression::CompressorFactory::_duplicate((*it).second);
    return factory;
  }
  else{

    if ( (compressor_id != Compression::COMPRESSORID_GZIP) &&
         (compressor_id != Compression::COMPRESSORID_PKZIP) &&
         (compressor_id != Compression::COMPRESSORID_BZIP2) &&
         (compressor_id != Compression::COMPRESSORID_ZLIB) &&
         (compressor_id != Compression::COMPRESSORID_LZMA) &&  
         (compressor_id != Compression::COMPRESSORID_LZOP) &&  
         (compressor_id != Compression::COMPRESSORID_RZIP) &&  
         (compressor_id != Compression::COMPRESSORID_7X) &&    
         (compressor_id != Compression::COMPRESSORID_XAR) )   
      throw Compression::UnknownCompressorId();

    Compression::CompressorFactory_ptr factory = 
      new TIDorb::core::compression::CompressorFactoryImpl(m_orb, 
                                                           compressor_id);
    return factory;
  }

}


Compression::Compressor_ptr 
TIDorb::core::compression::CompressionManagerImpl::get_compressor(
                                   Compression::CompressorId compressor_id, 
                                   Compression::CompressionLevel compression_level)
  throw (Compression::UnknownCompressorId)
{
  TIDThr::Synchronized synchro(m_recursive_mutex);

  CompressorFactoryMapT::iterator it = m_factories.find(compressor_id);
  
  if (it != m_factories.end()){
    Compression::CompressorFactory_var factory = 
      Compression::CompressorFactory::_duplicate((*it).second);
    return factory->get_compressor(compression_level);
  }
  else
    throw Compression::UnknownCompressorId();
}
  
  
Compression::CompressorFactorySeq* 
TIDorb::core::compression::CompressionManagerImpl::get_factories()
{
  TIDThr::Synchronized synchro(m_recursive_mutex);
  
  Compression::CompressorFactorySeq* factories = 
    new Compression::CompressorFactorySeq();
  
  factories->length(m_factories.size());
  
  CompressorFactoryMapT::iterator it;
  CompressorFactoryMapT::iterator end = m_factories.end();
  CORBA::ULong i = 0;
  
  for (it = m_factories.begin(); it != end; it++) {
    (*factories)[i] = Compression::CompressorFactory::_duplicate((*it).second);
    i++;
  }
  
  return factories;
}
