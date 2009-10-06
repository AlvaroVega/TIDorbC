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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
	
