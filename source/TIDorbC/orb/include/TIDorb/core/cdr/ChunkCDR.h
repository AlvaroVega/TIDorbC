/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/ChunkCDR.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
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
        //jagd  
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
   //jagd
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
