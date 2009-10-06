/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/PointerCDR.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
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
