/////////////////////////////////////////////////////////////////////////
//
// File:        ORBComponent.C
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




TIDorb::core::iop::ORBComponent::ORBComponent() throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_ORB_TYPE)
{
	_str = NULL;
	orb_type = 0;
}




TIDorb::core::iop::ORBComponent::ORBComponent(CORBA::ULong type) throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_ORB_TYPE)
{
	_str = NULL;
	orb_type = type;
}




TIDorb::core::iop::ORBComponent::ORBComponent(const ORBComponent& v) throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_ORB_TYPE)
{
	_str = NULL;
	orb_type = v.orb_type;
}




TIDorb::core::iop::ORBComponent::~ORBComponent() throw(TIDThr::SystemException)
{
	CORBA::string_free(_str);
}



  
void TIDorb::core::iop::ORBComponent::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((ORBComponent*)this));
  
  if(!_component_data)
  {
    //jagd
    //TIDorb::core::cdr::CDROutputStream encapsulation (dynamic_cast<TIDorb::core::TIDORB*>(out.orb()),
    TIDorb::core::cdr::CDROutputStream encapsulation ((TIDorb::core::TIDORB*)(out.orb()),
                                                      new TIDorb::core::cdr::BufferCDR(
                                                      2 * TIDorb::core::cdr::CDR::ULONG_SIZE));
                       
    encapsulation.write_boolean(encapsulation.get_byte_order());
    
    encapsulation.write_ulong(orb_type);
   
    //jagd 
    //((ORBComponent*) this)->_component_data = dynamic_cast<TIDorb::core::cdr::CDRInputStream* >
    ((ORBComponent*) this)->_component_data = (TIDorb::core::cdr::CDRInputStream* )
                                              (encapsulation.create_input_stream());
  }
  
  TaggedComponent::write(out);
}



	
void TIDorb::core::iop::ORBComponent::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  TaggedComponent::partial_read(input);
/*  
  CORBA::Boolean byte_order;
  _component_data->read_boolean(byte_order);
  _component_data->set_byte_order(byte_order);
*/
  _component_data->read_ulong(orb_type);
  _component_data->rewind();
}




const char* TIDorb::core::iop::ORBComponent::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;

    buffer << "ORBType " << orb_type;
    ((ORBComponent*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
