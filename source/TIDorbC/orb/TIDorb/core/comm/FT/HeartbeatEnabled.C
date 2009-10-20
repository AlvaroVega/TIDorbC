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
