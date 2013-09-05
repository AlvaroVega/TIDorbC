//////////////////////////////////////////////////////////////////////////////////
//
// File:        BiDirServiceContext.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/BiDirServiceContext.h"

void TIDorb::core::comm::iiop::BiDirServiceContext::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();

  try {
    CORBA::ULong size;
    encapsulation->read_ulong(size);
    
    ListenPoint ls;
    
    for(CORBA::ULong i = 0; i < size; i++){
      ls.read(*encapsulation);		
      listen_points.push_back(ls);
    }
    
    delete encapsulation;
    
  }catch (...) {
    delete encapsulation;
    throw;
  }
};
	
void TIDorb::core::comm::iiop::BiDirServiceContext::write(TIDorb::core::cdr::CDROutputStream& output) const
{
  output.write_ulong(_context_id);
  
  
  TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
  
  try {
    
    encapsulation->write_ulong((CORBA::ULong) listen_points.size());
    
    for(CORBA::ULong i = 0; i <listen_points.size(); i++)
      listen_points[i].write(*encapsulation);  	
    
    output.write_buffer(*(encapsulation->get_buffer()));
    
    delete encapsulation;
  	
  }catch (...) {
    delete encapsulation;
    throw;
  }
};
	
