/////////////////////////////////////////////////////////////////////////
//
// File:        ProfileMultipleComponents.C
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




TIDorb::core::iop::ProfileMultipleComponents::ProfileMultipleComponents() 
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_MULTIPLE_COMPONENTS)
{
 	_profile_data = NULL;
 	_components_extracted = false;
}




TIDorb::core::iop::ProfileMultipleComponents::ProfileMultipleComponents
  (const TIDorb::core::iop::VectorTaggedComponent& tagged_components) 
  throw (TIDThr::SystemException)
  :TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_MULTIPLE_COMPONENTS)
{
	_components = tagged_components;
	_components_extracted = true;
}




TIDorb::core::iop::ProfileMultipleComponents::~ProfileMultipleComponents() 
  throw (TIDThr::SystemException)
{
	delete _profile_data;
}



	
//ATENCION - MCPG
void TIDorb::core::iop::ProfileMultipleComponents::extract_members()
{

  if(!_components_extracted)
  {
    TIDThr::Synchronized synchro(*this);

    if(_components_extracted)  // May be changed after return of synchro
      return;

    if(!_profile_data)
    throw CORBA::INTERNAL("ProfileMultipleComponents without marshaled data");

    _profile_data->rewind();

    CORBA::ULong size;
    _profile_data->read_ulong(size);

    if (size < 0)
      throw CORBA::MARSHAL("Invalid component size");

    if (size > 0) {
      _components.resize(size);

      for(size_t i = 0; i < size; i++){        
        _components[i] = TIDorb::core::iop::TaggedComponentReader::read(*_profile_data);
      }
    }
    _components_extracted = true;
  }
}




const TIDorb::core::iop::VectorTaggedComponent& 
  TIDorb::core::iop::ProfileMultipleComponents::getTaggedComponents() const
{
	((ProfileMultipleComponents*)this)->extract_members();
	return _components;
}




bool TIDorb::core::iop::ProfileMultipleComponents::operator== (const TIDorb::core::iop::TaggedProfile& other) const
{
	const TIDorb::core::iop::ProfileMultipleComponents* profile =
	        //jagd	
                //dynamic_cast<const TIDorb::core::iop::ProfileMultipleComponents*>(&other);
		(const TIDorb::core::iop::ProfileMultipleComponents*)(&other);

	if((profile == NULL) || (_tag != profile->_tag))
		return false;

        if(! _components_extracted )
                ((ProfileMultipleComponents*) this)->extract_members();

	// TODO: implement TAG_MULTIPLE_COMPONENTS comparison 
	return true;
}




//ATENCION -MCPG	
void TIDorb::core::iop::ProfileMultipleComponents::partial_write(TIDorb::core::cdr::CDROutputStream& output) const
{
  TIDThr::Synchronized synchro(*((ProfileMultipleComponents*) this));
	
  if(_profile_data) {
    output.write_buffer(*(_profile_data->get_buffer()));
  } else {
    if(!_components_extracted)
      throw CORBA::INTERNAL();
 
    output.write_ulong((CORBA::ULong) _components.size());
	
    for(size_t i = 0; i <_components.size(); i++)
      _components[i]->write(output);
  }
}



	
void TIDorb::core::iop::ProfileMultipleComponents::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{  
	_profile_data = input.copy();
    
    input.skip_encapsulation();


// Other ORBs like MICO IORs with have MultipleComponentProfile faults here
//  	_profile_data = input.copy();
    
//  	_profile_data->fix_starting();
    
// 	CORBA::ULong length;
// 	input.read_ulong(length);

// 	_components.resize(length);
        
// 	for(size_t i = 0; i <length; i++)
//           TIDorb::core::iop::TaggedComponent::skip(input);  
}
