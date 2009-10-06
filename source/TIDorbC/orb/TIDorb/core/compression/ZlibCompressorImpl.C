
//////////////////////////////////////////////////////////////////////////////////
//
// File:        ZlibCompressorImpl.h
// Description: Compression::ZlibCompressor implementation.
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

#include "TIDorb/core/compression/ZlibCompressorImpl.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "zlib.h"


TIDorb::core::compression::ZlibCompressorImpl::ZlibCompressorImpl(
                                        TIDorb::core::TIDORB* orb,
                                        Compression::CompressionLevel level,
                                        Compression::CompressorFactory_ptr factory)
  throw (TIDThr::SystemException)
  : TIDorb::core::compression::CompressorImpl(orb, level, factory)
{
  chunk_size = orb->conf().ziop_chunk_size;
}



TIDorb::core::compression::ZlibCompressorImpl::~ZlibCompressorImpl() 
  throw(TIDThr::SystemException)
{

}

void TIDorb::core::compression::ZlibCompressorImpl::compress(
                                      const ::CORBA::OctetSeq& source, 
                                      ::CORBA::OctetSeq& target)
  throw (Compression::CompressionException)
{

  // TODO: Control error -> throw adequeate exceptions
  // target must have memory now??


  int ret, flush;

  z_stream strm;

  /* allocate deflate state */
  strm.zalloc = Z_NULL;
  strm.zfree  = Z_NULL;
  strm.opaque = Z_NULL;

  ret = deflateInit(&strm, m_level);
  if (ret != Z_OK)
    throw Compression::CompressionException();


  unsigned have;

  unsigned char out[chunk_size]; // temporal buffer

  CORBA::ULong  source_length = source.length();
  const CORBA::Octet* source_buffer = source.get_buffer();
   
  CORBA::ULong  target_length = 0;
  //CORBA::Octet* target_buffer = target.get_buffer();

  // Set initial input buffer
  strm.avail_in = source_length;
  strm.next_in  = (CORBA::Octet*) source_buffer;

  // Set flush flag: strm.next_in has the whole input
  flush = Z_FINISH;

  // Run deflate() on input until output buffer not full, finish
  // compression if all of source has been read in 
  do {

    strm.avail_out = chunk_size;
    strm.next_out  = out;
    ret = deflate(&strm, flush);    /* no bad return value */
    assert(ret != Z_STREAM_ERROR);  /* state not clobbered */

    have = chunk_size - strm.avail_out;
    if (have > 0) {
      // Copy 'out' buffer to 'target' OctetSeq
      CORBA::Long current_length = target_length;
      target_length += have;
      target.length(target_length+1);
      memcpy(&(target[current_length]), out, have);
    }

  } while (strm.avail_out == 0);
  assert(strm.avail_in == 0);     /* all input will be used */
      
  (void)deflateEnd(&strm);
  
  //m_factory->add_sample(target.length(), source.length());
  m_compressed_bytes   += target.length();
  m_uncompressed_bytes += source.length();
}
    

void TIDorb::core::compression::ZlibCompressorImpl::decompress(
                                      const ::CORBA::OctetSeq& source, 
                                      ::CORBA::OctetSeq& target)
  throw (Compression::CompressionException)
{
  // TODO: Control error -> throw adequeate exceptions
  int ret;

  z_stream strm;
  
  /* allocate inflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;

  ret = inflateInit(&strm);
  if (ret != Z_OK)
    throw Compression::CompressionException();
  

  unsigned char out[chunk_size]; // temporal buffer

  CORBA::ULong  source_length = source.length();
  const CORBA::Octet* source_buffer = source.get_buffer();
   
  CORBA::ULong  target_length = 0;
  //CORBA::Octet* target_buffer = target.get_buffer();

  // Set initial input buffer
  strm.avail_in = source_length;
  strm.next_in  = (CORBA::Octet*) source_buffer;

  unsigned have;


  /* run inflate() on input until output buffer not full */
  do {

    strm.avail_out = chunk_size;
    strm.next_out  = out;
    
    ret = inflate(&strm, Z_NO_FLUSH);
    assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
    switch (ret) {
    case Z_NEED_DICT:
      ret = Z_DATA_ERROR;     /* and fall through */
      throw Compression::CompressionException();
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
      (void)inflateEnd(&strm);
      throw Compression::CompressionException();
    }

    have = chunk_size - strm.avail_out;
    if ( have > 0) {
      // Copy 'out' buffer to 'target' OctetSeq
      CORBA::Long current_length = target_length;
      target_length += have;
      target.length(target_length);
      memcpy(&(target[current_length]), out, have);
    }

  } while (strm.avail_out == 0);
  
  
  /* clean up and return */
  (void)inflateEnd(&strm);
}
 
