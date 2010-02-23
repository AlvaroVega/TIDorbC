/* -----------------------------------------------------------------------------

File: IteratorCDR.C

Revisions:

Autor:
caceres@tid.es

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

#ifndef _TIDORB_CORE_CDR_ITERATOR_CDR_H
#define _TIDORB_CORE_CDR_ITERATOR_CDR_H

#include "CORBA.h"

#include "TIDorb/core/comm/iiop/Version.h"


#include <inttypes.h>
//jagd 
#define NEXT_ALIGNED_POSITION(pos,type_size)\
                {\
                  --type_size; \
                  pos=(CDR::Addr) (((uintptr_t)m_next + ((uintptr_t)type_size)) & ~((uintptr_t)type_size));\
                }\


#define GO_NEXT_ALIGNED_POSITION(size)\
                {\
                  CDR::Addr next_position ;\
                  NEXT_ALIGNED_POSITION(next_position,size);\
                  if (next_position < m_chunk->m_buffer_end)\
                  {\
                    m_next = next_position;\
                  }\
                  else\
                  {\
                    get_next_chunk();\
                  }\
                }\


#define ENOUGH_SPACE(type_size)\
          (((uintptr_t) m_chunk->m_buffer_end >= ((uintptr_t) m_next + (uintptr_t) type_size)))


namespace TIDorb {
  namespace core {
    namespace cdr {

      class IteratorCDR {

        public:
        //jagd 4
        IteratorCDR(){};

        IteratorCDR(TIDorb::core::TIDORB* orb);
        
        IteratorCDR(TIDorb::core::TIDORB* orb, BufferCDR* buffer);       

        IteratorCDR(const IteratorCDR& buffer);

        virtual ~IteratorCDR() {}

        ContextCDR* get_context() const {
            return m_bufferCDR->get_context();
        }

        /**
        *  @return <code>true</code> if little-endian
        *          or <code>false</code>if big-endian
        */

        CORBA::Boolean get_byte_order() const
        {
          return m_byte_order;
        }

        /**
        *  Sets the byte order.
        *  @param byte_order is<code>true</code> if little-endian
        *                    or <code>false</code>if big-endian
        */

        void set_byte_order(CORBA::Boolean byte_order)
		{
    		m_byte_order = byte_order;
			m_byte_swap = (m_byte_order != CDR::LOCAL_BYTE_ORDER);
		}

        /**
        Stream start positioning in the buffer
        */

        void fix_starting()
		{
		m_starting = PointerCDR(m_bufferCDR, m_current_chunk_num, m_next);
		}
        
        void rewind()
		{
    		m_bufferCDR = m_starting.get_buffer();
    		m_current_chunk_num = m_starting.get_num_chunk();
    		m_next = m_starting.get_position();
		}
        
        void goBack(const PointerCDR& pointer)
		{
    		m_bufferCDR = pointer.get_buffer();
    		m_current_chunk_num = pointer.get_num_chunk();
    		m_next = pointer.get_position();
		}
        
        void set_version(const TIDorb::core::comm::iiop::Version& version)
        {
          m_version = version;
        }
        
        const TIDorb::core::comm::iiop::Version& get_version() const
        {
          return m_version;
        }
        
        /**
        *  Returns the buffer CDR.
        */

        BufferCDR* get_buffer() const
        {
          return m_bufferCDR;
        }

        /**
        *  @return the chunk pointered.
        */
        ChunkCDR* get_chunk() const
        {
          return m_chunk;
        }

        bool is_message() const
        {
          return m_message;
        }

        void set_message(bool val);
       
        /**
        *  @return the remaining bytes of the current chunk.
        */

        CORBA::ULong available() const
        {
              CORBA::Long av = (CORBA::Long) (m_chunk->get_available_end() - (CDR::Addr) m_next);
              if(av < 0)
                return 0;
              else
                return av;
        }

        /**
        *  @return the remaining bytes of the buffer.
        */

        CORBA::ULong buffer_available() const
		{
		   CORBA::ULong av = available();
		   CORBA::ULong num_chunks = m_bufferCDR->get_num_available_chunks();
		   
		   for(CORBA::ULong i = num_chunks - 1; i > m_current_chunk_num; i--)
		     av += m_bufferCDR->get_chunk(i)->get_available_bytes();
		   
		   return av;
		}

        /**
        *  @return a pointer to actual position in the buffer,
        */

        PointerCDR get_pointer() const
        {
          PointerCDR p(m_bufferCDR, m_current_chunk_num, m_next);
          return p;
        }

        CDR::AbsolutePosition get_absolute_position() const
		{
    		PointerCDR p(m_bufferCDR, m_current_chunk_num, m_next);
    		CDR::AbsolutePosition position = p.get_absolute_position();
    		position += get_context()->get_position_from_root();
    		return position;
		}
        	

        /**
        *  Sets the covering position. It assumes if the new position is less than
        * actual, the position is in a father encapsulation, of if greater is in the
        * same encapsulation.
        * @return <code>true</code> if OK, or <code>false</code> if it is a bad position.
        */

        virtual void skip(CORBA::ULong n)
        {
          m_next += n;
        }

        /**
        * Sets the writing pointer in the correct aligmnet for the data
        * type. If necessary, gets the next chunk.<p>
        * CORBA 2.3 specification says: A primitive data type of 8 bytes or smaller,
        * should never be broken across two fragments.Therefore, if the remaining bytes are not sufficient for reading the data
        * next chunk will be set for reading.<p>
        * If it is reading a encapsulation and the data is broken into two chunks,
        * reads the data compoentes byte per byte.
        * @param <code>type_size</code> the type size for alignment.
        */

        inline void alignment(CORBA::ULong type_size)
		{
		    // set the aligned position
	
                    //jagd 	
		    //go_next_aligned_position(type_size);
                    GO_NEXT_ALIGNED_POSITION(type_size); 		

		    //if(!enough_space(type_size))
		    if(!ENOUGH_SPACE(type_size))
		    {
		      // there is no space for whole data bytes
		      m_chunk->set_available_end(m_next);
		     // marshaling in the main buffer, get next chunk
		      get_next_chunk();
		    }
		}

        /**
        * Set the stream pointer to the next aligned position of given data type
        * @param type_size the type size in octets
        */
        void go_next_aligned_position(CORBA::ULong type_size)
		{
                  //jagd
		  //CDR::Addr next_position = next_aligned_position(type_size);
		  CDR::Addr next_position ;
		  NEXT_ALIGNED_POSITION(next_position,type_size); 
		  //jagd if (next_position < m_chunk->get_buffer_end())
		  if (next_position < m_chunk->m_buffer_end)
		  {
		    m_next = next_position;
		    return;
		  }
		
		  get_next_chunk();
		}


        //jagd
        void go_next_aligned_position_sin_exception(CORBA::ULong type_size)
        {
            CDR::Addr next_position ;
            NEXT_ALIGNED_POSITION(next_position,type_size);
            //jagd if (next_position < m_chunk->get_buffer_end())
            if (next_position < m_chunk->m_buffer_end)
            {
              m_next = next_position;
              return;
            }
            m_current_chunk_num++;

            if (m_current_chunk_num >= m_bufferCDR->get_num_chunks()) {
                             return; 
                }
            m_chunk = m_bufferCDR->get_chunk(m_current_chunk_num);
            m_next = m_chunk->get_buffer();


        }


        protected:

        /**
        * Gets from the buffer the next chunk. Used by get_next_chunk
        */

        void next_chunk()
		{
	  		m_current_chunk_num++;
	
		 		if (m_current_chunk_num >= m_bufferCDR->get_num_chunks()) {
	      			throw CORBA::MARSHAL("END OF BUFFER",0, CORBA::COMPLETED_NO);
    		}
	  		m_chunk = m_bufferCDR->get_chunk(m_current_chunk_num);
	  		m_next = m_chunk->get_buffer();
		}

        /**
        * Makes the iterator go from one to the next chunk.
        */

        virtual void get_next_chunk() = 0;


        /**
        * Calculates the aligned position for the given type size. The aligment is
        * refered at the actual encapsulation beginning.
        * @param type_size the data type size.
        */
        
        CDR::Addr next_aligned_position(CORBA::ULong type_size) const
		{
		  return (CDR::Addr) (((uintptr_t)m_next + ((uintptr_t)--type_size)) & ~((uintptr_t)type_size));
		}        	


        /**
        *  Says if the data must be aligned.
        * @param type_size the data type size
        */

        bool must_align(CORBA::ULong type_size)
        {
          return m_next != next_aligned_position(type_size);
        }

        /**
        * Changes aligns the position for the given type size.The aligment is
        * refered at the actual encapsulation beginning.
        * @param type_size the data type size.
        */

        void align(CORBA::ULong type_size)
        {
          m_next = next_aligned_position(type_size);
        }

        /**
        *  Says if there is enougth space in the actual chunk for writting
        *  data from a given size. Assumes the position is aligned.
        * @param type_size the data type size.
        * @return <code>true<\code> if there is enougth space or <code>false<\code> otherwise.
        */

        bool enough_space(CORBA::ULong type_size)
        {
          return ((uintptr_t) m_chunk->get_buffer_end() >=
          ((uintptr_t) m_next + (uintptr_t) type_size));
        }
//MLG
        //TIDorb::core::TIDORB_ref  m_orb;
        TIDorb::core::TIDORB* m_orb;
//EMLG        
        /**
        * Byte order of data in stream: big-endian if <code>true</code>, or little-endian if
        * <code>false</code>
        */

        CORBA::Boolean m_byte_order;

        /**
        * If the buffer byte order is different from the local machine, do byte_swap
        */

        bool m_byte_swap;
        
        /**
        * The buffer controled by the buffer;
        */

        BufferCDR_ref m_bufferCDR;
       

        /**
        * Actual Chunk Number.
        */
        CORBA::ULong m_current_chunk_num;

        /**
        *  The chunk covered
        */
        ChunkCDR* m_chunk;


        /**
        * The stream mark
        */

        PointerCDR m_starting;

        /**
        * The actual position in the chunk
        */
        CDR::Addr m_next;

        /**
        * Tells if the buffer corresponds to a message
        */

        bool m_message;

        /**
        * GIOP version for marshalling
        */

        TIDorb::core::comm::iiop::Version m_version;
      };

    } // namespace cdr
  } // namespace core
} // namespace TIDorb


#endif
