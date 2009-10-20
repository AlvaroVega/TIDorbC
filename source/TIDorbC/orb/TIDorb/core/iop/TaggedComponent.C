/////////////////////////////////////////////////////////////////////////
//
// File:        TaggedComponent.C
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

#include "TIDorb/core.h"




TIDorb::core::iop::TaggedComponent::TaggedComponent(CORBA::ULong tag)
 throw (TIDThr::SystemException)
{
	_tag = tag;
	_component_data = NULL;
}



  
TIDorb::core::iop::TaggedComponent::TaggedComponent(
	CORBA::ULong tag, TIDorb::core::cdr::CDRInputStream* component_data)
	 throw (TIDThr::SystemException)
{
	_tag = tag;
	_component_data = component_data;
}




TIDorb::core::iop::TaggedComponent::~TaggedComponent() 
	throw (TIDThr::SystemException)
{
	delete _component_data;
}




void TIDorb::core::iop::TaggedComponent::write(TIDorb::core::cdr::CDROutputStream& output) const
{
	output.write_ulong(_tag);
	//output.write_Encapsulation(_component_data);
	output.write_buffer(*(_component_data->get_buffer()));
}



	
void TIDorb::core::iop::TaggedComponent::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
	_component_data = input.read_encapsulation();
}




void TIDorb::core::iop::TaggedComponent::skip(TIDorb::core::cdr::CDRInputStream& input)
{  
	input.skip_ulong(); //tag
	input.skip_encapsulation(); // component_data
}



const char* TIDorb::core::iop::TaggedComponent::toString() const
{
	return "";
}
