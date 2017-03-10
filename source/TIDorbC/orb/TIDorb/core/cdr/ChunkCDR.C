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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/cdr.h"


/**
 * Constructor, gets the control of the byte array. It will have only a
 * fragment and it will not be increaseable. (increments size +8 for alignment)
 */
TIDorb::core::cdr::ChunkCDR::ChunkCDR(CORBA::ULong fixed_size)
{
  int size_buffer = fixed_size + CDR::MAX_ALIGNMENT;
  m_real_buffer = (CORBA::Octet*) malloc(size_buffer);
 
  if(!m_real_buffer)
    throw CORBA::NO_MEMORY();
    
  m_buffer = CDR::align(m_real_buffer);
  m_available_end = m_buffer;
  m_buffer_end = m_buffer + fixed_size;
  m_length  = fixed_size;
  m_with_header = false;
}

TIDorb::core::cdr::ChunkCDR::~ChunkCDR()
{
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

