//////////////////////////////////////////////////////////////////////////////////
//
// File:        CompressionManagerImpl.h
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

#include "TIDorb/core/compression.h"

#ifndef _TIDORB_CORE_COMPRESSION_COMPRESSION_MANAGER_IMPL_H_
#define _TIDORB_CORE_COMPRESSION_COMPRESSION_MANAGER_IMPL_H_

#include "CORBA.h"
#include "Compression.h"


namespace TIDorb {
namespace core {
namespace compression {

class CompressionManagerImpl : public virtual Compression::CompressionManager,
                               public virtual CORBA::LocalObject
{

protected:

  TIDorb::core::TIDORB*  m_orb;

  typedef map<Compression::CompressorId, Compression::CompressorFactory_var> CompressorFactoryMapT; 
  CompressorFactoryMapT m_factories;

  TIDThr::RecursiveMutex m_recursive_mutex;

public:
 
  CompressionManagerImpl(TIDorb::core::TIDORB* orb);

  ~CompressionManagerImpl();

 
  void register_factory(Compression::CompressorFactory_ptr compressor_factory)
    throw (Compression::FactoryAlreadyRegistered);
  
  
  void unregister_factory(Compression::CompressorId compressor_id)
    throw (Compression::UnknownCompressorId);
  
  
  Compression::CompressorFactory_ptr get_factory(Compression::CompressorId compressor_id)
    throw (Compression::UnknownCompressorId);
  
  Compression::Compressor_ptr get_compressor(Compression::CompressorId compressor_id, 
                                             Compression::CompressionLevel compression_level)
    throw (Compression::UnknownCompressorId);
  
  
  Compression::CompressorFactorySeq* get_factories();
  
  
};

}
}
}

#endif

