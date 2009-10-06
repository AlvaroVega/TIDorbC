//////////////////////////////////////////////////////////////////////////////////
//
// File:        CompressorImpl.h
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

#include "TIDorb/core/compression.h"

#ifndef _TIDORB_CORE_COMPRESSION_COMPRESSOR_IMPL_H_
#define _TIDORB_CORE_COMPRESSION_COMPRESSOR_IMPL_H_

#include "CORBA.h"
#include "Compression.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace compression {

class CompressorImpl: public virtual Compression::Compressor,
                      public TIDThr::RefCounter,
                      public virtual CORBA::LocalObject
{
protected:

  TIDorb::core::TIDORB*              m_orb;
  Compression::CompressionLevel      m_level;
  CORBA::ULongLong                   m_compressed_bytes;
  CORBA::ULongLong                   m_uncompressed_bytes;
  Compression::CompressionRatio      m_ratio;
  Compression::CompressorFactory_ptr m_factory;

public:

  CompressorImpl(TIDorb::core::TIDORB* orb,
                 Compression::CompressionLevel level,
                 Compression::CompressorFactory_ptr factory)
    throw (TIDThr::SystemException);

  ~CompressorImpl() 
    throw (TIDThr::SystemException);
  
  
  virtual void compress(const ::CORBA::OctetSeq& source, 
                        ::CORBA::OctetSeq& target)
    throw (Compression::CompressionException) = 0;
  
  virtual void decompress(const ::CORBA::OctetSeq& source, 
                          ::CORBA::OctetSeq& target)
    throw (Compression::CompressionException) = 0;
  
  Compression::CompressorFactory_ptr compressor_factory();
    
  Compression::CompressionLevel compression_level();

  CORBA::ULongLong compressed_bytes();
  
  CORBA::ULongLong uncompressed_bytes();
  
  Compression::CompressionRatio compression_ratio();
  
};

}// compression
}// core
}// TIDorb

#endif
