//////////////////////////////////////////////////////////////////////////////////
//
// File:        ProfileMIOP.C
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

#include "TIDorb/core/comm/miop/ProfileMIOP.h"




TIDorb::core::comm::miop::ProfileMIOP::ProfileMIOP() throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_UIPMC)
{
        _profile_data = NULL;
        _components_extracted = false;
}




TIDorb::core::comm::miop::ProfileMIOP::ProfileMIOP
        (const TIDorb::core::comm::miop::Version& version,
         const TIDorb::core::comm::miop::ListenPoint& listen_point)
                throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_UIPMC)
{
        _version = version;
        _listen_point = listen_point;
        _profile_data = NULL;
        _components_extracted = true;
}




TIDorb::core::comm::miop::ProfileMIOP::ProfileMIOP
        (const TIDorb::core::comm::miop::Version& version,
         const TIDorb::core::comm::miop::ListenPoint& listen_point,
         const TIDorb::core::iop::VectorTaggedComponent& tagged_components)
                throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_UIPMC)
{
        _version = version;
        _listen_point = listen_point;
        _components = tagged_components;
        _profile_data = NULL;
        _components_extracted = true;
}




TIDorb::core::comm::miop::ProfileMIOP::ProfileMIOP
        (const TIDorb::core::comm::miop::ProfileMIOP& other)
        throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_UIPMC)
{
	if (! other._components_extracted)
                ((ProfileMIOP*) &other)->extract_members();

        _version = other._version;
        _listen_point = other._listen_point;
        _components = other._components;
        _profile_data = NULL;
        _components_extracted = true;
}




TIDorb::core::comm::miop::ProfileMIOP::~ProfileMIOP() throw(TIDThr::SystemException)
{
        delete _profile_data;
}




TIDorb::core::comm::miop::ProfileMIOP&
TIDorb::core::comm::miop::ProfileMIOP::operator= (const TIDorb::core::comm::miop::ProfileMIOP& other)
{
	if (! other._components_extracted)
                ((ProfileMIOP*) &other)->extract_members();

        _version = other._version;
        _listen_point = other._listen_point;
        _components = other._components;
        _profile_data = NULL;
        _components_extracted = true;
	return *this;
}




void TIDorb::core::comm::miop::ProfileMIOP::extract_members()
{

  if (!_components_extracted) {

        TIDThr::Synchronized synchro(*this);
        
        if (_components_extracted) // May be changed after return of synchro
          return;


        if (_profile_data == NULL) {
                throw CORBA::INTERNAL("Empty Profile");
        }

        //profile_data->enterEncapsulation();

        /* byte_order already readed in _profile_data = input->read_encapsulation()
        CORBA::Boolean byte_order;

        _profile_data->read_boolean(byte_order);

        _profile_data->set_byte_order(byte_order);
        */

        _version.read(*_profile_data);

        _listen_point.read(*_profile_data);

        CORBA::ULong size;
        _profile_data->read_ulong(size);

        if (size < 0) {
                throw CORBA::MARSHAL("Invalid component size");
        } else {
                _components.resize(size);

                for(size_t i = 0; i < size; i++) {
                        _components[i] = TIDorb::core::iop::TaggedComponentReader::read(*_profile_data);
                }
        }

        _components_extracted = true;
  }
}




const TIDorb::core::comm::miop::Version&
TIDorb::core::comm::miop::ProfileMIOP::getVersion() const
{
        if (! _components_extracted) {
                ((ProfileMIOP*) this)->extract_members();
        }
        return _version;
}




const TIDorb::core::comm::miop::ListenPoint&
TIDorb::core::comm::miop::ProfileMIOP::getListenPoint() const
{
        if (! _components_extracted) {
                ((ProfileMIOP*) this)->extract_members();
        }
        return _listen_point;
}




const TIDorb::core::iop::VectorTaggedComponent&
TIDorb::core::comm::miop::ProfileMIOP::getTaggedComponents() const
{
        if (! _components_extracted) {
                ((ProfileMIOP*) this)->extract_members();
        }
        return _components;
}




