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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
