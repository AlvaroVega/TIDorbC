//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServiceContext.C
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

#include "TIDorb/core/comm/iiop/ServiceContext.h"

TIDorb::core::comm::iiop::ServiceContext::ServiceContext(CORBA::ULong context_id)
throw (TIDThr::SystemException)
{
	_context_id = context_id;
	_context_data = NULL;
};
  
TIDorb::core::comm::iiop::ServiceContext::ServiceContext(CORBA::ULong context_id, 
                                                         TIDorb::core::cdr::CDRInputStream* context_data)
 throw (TIDThr::SystemException)
{
	_context_id = context_id;
	_context_data = context_data->copy();
};

TIDorb::core::comm::iiop::ServiceContext::~ServiceContext()  throw (TIDThr::SystemException)
{
  if (_context_data)
	delete _context_data;
};

void TIDorb::core::comm::iiop::ServiceContext::write(TIDorb::core::cdr::CDROutputStream& output) const
{
	output.write_ulong(_context_id);
	//MCPG
	//output.write_encapsulation(_context_data);
	output.write_buffer(*(_context_data->get_buffer()));
};
		
void TIDorb::core::comm::iiop::ServiceContext::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
	_context_data = input.read_encapsulation();
};

void TIDorb::core::comm::iiop::ServiceContext::skip(TIDorb::core::cdr::CDRInputStream& input)
{
	input.skip_ulong(); // skip context_id
	input.skip_encapsulation();
};
	
void TIDorb::core::comm::iiop::ServiceContext::partial_skip(TIDorb::core::cdr::CDRInputStream& input)
{
	//skip encapusulation
	input.skip_encapsulation();
}; 
