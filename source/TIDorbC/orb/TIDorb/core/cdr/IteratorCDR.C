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


