//////////////////////////////////////////////////////////////////////////////////
//
// File:        ZlibCompressorImpl.h
// Description: Compression::ZlibCompressor implementation using Zlib
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

#ifndef _TIDORB_CORE_COMPRESSION_ZLIB_COMPRESSOR_IMPL_H_
#define _TIDORB_CORE_COMPRESSION_ZLIB_COMPRESSOR_IMPL_H_

#include "CORBA.h"
#include "Compression.h"
#include "TIDThr.h"

#include "zlib.h"


namespace TIDorb {
namespace core {
namespace compression {

class ZlibCompressorImpl: public virtual TIDorb::core::compression::CompressorImpl
{
private:
  size_t chunk_size;

public:
  
  ZlibCompressorImpl(TIDorb::core::TIDORB* orb,
                     Compression::CompressionLevel level,
                     Compression::CompressorFactory_ptr factory)
    throw (TIDThr::SystemException);
  
  ~ZlibCompressorImpl() 
    throw(TIDThr::SystemException);

  
  void compress(const ::CORBA::OctetSeq& source, 
                ::CORBA::OctetSeq& target)
    throw (Compression::CompressionException);
  
  void decompress(const ::CORBA::OctetSeq& source, 
                  ::CORBA::OctetSeq& target)
    throw (Compression::CompressionException);
  
};

}// compression
}// core
}// TIDorb

#endif
