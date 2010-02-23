/////////////////////////////////////////////////////////////////////////
//
// File:        PointerCDR.C
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


TIDorb::core::cdr::CDR::AbsolutePosition 
TIDorb::core::cdr::PointerCDR::get_absolute_position() const
{
  
  if (!m_abs_calculated) {
    
    CORBA::ULongLong total_position = 0;
    // add all the precedent chunks sizes
    for(CORBA::ULongLong i=0; i < m_num_chunk; i++)
      total_position+= m_buffer->get_chunk(i)->get_length();
    // add the actual chunk position
    total_position+= m_position - m_buffer->get_chunk(m_num_chunk)->get_buffer();
    ((PointerCDR*) this)->m_absolute_position = total_position;
    ((PointerCDR*) this)->m_abs_calculated = true;
  }
  
  return m_absolute_position;
}
