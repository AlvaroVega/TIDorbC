/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/ChunkCDR.h
  
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

#ifndef _TIDORB_CORE_CDR_CHUNKCDR_H_
#define _TIDORB_CORE_CDR_CHUNKCDR_H_

#include "CORBA.h"


namespace TIDorb {
namespace core {
namespace cdr {
  
class ChunkCDR {
  	
	public:


     ChunkCDR(){};	
  	/**
  	 * Constructor, gets the control of the byte array. It will have
  	 * only a fragment and it will not be increaseable. (increments size +8 for alignemt
  	 */
  	ChunkCDR(CORBA::ULong fixed_size); 
  	
//    ChunkCDR(CORBA::Octet* buffer, CORBA::ULong size); 
    
//    ChunkCDR(const CORBA::Octet* buffer, CORBA::ULong size); 
    
    ~ChunkCDR();
  
    bool has_header() const  {return m_with_header;}
  
    void set_header(CORBA::Boolean value) {m_with_header = value;} 
  	/**
  	 * @return the byte array associated to the buffer. 
  	 */
  	CDR::Addr get_buffer() const {return m_buffer;}
  	
  	/**
  	 * @return the buffer limit (this value contains no data). 
  	 */
  	CDR::Addr get_buffer_end() const {return m_buffer_end;}
  	/**
  	 *  Sets the available bytes to 0. It is used to reuse the chunk buffer for new data. 
  	 */
  	void recycle() {m_available_end = m_buffer;}
  	
  	/**
  	 * Sets the available bytes to the <code>value</code> position. An error can appear
  	 * if the value is higher than the buffer length.
  	 * @return <code>true</code> if OK or <code>false</code> on error. 
  	 */
  	bool set_available_bytes(CORBA::ULong value)
        {
  		if(m_length >= value) {
    			m_available_end = m_buffer + value;
    			return true;
  		} else {
    			return false;
  		}
	}

  	/**
  	 * Sets the available end to the <code>value</code> position. An error can appear
  	 * if the value is higher than the buffer length.
  	 * @return <code>true</code> if OK or <code>false</code> on error. 
  	 */
  	inline bool set_available_end(CDR::Addr position)
	{
  		if((position >= m_buffer) && (position <= m_buffer_end)) {
    			m_available_end = position;
    			return true;
  		} else {
    			return false;
  		}
	}
   	
  	/**
  	 * @return The available bytes. 
  	 */
  
  	CORBA::ULong get_available_bytes() const {return m_available_end - m_buffer;}
  	
  	/**
  	 * @return The available bytes. 
  	 */
  	inline CDR::Addr get_available_end() const {return m_available_end;}
  	
  	/**
  	 * @return The lentgh ofthe byte buffer. 
  	 */
  
  	CORBA::ULong get_length() const {return m_length;}
  	
  protected:
  	/**
  	 *	firs aligned position.
  	 */
  	CDR::Addr m_buffer;
 	  	
  	/**
  	 *	bytes of the chunk.
  	 */
  	CDR::Addr m_real_buffer;
  	
  	/**
  	 *	last available position, this position is not included
  	 */
  	CDR::Addr m_available_end;
  	
  	/**
  	 *	buffer limint: this position is not included
  	 */
  public:

  	CDR::Addr m_buffer_end;

        ChunkCDR* duplicate();
         
  protected:  	
  	/**
  	 * Useful Bytes in Buffer
  	 */ 	
    CORBA::ULong m_length;
  	
   /**
    * The starting of the chunk has an message header
    */ 
    
    CORBA::Boolean m_with_header;
   	
};

} // namespace cdr
} // namespace core
} // namespace TIDorb

#endif // _TIDorb_core_cdr_ChunkCDR_H_
