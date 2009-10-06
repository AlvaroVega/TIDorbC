//////////////////////////////////////////////////////////////////////////////////
//
// File:        HeartbeatEnableFT.C
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

#include "TIDorb/core/comm/FT/HeartbeatEnabled.h"




TIDorb::core::comm::FT::HeartbeatEnabled::HeartbeatEnabled()
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_FT_HEARTBEAT_ENABLED)
{
  _str = NULL;
}




TIDorb::core::comm::FT::HeartbeatEnabled::HeartbeatEnabled(CORBA::Boolean enabled)
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_FT_HEARTBEAT_ENABLED)
{
  _str = NULL;
  heartbeat_enabled = enabled;
}




TIDorb::core::comm::FT::HeartbeatEnabled::HeartbeatEnabled(const HeartbeatEnabled& other)
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_FT_HEARTBEAT_ENABLED)
{
  _str = CORBA::string_dup(other._str);
  heartbeat_enabled = other.heartbeat_enabled;
}




TIDorb::core::comm::FT::HeartbeatEnabled::~HeartbeatEnabled()
  throw (TIDThr::SystemException)
{
  CORBA::string_free(_str);
}




bool TIDorb::core::comm::FT::HeartbeatEnabled::operator== (const HeartbeatEnabled& other) const
{
  return (heartbeat_enabled == other.heartbeat_enabled);
}




void TIDorb::core::comm::FT::HeartbeatEnabled::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((HeartbeatEnabled*) this));
  
  if (!_component_data) {
    //TIDorb::core::TIDORB* orb = dynamic_cast<TIDorb::core::TIDORB*>(out.orb());
    TIDorb::core::TIDORB* orb = (TIDorb::core::TIDORB*)(out.orb());
    TIDorb::core::cdr::CDROutputStream encapsulation(orb, new TIDorb::core::cdr::BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE));
                       
    encapsulation.write_boolean(encapsulation.get_byte_order());

    encapsulation.write_boolean(heartbeat_enabled);
    
    ((HeartbeatEnabled*) this)->_component_data = dynamic_cast<TIDorb::core::cdr::CDRInputStream*>
                                                      (encapsulation.create_input_stream());
  }
  
  TIDorb::core::iop::TaggedComponent::write(out);
}




void TIDorb::core::comm::FT::HeartbeatEnabled::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  TIDorb::core::iop::TaggedComponent::partial_read(input);
/*  
  CORBA::Boolean byte_order;
  _component_data->read_boolean(byte_order);
  _component_data->set_byte_order(byte_order);
*/
  _component_data->read_boolean(heartbeat_enabled);

  _component_data->rewind();
}




const char* TIDorb::core::comm::FT::HeartbeatEnabled::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;

    buffer << "HeartbeatEnabled {" 
           << ((heartbeat_enabled) ? "true" : "false")
           << "}";
    ((HeartbeatEnabled*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