bool TIDorb::core::comm::miop::ProfileMIOP::operator== (const TIDorb::core::iop::TaggedProfile& other) const
{
	const TIDorb::core::comm::miop::ProfileMIOP* profile =
		dynamic_cast<const TIDorb::core::comm::miop::ProfileMIOP*>(&other);

	if((profile == NULL) || (_tag != profile->_tag))
		return false;

        if (! _components_extracted) {
                ((ProfileMIOP*) this)->extract_members();
        }

	TIDorb::core::comm::miop::GroupInfo*        group_info       = NULL;
	TIDorb::core::comm::miop::GroupIIOPProfile* group_iiop       = NULL;

	TIDorb::core::comm::miop::GroupInfo*        other_group_info = NULL;
	TIDorb::core::comm::miop::GroupIIOPProfile* other_group_iiop = NULL;

	TIDorb::core::iop::VectorTaggedComponent::const_iterator iter;
	TIDorb::core::iop::VectorTaggedComponent::const_iterator end = _components.end();

	for (iter = _components.begin(); iter != end; iter++) {
		TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
		if (comp->_tag == TIDorb::core::iop::TAG_GROUP) {
			group_info = dynamic_cast<TIDorb::core::comm::miop::GroupInfo*>(comp);
		} else if (comp->_tag == TIDorb::core::iop::TAG_GROUP_IIOP) {
			group_iiop = dynamic_cast<TIDorb::core::comm::miop::GroupIIOPProfile*>(comp);
		}
	}
	end = profile->_components.end();
	for (iter = profile->_components.begin(); iter != end; iter++) {
		TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
		if (comp->_tag == TIDorb::core::iop::TAG_GROUP) {
			other_group_info = dynamic_cast<TIDorb::core::comm::miop::GroupInfo*>(comp);
		} else if (comp->_tag == TIDorb::core::iop::TAG_GROUP_IIOP) {
			other_group_iiop = dynamic_cast<TIDorb::core::comm::miop::GroupIIOPProfile*>(comp);
		}
	}


	// Compare profiles

        if (getVersion() != profile->getVersion()) {
                return false;
        }

        if (getListenPoint() != profile->getListenPoint()) {
                return false;
        }

	if (((group_info == NULL) && (other_group_info != NULL))
	    || ((group_info != NULL) && (other_group_info == NULL))) {
		return false;
	}

	if (group_info && other_group_info && !(*group_info == *other_group_info)) {
		return false;
	}

	if (((group_iiop == NULL) && (other_group_iiop != NULL))
	    || ((group_iiop != NULL) && (other_group_iiop == NULL))) {
		return false;
	}

	if (group_iiop && other_group_iiop && !(*group_iiop == *other_group_iiop)) {
		return false;
	}

        return true;
}




void TIDorb::core::comm::miop::ProfileMIOP::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
        _profile_data = input.read_encapsulation();
}




void TIDorb::core::comm::miop::ProfileMIOP::partial_write(TIDorb::core::cdr::CDROutputStream& out) const
{
        TIDThr::Synchronized synchro (*((ProfileMIOP*) this));

        // write profile data
        if (_profile_data == NULL) {
          TIDorb::core::cdr::CDROutputStream encapsulation((TIDorb::core::TIDORB*) (out.orb()));

                encapsulation.write_boolean(encapsulation.get_byte_order());

                _version.write(encapsulation);
                _listen_point.write(encapsulation);

                size_t size = _components.size();
                encapsulation.write_ulong((CORBA::ULong) size);
                for(size_t i = 0; i < size; i++) {
                        _components[i]->write(encapsulation);
                }

                ((ProfileMIOP*) this)->_profile_data = (TIDorb::core::cdr::CDRInputStream*)
                  (encapsulation.create_input_stream());
        }

        out.write_buffer(*(_profile_data->get_buffer()));
}




char* TIDorb::core::comm::miop::ProfileMIOP::toString() const
{
  ((ProfileMIOP*) this)->extract_members();

  TIDorb::util::StringBuffer buffer;

  buffer << ("Profile MIOP: {")
         << getVersion().toString() << ", "
         << getListenPoint().toString();
          
  buffer << " Components {";
  for (size_t i = 0; i <_components.size(); i++) {
    buffer << " " << _components[i]->toString();
  }
  buffer << " }}";

  return CORBA::string_dup(buffer.str().data());
}


