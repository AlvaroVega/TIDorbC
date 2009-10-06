//////////////////////////////////////////////////////////////////////////////////
//
// File:        GroupInfo.C
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

#include "TIDorb/core/comm/miop/GroupInfo.h"




TIDorb::core::comm::miop::GroupInfo::GroupInfo()
	throw (TIDThr::SystemException)
	: TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP) 
{
	_str = NULL;
	_group_domain_id = NULL; //CORBA::string_dup("");
	_object_group_id = 0;
	_object_group_ref_version = 0;
}




TIDorb::core::comm::miop::GroupInfo::GroupInfo(
	const Version& version,
	const char* group_domain_id,
	CORBA::ULongLong object_group_id,
	CORBA::ULong object_group_ref_version)
	throw (TIDThr::SystemException)
	: TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP) 
{
	_str = NULL;
	_version = version;
	_group_domain_id = CORBA::string_dup(group_domain_id);
	_object_group_id = object_group_id;
	_object_group_ref_version = object_group_ref_version;
}




TIDorb::core::comm::miop::GroupInfo::GroupInfo(const TIDorb::core::comm::miop::GroupInfo& other)
	throw (TIDThr::SystemException)
	: TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP) 
{
	_str = CORBA::string_dup(other._str);
	_version = other._version;
	_group_domain_id = CORBA::string_dup(other._group_domain_id);
	_object_group_id = other._object_group_id;
	_object_group_ref_version = other._object_group_ref_version;
}




TIDorb::core::comm::miop::GroupInfo::~GroupInfo()
	throw (TIDThr::SystemException)
{
	CORBA::string_free(_str);
	CORBA::string_free(_group_domain_id);
}




bool TIDorb::core::comm::miop::GroupInfo::operator== (const GroupInfo& other) const
{
	return ((_version == other._version)
	        && (!strcmp(_group_domain_id, other._group_domain_id))
	        && (_object_group_id == other._object_group_id)
	        && (_object_group_ref_version == other._object_group_ref_version));
}




void TIDorb::core::comm::miop::GroupInfo::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((GroupInfo*) this));
  
  if (!_component_data) {
    //TIDorb::core::TIDORB* orb = dynamic_cast<TIDorb::core::TIDORB*>(out.orb());
    TIDorb::core::TIDORB* orb = (TIDorb::core::TIDORB*)(out.orb());
    TIDorb::core::cdr::CDROutputStream encapsulation(orb, new TIDorb::core::cdr::BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE));
                       
    encapsulation.write_boolean(encapsulation.get_byte_order());

    encapsulation.write_octet((CORBA::Octet)_version.getMajor());
    encapsulation.write_octet((CORBA::Octet)_version.getMinor());
    encapsulation.write_string(_group_domain_id);
    encapsulation.write_ulonglong(_object_group_id);
    encapsulation.write_ulong(_object_group_ref_version);
    
//     ((GroupInfo*) this)->_component_data = dynamic_cast<TIDorb::core::cdr::CDRInputStream*>
//                                            (encapsulation.create_input_stream());
    ((GroupInfo*) this)->_component_data = (TIDorb::core::cdr::CDRInputStream*)
                                           (encapsulation.create_input_stream());
  }
  
  TIDorb::core::iop::TaggedComponent::write(out);
}




void TIDorb::core::comm::miop::GroupInfo::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  TIDorb::core::iop::TaggedComponent::partial_read(input);
/*  
  CORBA::Boolean byte_order;
  _component_data->read_boolean(byte_order);
  _component_data->set_byte_order(byte_order);
*/
  CORBA::ULong major, minor;
  CORBA::Octet aux;
  _component_data->read_octet(aux);
  major = aux;
  _component_data->read_octet(aux);
  minor = aux;
  _version = Version(major, minor);

//PRA
//TODO: posible memory leak
//if (_group_domain_id) {
//  CORBA::string_free(_group_domain_id);
//}
//EPRA
  _component_data->read_string(_group_domain_id);
  _component_data->read_ulonglong(_object_group_id);
  _component_data->read_ulong(_object_group_ref_version);
  
  _component_data->rewind();
}




const char* TIDorb::core::comm::miop::GroupInfo::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;

    buffer << "GroupInfo {" << _version.toString() << ", "
           << _group_domain_id << ", "
           << _object_group_id << ", "
           << _object_group_ref_version << " }";                       
    ((GroupInfo*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
