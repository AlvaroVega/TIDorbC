/////////////////////////////////////////////////////////////////////////
//
// File:        ChunkCDR.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/cdr.h"


/**
 * Constructor, gets the control of the byte array. It will have only a
 * fragment and it will not be increaseable. (increments size +8 for alignment)
 */
TIDorb::core::cdr::ChunkCDR::ChunkCDR(CORBA::ULong fixed_size)
{
  //MLG
  int size_buffer = fixed_size + CDR::MAX_ALIGNMENT;
  //m_real_buffer = new CORBA::Octet[size_buffer];
  m_real_buffer = (CORBA::Octet*) malloc(size_buffer);

  //caceres: performance improvements
  //memset(m_real_buffer,0,size_buffer*sizeof(CORBA::Octet));
  
  //EMLG
  if(!m_real_buffer)
    throw CORBA::NO_MEMORY();

  //PRA  
  //if(!m_real_buffer) 
  //  throw CORBA::NO_MEMORY();
  //EPRA
    
  m_buffer = CDR::align(m_real_buffer);
  m_available_end = m_buffer;
  m_buffer_end = m_buffer + fixed_size;
  m_length  = fixed_size;
  m_with_header = false;
}

TIDorb::core::cdr::ChunkCDR::~ChunkCDR()
{
  //delete[] m_real_buffer;
  free(m_real_buffer);
}

TIDorb::core::cdr::ChunkCDR*
TIDorb::core::cdr::ChunkCDR::duplicate()
{
  TIDorb::core::cdr::ChunkCDR* new_chunk = 
    new TIDorb::core::cdr::ChunkCDR(this->m_length);

  memcpy(new_chunk->m_real_buffer, this->m_real_buffer, 
         this->m_length+CDR::MAX_ALIGNMENT);
  // Calculate with relative offsets of orignal chunk
  new_chunk->m_buffer = new_chunk->m_real_buffer + 
    (this->m_buffer - this->m_real_buffer);

  // These never change
  new_chunk->m_length = this->m_length;
  new_chunk->m_with_header = this->m_with_header;

  // Calculate with relative offsets of orignal chunk
  new_chunk->m_buffer_end = new_chunk->m_buffer + new_chunk->m_length;
  new_chunk->m_available_end = new_chunk->m_buffer + 
    (this->m_available_end - this->m_buffer);
  
  return new_chunk;
}

