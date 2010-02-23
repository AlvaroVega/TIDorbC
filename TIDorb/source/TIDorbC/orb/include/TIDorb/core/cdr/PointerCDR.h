/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/PointerCDR.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/cdr.h"

#ifndef _TIDorb_core_cdr_PointerCDR_H_
#define _TIDorb_core_cdr_PointerCDR_H_

#include "CORBA.h"


namespace TIDorb {
namespace core {
namespace cdr {
 
class PointerCDR {
	
	public: 
  	/**
  	 * Constructor: creates a pointer to position of a chunk in a buffer.
  	 */
  	PointerCDR(): m_buffer(0), m_num_chunk(0), m_position(0){}
  	 
  	PointerCDR(BufferCDR* buffer, CORBA::ULong num_chunk, CDR::Addr position)
  	:	m_buffer(buffer), m_num_chunk(num_chunk), m_position(position), m_abs_calculated(false)
  	{	//m_absolute_position = 0;
  	}
  	
  	PointerCDR(const PointerCDR& pointer)
  	:	m_buffer(pointer.m_buffer), m_num_chunk(pointer.m_num_chunk), m_position(pointer.m_position)
  	{	  	 
    }
    
  	
  	BufferCDR* get_buffer() const
  	{
  	  return m_buffer;
  	}
  	
  	CORBA::ULong get_num_chunk() const 
    {
        return m_num_chunk;
    }
  	
  	CDR::Addr get_position() const 
    {
        return m_position;
    }
  		
  	/**
  	 * Calculates the absolute position of a pointer from the begining of the buffer.
  	 * @return the absulte position from the begining of the buffer.
  	*/ 
  
  	CDR::AbsolutePosition get_absolute_position() const;   	
 
	private:
  	/**
  	 * Buffer pointered.
  	 */
  	BufferCDR* m_buffer;
  	
  	/**
  	 * Chunk number of the buffer.
  	 */
  	CORBA::ULong m_num_chunk;
  	
  	/**
  	 * Position in the chunk
  	 */
  	CDR::Addr m_position;
  	
   /**
	  * Absolute position in the buffer.
	  */
	  CDR::AbsolutePosition m_absolute_position;
	  
	 /**
	  * Absolute position in the buffer.
	  */
	  bool m_abs_calculated;
	 
	  
}; // class PointerCDR

} // namespace cdr
} // namespace core
} // namespace TIDorb

#endif
