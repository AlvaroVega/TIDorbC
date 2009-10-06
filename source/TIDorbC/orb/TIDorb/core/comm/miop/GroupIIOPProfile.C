//////////////////////////////////////////////////////////////////////////////////
//
// File:        GroupIIOPProfile.C
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

#include "TIDorb/core/comm/miop/GroupIIOPProfile.h"




TIDorb::core::comm::miop::GroupIIOPProfile::GroupIIOPProfile()
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP_IIOP)
{
  _str = NULL;
}




TIDorb::core::comm::miop::GroupIIOPProfile::GroupIIOPProfile
  (const TIDorb::core::comm::iiop::ProfileIIOP& profile)
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP_IIOP)
{
  _str = NULL;
  _iiop_profile = profile;
}




TIDorb::core::comm::miop::GroupIIOPProfile::GroupIIOPProfile
  (const GroupIIOPProfile& other)
  throw (TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP_IIOP)
{
  _str = NULL;
  _iiop_profile = other._iiop_profile;
}




TIDorb::core::comm::miop::GroupIIOPProfile::~GroupIIOPProfile()
  throw (TIDThr::SystemException)
{
  CORBA::string_free(_str);
}




bool TIDorb::core::comm::miop::GroupIIOPProfile::operator== (const GroupIIOPProfile& other) const
{
  return (_iiop_profile == other._iiop_profile);
}




void TIDorb::core::comm::miop::GroupIIOPProfile::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((GroupIIOPProfile*) this));
  
  if (!_component_data) {
    //TIDorb::core::TIDORB* orb = dynamic_cast<TIDorb::core::TIDORB*>(out.orb());
    TIDorb::core::TIDORB* orb = (TIDorb::core::TIDORB*)(out.orb());
    TIDorb::core::cdr::CDROutputStream encapsulation(orb, new TIDorb::core::cdr::BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE));
                       
    encapsulation.write_boolean(encapsulation.get_byte_order());
    _iiop_profile.partial_write(encapsulation);
    
//     ((GroupIIOPProfile*) this)->_component_data = dynamic_cast<TIDorb::core::cdr::CDRInputStream*>
//                                                   (encapsulation.create_input_stream());
    ((GroupIIOPProfile*) this)->_component_data = (TIDorb::core::cdr::CDRInputStream*)
                                                  (encapsulation.create_input_stream());
  }
  
  TIDorb::core::iop::TaggedComponent::write(out);
}




void TIDorb::core::comm::miop::GroupIIOPProfile::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  TIDorb::core::iop::TaggedComponent::partial_read(input);
/*  
  CORBA::Boolean byte_order;
  _component_data->read_boolean(byte_order);
  _component_data->set_byte_order(byte_order);
*/
  _iiop_profile.partial_read(*_component_data);

  _component_data->rewind();
}




const char* TIDorb::core::comm::miop::GroupIIOPProfile::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;
    char* profile = _iiop_profile.toString();

    buffer << "GroupIIOPProfile {" << profile << "}";
    CORBA::string_free(profile);

    ((GroupIIOPProfile*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
