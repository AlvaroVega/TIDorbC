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
