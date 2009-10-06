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
// Copyright 2007 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

