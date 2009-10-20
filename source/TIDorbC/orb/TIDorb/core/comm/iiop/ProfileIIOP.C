//////////////////////////////////////////////////////////////////////////////////
//
// File:        ProfileIIOP.C
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

#include "TIDorb/core/comm/iiop/ProfileIIOP.h"
#include <math.h>




TIDorb::core::comm::iiop::ProfileIIOP::ProfileIIOP() throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_INTERNET_IOP)
{
        _object_key = NULL;
        _profile_data = NULL;
        _policies = NULL;
        _components_extracted = false;
}




TIDorb::core::comm::iiop::ProfileIIOP::ProfileIIOP
        (const TIDorb::core::comm::iiop::Version& version,
         const TIDorb::core::comm::iiop::ListenPoint& listen_point) throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_INTERNET_IOP)
{
        _version = version;
	_listen_points.push_back(listen_point);
        //_listen_point = listen_point;
        _object_key = NULL;
        _profile_data = NULL;
        _policies = NULL;
        _components_extracted = true;
}




TIDorb::core::comm::iiop::ProfileIIOP::ProfileIIOP
        (const TIDorb::core::comm::iiop::Version& version,
         const TIDorb::core::comm::iiop::ListenPoint& listen_point,
         TIDorb::core::comm::iiop::ObjectKey* object_key,
         const TIDorb::core::iop::VectorTaggedComponent& tagged_components)
        throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_INTERNET_IOP)
{
        _version = version;
        _listen_points.push_back(listen_point);
        //_listen_point = listen_point;
        _object_key = object_key;
        _components = tagged_components;
        _profile_data = NULL;
        _components_extracted = true;
}




TIDorb::core::comm::iiop::ProfileIIOP::ProfileIIOP
        (const TIDorb::core::comm::iiop::ProfileIIOP& other)
        throw (TIDThr::SystemException)
        : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_INTERNET_IOP)
{
	if (! other._components_extracted)
                ((ProfileIIOP*) &other)->extract_members();

        _version = other._version;
        _listen_points = other._listen_points;
        //_listen_point = other._listen_point;
        _object_key = other._object_key;
        _components = other._components;
        _profile_data = NULL;
        _components_extracted = true;
}




TIDorb::core::comm::iiop::ProfileIIOP::~ProfileIIOP() throw(TIDThr::SystemException)
{
        delete _profile_data;
}




TIDorb::core::comm::iiop::ProfileIIOP&
TIDorb::core::comm::iiop::ProfileIIOP::operator= (const TIDorb::core::comm::iiop::ProfileIIOP& other)
{
	if (! other._components_extracted)
                ((ProfileIIOP*) &other)->extract_members();

        _version = other._version;
        _listen_points = other._listen_points;
        //_listen_point = other._listen_point;
        _object_key = other._object_key;
        _components = other._components;
        _profile_data = NULL;
        _components_extracted = true;
	return *this;
}




void TIDorb::core::comm::iiop::ProfileIIOP::extract_members()
{

  if(!_components_extracted) {
    
        TIDThr::Synchronized synchro(*this);
    
        if(_components_extracted) // May be changed after return of synchro
          return;

        if(_profile_data == NULL)
                throw CORBA::INTERNAL("Empty Profile");

        _version.read(*_profile_data);

        //_listen_point.read(*_profile_data);
        TIDorb::core::comm::iiop::ListenPoint point;
        point.read(*_profile_data);
        _listen_points.clear();
        _listen_points.push_back(point);

        _object_key = new ObjectKey();
        _object_key->read(*_profile_data);

        if(_version > Version::VERSION_1_0) {
                CORBA::ULong size;
                _profile_data->read_ulong(size);

                if (size < 0) {
                        throw CORBA::MARSHAL("Invalid component size");
                } else {
                        _components.resize(size);

                        for(size_t i = 0; i < size; i++)
                        {
                              //_components[i] = TIDorb::core::iop::TaggedComponentReader::read(*_profile_data);

                              TIDorb::core::iop::TaggedComponent* comp;
                              comp = TIDorb::core::iop::TaggedComponentReader::read(*_profile_data);
                              _components[i] = comp;
                              
                              if (comp->_tag == TIDorb::core::iop::TAG_POLICIES) {
                                TIDorb::core::messaging::PoliciesComponent* policies_component =
                                  dynamic_cast<TIDorb::core::messaging::PoliciesComponent*>(comp);
                                _policies = policies_component->getPolicies();
                              }

                              TIDorb::core::comm::FT::AlternateIIOPAddress* addr;
                              addr = dynamic_cast<TIDorb::core::comm::FT::AlternateIIOPAddress*>(comp);
			      if (addr) _listen_points.push_back(addr->_listen_point);
                        }
                }
        }

        _components_extracted = true;
  }
}




const TIDorb::core::comm::iiop::Version& TIDorb::core::comm::iiop::ProfileIIOP::getVersion() const
{
        if(! _components_extracted )
                ((ProfileIIOP*) this)->extract_members();
        return _version;
}




