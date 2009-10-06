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
