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
// Copyright 2007 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
