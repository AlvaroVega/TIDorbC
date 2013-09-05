/////////////////////////////////////////////////////////////////////////
//
// File:        IteratorCDR.C
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
//#include "TIDorb/core/ConfORB.h"

namespace TIDorb {
namespace core {
namespace cdr {



IteratorCDR::IteratorCDR(TIDorb::core::TIDORB* orb)
:
  m_orb(orb),
  m_byte_order (CDR::LOCAL_BYTE_ORDER),
  m_byte_swap(false),
  m_current_chunk_num(0),
  m_message(false),
  m_version(TIDorb::core::comm::iiop::Version::VERSION_1_2) 
{
 if(m_orb)
   m_bufferCDR = new BufferCDR(m_orb->conf().block_size);
 else 
   m_bufferCDR = new BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE);

  m_chunk = m_bufferCDR->get_chunk(0);
  m_next= m_chunk->get_buffer();
  
  m_starting = PointerCDR(m_bufferCDR, m_current_chunk_num, m_next);
}

IteratorCDR::IteratorCDR(TIDorb::core::TIDORB* orb,
                                            BufferCDR* buffer)
:
  m_orb(orb),
  m_byte_order (CDR::LOCAL_BYTE_ORDER),
  m_byte_swap(false),
  m_bufferCDR (buffer),
  m_current_chunk_num(0),
  m_message(false),
  m_version(TIDorb::core::comm::iiop::Version::VERSION_1_2)
{
  m_chunk = buffer->get_chunk(0);
  m_next = m_chunk->get_buffer();
  m_starting = PointerCDR(m_bufferCDR, m_current_chunk_num, m_next);
}

IteratorCDR::IteratorCDR(const IteratorCDR& other)
 :
  m_orb(other.m_orb),
  m_byte_order(other.m_byte_order),
  m_byte_swap(other.m_byte_swap),
  m_bufferCDR(other.m_bufferCDR),
  m_current_chunk_num(other.m_current_chunk_num),
  m_chunk(other.m_chunk),
  m_starting(other.m_starting),
  m_next(other.m_next),
  m_message(other.m_message),
  m_version(other.m_version)
{  
}

void IteratorCDR::set_message(bool val)
{
	m_message = val;
			  
	if ((val) && (m_current_chunk_num == 0) && (m_next == m_chunk->get_buffer()))
		skip(TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE);
}
 
} // namespaces: cdr
} // core
} // cdr


