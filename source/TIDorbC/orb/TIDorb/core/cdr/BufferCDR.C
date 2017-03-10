/////////////////////////////////////////////////////////////////////////
//
// File:        BufferCDR.C
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
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/cdr/BufferCDR.h"
#include <string.h>

namespace TIDorb {
namespace core {
namespace cdr {



BufferCDR::BufferCDR(CORBA::ULong block_size,
                     const ContextCDR* father_context,
                     CDR::AbsolutePosition  position_from_root)
  : m_chunk_size(block_size), m_byte_order(CDR::LOCAL_BYTE_ORDER),
    m_context(father_context, position_from_root)
{	
	m_chunks.push_back(new ChunkCDR(m_chunk_size));    
}

BufferCDR::BufferCDR(ChunkCDR* chunk,
                     const ContextCDR* father_context,
                     CDR::AbsolutePosition  position_from_root)
  : m_chunk_size(chunk->get_length()), m_byte_order(CDR::LOCAL_BYTE_ORDER),
    m_context(father_context, position_from_root)
{
  m_chunks.push_back(chunk);
}

BufferCDR::~BufferCDR()throw (TIDThr::SystemException)
{ 
  ChunkVector::iterator i = m_chunks.begin();
  ChunkVector::iterator end = m_chunks.end();

  while(i < end) {
    delete(*i);
    i++;
  }
  m_chunks.clear();
}

/**
 * @return the chunk associated to de index, or <code>null</code> if not exits.
 */
ChunkCDR* BufferCDR::get_chunk (size_t index) const
{ 
	if (index < m_chunks.size()) 
		return m_chunks[index];
	else 
		return 0;
}
	
	
/**
 * Prepare the buffer to be reused.
 */	 
void BufferCDR::recycle()
{   
  ChunkVector::iterator i = m_chunks.begin();
  ChunkVector::iterator end = m_chunks.end();
  
  while(i < end) {
    (*i)->recycle();
    i++;
  } 
}
			
/**
 * Insert a new chunk in the buffer.
 */
bool BufferCDR::add_chunk(TIDorb::core::cdr::ChunkCDR* chunk) 
{   
	m_chunks.push_back(chunk);
	return true;
}
	
/**
 * Insert a new chunk in the buffer at a position less than the buffer size.
 */
bool BufferCDR::insert_chunk(ChunkCDR* chunk, size_t at)
{  
	if(at >= m_chunks.size())
		return false;
	m_chunks[at] = chunk;
	return true;
}

/**
 * Creates and inserts a new chunk ant the end of the buffer.
 * @return <code>true</code> if OK or <code>false</code> on error;
 */

bool BufferCDR::grow() 
{
  if (m_chunk_size > 0) {
		add_chunk(new ChunkCDR(m_chunk_size));
		return true;
	} else 
		return false;
}


CORBA::ULong BufferCDR::get_available_bytes() const
{    
    CORBA::ULong available = 0;
      
    ChunkVector::const_iterator i = m_chunks.begin();
    ChunkVector::const_iterator end = m_chunks.end();
      
    while(i < end) {
        available += (*i)->get_available_bytes();
        i++;
    } 
    	
    return available;
}

void BufferCDR::set_available_bytes(CORBA::ULong available)
{
  CORBA::ULong allocated = m_chunks.size() * m_chunk_size;
  
  if(available > allocated)
    throw CORBA::INTERNAL("BufferCDR::set_available_bytes available > allocated");
    
  ChunkVector::iterator i = m_chunks.begin();
  ChunkVector::iterator end = m_chunks.end();
  
  while(available > 0) {
    if(available < m_chunk_size) {
          (*i)->set_available_bytes(available);
          available = 0;
    } else {
      (*i)->set_available_bytes(m_chunk_size);
          available -= m_chunk_size;
    }
    i++;
  } 

  // set to 0 the rest of chunks
    
  while(i < end) {
    (*i)->set_available_bytes(0);
    i++;
  }
}
	
CORBA::ULong BufferCDR::get_num_available_chunks() const 
{ 
  CORBA::ULong available = 0;
  
  ChunkVector::const_iterator i = m_chunks.begin();
  ChunkVector::const_iterator end = m_chunks.end();
  
  while(i < end) {
    if((*i)->get_available_bytes() > 0) {
    	available++;
//     } else {
//         break;
    }
	i++;
  } 
	
  return available;
}

/**
 * Compares the data of two <code>BufferCDR</code>
 * @return <code> true</code> if the data contained is equal.
 */
 
bool BufferCDR::equal(const BufferCDR& buff) const
{  	  
	CORBA::ULong available = get_available_bytes();
	
	if(available != buff.get_available_bytes())
		return false;
	CORBA::ULong num_chunks = get_num_available_chunks();
	
	if (num_chunks != buff.get_num_available_chunks())
		return false;

	CDR::Addr buffer_a = 0;
	CDR::Addr buffer_b = 0;
	
	for(CORBA::ULong i= 0; i < num_chunks; i++) {
		buffer_a = get_chunk(i)->get_buffer();
		buffer_b = buff.get_chunk(i)->get_buffer();
		
		CORBA::ULong size = get_chunk(i)->get_available_bytes();
		
		if(memcmp(buffer_a, buffer_b, size))
		  return false;
	}
	
	return true;
	
}


BufferCDR* BufferCDR::duplicate()
{
  BufferCDR* newBufferCDR = new BufferCDR();

  ChunkVector::iterator it = this->m_chunks.begin();
  ChunkVector::iterator end = this->m_chunks.end();
  while(it != end) {
    newBufferCDR->m_chunks.push_back((*it)->duplicate());
    it++;
  }

  newBufferCDR->m_byte_order = this->m_byte_order;

  newBufferCDR->m_chunk_size = this->m_chunk_size;

  this->m_context.duplicate(&(newBufferCDR->m_context)); 


  return newBufferCDR;
}


} // cdr
} // core
} // TIDorb

