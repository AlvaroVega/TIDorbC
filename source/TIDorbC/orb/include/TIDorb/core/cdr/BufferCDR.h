/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/BufferCDR.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/cdr.h"

#ifndef _TIDORB_CORE_CDR_BUFFER_CDR_H_
#define _TIDORB_CORE_CDR_BUFFER_CDR_H_

#include "CORBA.h"
#include "TIDThr.h"

#include <vector>
#include <memory>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif

// PABLO ***
// using std::allocator; 
// using namespace std;
// PABLO ***


namespace TIDorb {
namespace core {
namespace cdr {

class BufferCDR : public virtual TIDThr::RefCounter {
  
  public: 
      BufferCDR(){}; 
      /**
       * Header size in the array format.
       */ 
      static const size_t ARRAY_HEADER_SIZE = 5;
    
      /**
       * Header size in the string format.
       */   
      static const size_t STRING_HEADER_SIZE = 14;
    	
      /**
       *  Creates a new empty buffer monochunk for marshaling.
       */
     
      BufferCDR(CORBA::ULong block_size,            
                const ContextCDR* father_context = 0,
                CDR::AbsolutePosition  position_from_root = 0);

      /**
       *  Creates a buffer monochunk with the chunk.
       */
      
      BufferCDR(ChunkCDR* chunk,            
                const ContextCDR* father_context = 0,
                CDR::AbsolutePosition  position_from_root = 0);
    
      ~BufferCDR() throw (TIDThr::SystemException);

        /**
         *  Sets the byte_order
         */ 
         
         void set_byte_order(bool order)
         {
            m_byte_order = order;
         }
         
         bool get_byte_order()
         {
            return m_byte_order;
         }

      /**
       * Returns the number of chunks.
       */
      size_t get_num_chunks() const
      {
        return m_chunks.size();
      } 


	/**
	 * @return <code>true</code> if it has an unique chunk,<code>false</code> otherwise.
	 */
	bool is_mono_chunk() const
	{
	  return (m_chunks.size() == 1);
	} 
	
	ChunkCDR* get_chunk (size_t index) const;
	
	/**
	 * Prepare the buffer to be reused.
	 */	 
	void recycle();
			
	/**
	 * Insert a new chunk in the buffer.
	 */
	bool add_chunk(ChunkCDR* chunk); 

	
	/**
	 * Insert a new chunk in the buffer at a position less than the buffer size.
	 */
	bool insert_chunk(ChunkCDR* chunk, size_t at);


	/**
	 * Creates and inserts a new chunk ant the end of the buffer.
	 * @return <code>true</code> if OK or <code>false</code> on error;
	 */

	bool grow(); 
	
	CORBA::ULong get_available_bytes() const;

	void set_available_bytes(CORBA::ULong available);
	
	CORBA::ULong get_num_available_chunks() const;


	/**
	 * Compares the data of two <code>BufferCDR</code>
	 * @return <code> true</code> if the data contained is equal.
	 */
	bool equal(const BufferCDR& buff) const;
    
    
    /**
     * Returns the indirection context
     */
     
    ContextCDR* get_context() {
        return &m_context;
    }
	
    BufferCDR* duplicate();
  
  protected:
  
    
    typedef vector< ChunkCDR* > ChunkVector;
    
  	/**
  	 * Vector that contains the buffer chunks.
  	 */
  	ChunkVector m_chunks;	
  	
  	/**
  	 * The size of the new chunks.
  	 */
  	 
  	CORBA::ULong m_chunk_size;
    
   /**
    * Byte order of data in stream: big-endian if <code>true</code>, or little-endian if
    * <code>false</code>
    */

    CORBA::Boolean m_byte_order;
    
    /**
     * Context for indirection resolution
     */
     
    ContextCDR m_context;

};

} // namespace cdr
} // namespace core
} // namespace TIDorb  

#endif