const TIDorb::core::comm::iiop::ListenPoint& TIDorb::core::comm::iiop::ProfileIIOP::getListenPoint() const
{
        if(! _components_extracted )
                ((ProfileIIOP*) this)->extract_members();
        //return _listen_point;
        return _listen_points[0];
}




const TIDorb::core::comm::iiop::ListenPoint& TIDorb::core::comm::iiop::ProfileIIOP::getAnyListenPoint() const
{
        if(! _components_extracted )
                ((ProfileIIOP*) this)->extract_members();

        size_t upper_index = _listen_points.size() - 1;
        size_t position    = (size_t) rint(upper_index * drand48());
        return _listen_points[position];
}




TIDorb::core::comm::iiop::ObjectKey* TIDorb::core::comm::iiop::ProfileIIOP::getObjectKey() const
{
        if(! _components_extracted )
                ((ProfileIIOP*) this)->extract_members();
        return _object_key;
}




const TIDorb::core::iop::VectorTaggedComponent&
  TIDorb::core::comm::iiop::ProfileIIOP::getTaggedComponents() const
{
        if(! _components_extracted )
                ((ProfileIIOP*) this)->extract_members();
        return _components;
}




bool TIDorb::core::comm::iiop::ProfileIIOP::operator== (const TIDorb::core::iop::TaggedProfile& other) const
{
	const TIDorb::core::comm::iiop::ProfileIIOP* profile =
		dynamic_cast<const TIDorb::core::comm::iiop::ProfileIIOP*>(&other);

	if((profile == NULL) || (_tag != profile->_tag))
		return false;

        if(! _components_extracted )
                ((ProfileIIOP*)this)->extract_members();

        if(getVersion() != profile->getVersion())
                return false;

        if(getListenPoint() != profile->getListenPoint())
                return false;

        return (*getObjectKey()) == (*(profile->getObjectKey()));
}




void TIDorb::core::comm::iiop::ProfileIIOP::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
        _profile_data = input.read_encapsulation();
}




void TIDorb::core::comm::iiop::ProfileIIOP::partial_write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro (*((ProfileIIOP*) this));

/* pra@tid.es - MIOP extensions
  // write tag
  out.write_ulong(_tag); //tag variable heredada de TaggedProfile
*/

  // write profile data

  if(_profile_data == NULL)
  {
    //TIDorb::core::cdr::CDROutputStream encapsulation(dynamic_cast <TIDorb::core::TIDORB*> (out.orb()));
    TIDorb::core::cdr::CDROutputStream encapsulation((TIDorb::core::TIDORB*) (out.orb()));

    encapsulation.write_boolean(encapsulation.get_byte_order());

    _version.write(encapsulation);
    _listen_points[0].write(encapsulation);
    _object_key->write(encapsulation);

    if(_version > Version::VERSION_1_0) {
      size_t size = _components.size();
      encapsulation.write_ulong((CORBA::ULong) size);
      for(size_t i = 0; i < size; i++)
        _components[i]->write(encapsulation);
    }

    ((ProfileIIOP*) this)->_profile_data =
      (TIDorb::core::cdr::CDRInputStream*)(encapsulation.create_input_stream());
    //dynamic_cast <TIDorb::core::cdr::CDRInputStream*>(encapsulation.create_input_stream());
  }

  out.write_buffer(*(_profile_data->get_buffer()));
}




char* TIDorb::core::comm::iiop::ProfileIIOP::toString() const
{
  ((ProfileIIOP*) this)->extract_members();

  TIDorb::util::StringBuffer buffer;
  TIDorb::util::StringBuffer buffer_comp;

  CORBA::Boolean from_tidorb = false;
  TIDorb::core::iop::ORBComponent* orb_comp = NULL;

  for (size_t i = 0; i <_components.size(); i++) {
    switch (_components[i]->_tag)
    {
      case TIDorb::core::iop::TAG_ORB_TYPE:
        orb_comp = dynamic_cast<TIDorb::core::iop::ORBComponent*>(((ProfileIIOP*) this)->_components[i].getT());
        if (orb_comp->orb_type == TIDorb::core::ConfORB::ORB_TYPE->orb_type) {
          from_tidorb = true;
        }
        break;

      default:
        buffer_comp << " " << _components[i]->toString();
    }
  }

  buffer << ("Profile IIOP: {")
         << getVersion().toString() << ", "
         << getListenPoint().toString() << " ";

  if (from_tidorb)
    buffer << getObjectKey()->get_key()->toString();
  else
    buffer << "ObjectKey[OPAQUE]";

  buffer << " Components {" << buffer_comp.str() << " }";
  buffer << " }";

  return CORBA::string_dup(buffer.str().data());
}


TIDorb::core::PolicyContext* TIDorb::core::comm::iiop::ProfileIIOP::getPolicies() const
{
        if(! _components_extracted )
                ((ProfileIIOP*) this)->extract_members();
        return _policies;
}
