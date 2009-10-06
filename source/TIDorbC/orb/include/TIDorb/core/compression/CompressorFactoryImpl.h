//////////////////////////////////////////////////////////////////////////////////
//
// File:        CompressorFactoryImpl.h
// Description: Compression::CompressorFactory implementation.
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

#ifndef _TIDORB_CORE_COMPRESSION_COMPRESSOR_FACTORY_IMPL_H_
#define _TIDORB_CORE_COMPRESSION_COMPRESSOR_FACTORY_IMPL_H_

#include "CORBA.h"
#include "Compression.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace compression {

class CompressorFactoryImpl: public virtual Compression::CompressorFactory,
                             public TIDThr::RefCounter, 
                             public virtual CORBA::LocalObject
                        
{
 private:
  TIDorb::core::TIDORB*       m_orb;
  Compression::CompressorId   m_compressor_id;
//   CORBA::ULongLong            m_compressed_bytes;
//   CORBA::ULongLong            m_uncompressed_bytes;

  typedef map<Compression::CompressionLevel, Compression::Compressor_var> CompressorMapT; 
  CompressorMapT m_compressors;

public:
  CompressorFactoryImpl(TIDorb::core::TIDORB* orb,
                        Compression::CompressorId id) throw (TIDThr::SystemException);
  
  ~CompressorFactoryImpl() throw (TIDThr::SystemException);
    


  Compression::CompressorId compressor_id();
  
  
//   CORBA::ULongLong compressed_bytes();
  
  
//   CORBA::ULongLong uncompressed_bytes();
  
  
//   CORBA::Double average_compression();
  
  Compression::Compressor_ptr get_compressor(Compression::CompressionLevel compression_level);
  
  //  void add_sample(CORBA::ULongLong compressed_bytes, CORBA::ULongLong uncompressed_bytes);


    

};

}
}
}

#endif
