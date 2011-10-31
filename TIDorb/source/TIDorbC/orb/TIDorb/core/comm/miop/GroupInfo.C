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

#include "TIDorb/core/comm/miop/GroupInfo.h"




TIDorb::core::comm::miop::GroupInfo::GroupInfo()
	throw (TIDThr::SystemException)
	: TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_GROUP) 
{
	_str = NULL;
	_group_domain_id = NULL;
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
    TIDorb::core::TIDORB* orb = (TIDorb::core::TIDORB*)(out.orb());
    TIDorb::core::cdr::CDROutputStream encapsulation(orb, new TIDorb::core::cdr::BufferCDR(TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE));
                       
    encapsulation.write_boolean(encapsulation.get_byte_order());

    encapsulation.write_octet((CORBA::Octet)_version.getMajor());
    encapsulation.write_octet((CORBA::Octet)_version.getMinor());
    encapsulation.write_string(_group_domain_id);
    encapsulation.write_ulonglong(_object_group_id);
    encapsulation.write_ulong(_object_group_ref_version);
    
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


  _component_data->read_string(_group_domain_id);
  _component_data->read_ulonglong(_object_group_id);
  _component_data->read_ulong(_object_group_ref_version);
  
  _component_data->rewind();
}




const char* TIDorb::core::comm::miop::GroupInfo::toString() const
{
  if (!_str) {
//     // Performance improvement: GCC uses ostrstream as StrinbBuffer, 
//     // and basic_streambuf::xsputn uses memcpy in each "<<" call
//     TIDorb::util::StringBuffer buffer;
//     buffer << "GroupInfo {" << _version.toString() << ", " // 11 bytes + 12 bytes + 2
//            << _group_domain_id << ", "
//            << _object_group_id << ", " // 4 bytes: ULongLong
//            << _object_group_ref_version << " }";     // 4bytes: ULong                   
//     ((GroupInfo*) this)->_str = CORBA::string_dup(buffer.str().data());
    // TODO: see sync_with_stdio
    // Another way: use in buffer a more big internal_buffer
    unsigned int _length = 11 + 12 + 2 + // Adition of constant terms is optimized by compiler
      strlen(_group_domain_id) + 2 +
      16 + 2 +
      16 + 2;
    char* _str_aux = (char*) malloc (_length);
    sprintf(_str_aux, "%s%s%s%s%s%0*d%s%0*d%s", "GroupInfo {", _version.toString(), ", ",
            _group_domain_id, ", ",
            16, _object_group_id, ", ",
            16, _object_group_ref_version, " }");
    ((GroupInfo*) this)->_str = _str_aux;
  }
  return _str;
}
