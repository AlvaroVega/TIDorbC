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
