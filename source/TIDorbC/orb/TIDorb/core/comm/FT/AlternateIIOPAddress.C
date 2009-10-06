//////////////////////////////////////////////////////////////////////////////////
//
// File:        AlternateIIOPAddress.C
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

#include "TIDorb/core/comm/FT/AlternateIIOPAddress.h"




TIDorb::core::comm::FT::AlternateIIOPAddress::AlternateIIOPAddress()
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_ALTERNATE_IIOP_ADDRESS)
{
  _str = NULL;
}




TIDorb::core::comm::FT::AlternateIIOPAddress::AlternateIIOPAddress(const TIDorb::core::comm::iiop::ListenPoint& addr)
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_ALTERNATE_IIOP_ADDRESS)
{
  _str = NULL;
  _listen_point = addr;
}




TIDorb::core::comm::FT::AlternateIIOPAddress::AlternateIIOPAddress(const AlternateIIOPAddress& other)
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_ALTERNATE_IIOP_ADDRESS)
{
  _str = CORBA::string_dup(other._str);
  _listen_point = other._listen_point;
}




TIDorb::core::comm::FT::AlternateIIOPAddress::~AlternateIIOPAddress()
  throw (TIDThr::SystemException)
{
  CORBA::string_free(_str);
}




bool TIDorb::core::comm::FT::AlternateIIOPAddress::operator== (const AlternateIIOPAddress& other) const
{
  return (_listen_point == other._listen_point);
}




void TIDorb::core::comm::FT::AlternateIIOPAddress::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((AlternateIIOPAddress*) this));
  
  if (!_component_data) {
    //TIDorb::core::TIDORB* orb = dynamic_cast<TIDorb::core::TIDORB*>(out.orb());
    TIDorb::core::TIDORB* orb = (TIDorb::core::TIDORB*)(out.orb());
    TIDorb::core::cdr::CDROutputStream encapsulation(orb, new TIDorb::core::cdr::BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE));
                       
    encapsulation.write_boolean(encapsulation.get_byte_order());

    encapsulation.write_string(_listen_point._host);
    encapsulation.write_ushort(_listen_point._port);
    
    ((AlternateIIOPAddress*) this)->_component_data = dynamic_cast<TIDorb::core::cdr::CDRInputStream*>
                                                      (encapsulation.create_input_stream());
  }
  
  TIDorb::core::iop::TaggedComponent::write(out);
}




void TIDorb::core::comm::FT::AlternateIIOPAddress::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  TIDorb::core::iop::TaggedComponent::partial_read(input);
/*  
  CORBA::Boolean byte_order;
  _component_data->read_boolean(byte_order);
  _component_data->set_byte_order(byte_order);
*/
  _listen_point.read(*_component_data);

  _component_data->rewind();
}




const char* TIDorb::core::comm::FT::AlternateIIOPAddress::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;

    buffer << "AlternateIIOPAddress {" << _listen_point.toString() << "}";
    ((AlternateIIOPAddress*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
