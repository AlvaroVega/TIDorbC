/////////////////////////////////////////////////////////////////////////
//
// File:        ActiveObjectMap.C
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

#include "TIDorb/core/poa/ActiveObjectMap.h"


TIDorb::core::poa::ActiveObjectMap::~ActiveObjectMap()
{};


void TIDorb::core::poa::ActiveObjectMap::put(const TIDorb::core::poa::OID& oid,
                                             PortableServer::Servant servant,
                                             CORBA::Boolean uniqueId)
throw (PortableServer::POA::ServantAlreadyActive, PortableServer::POA::ObjectAlreadyActive)
{

  if (_table.find(oid) != _table.end()) {
    throw PortableServer::POA::ObjectAlreadyActive();
  }


  if (uniqueId && contains(servant)) {
    throw PortableServer::POA::ServantAlreadyActive();
  }

  _table[oid] = new AOMElement(servant);

};

PortableServer::Servant TIDorb::core::poa::ActiveObjectMap::get(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive)
{
  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    throw PortableServer::POA::ObjectNotActive();
  }

  return (*index).second->_servant;
};


PortableServer::Servant 
TIDorb::core::poa::ActiveObjectMap::get_if_Active(const TIDorb::core::poa::OID& oid)
{
  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    return NULL;
  }

  return (*index).second->_servant;
};


PortableServer::Servant 
TIDorb::core::poa::ActiveObjectMap::get_and_remove(const TIDorb::core::poa::OID& oid)
  throw (PortableServer::POA::ObjectNotActive)
{
  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    throw PortableServer::POA::ObjectNotActive();
  }

  PortableServer::Servant servant = (*index).second->_servant;

  // add reference counting to the servant before erase from the AOM
  servant->_add_ref();
  _table.erase(index);

  return servant;
};

PortableServer::Servant TIDorb::core::poa::ActiveObjectMap::remove(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive)
{

  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    throw PortableServer::POA::ObjectNotActive();
  }

  PortableServer::Servant servant = (*index).second->_servant;

  _table.erase(index);

  return servant;

};

CORBA::Boolean TIDorb::core::poa::ActiveObjectMap::contains(PortableServer::Servant servant)
{
  AOMTable::iterator index = _table.begin();
  AOMTable::iterator end   = _table.end();

  while(index != end)
  {
    if((*index).second->_servant == servant)
      return true;
    index++;
  }

  return false;
};

CORBA::Boolean TIDorb::core::poa::ActiveObjectMap::isActive(const TIDorb::core::poa::OID& oid)
{
	return _table.find(oid) != _table.end();
};

CORBA::Boolean TIDorb::core::poa::ActiveObjectMap::addUser_if_Active(const TIDorb::core::poa::OID& oid)
{
  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) 
    return false;

  (*index).second->num_users++;
  return true;
};

const TIDorb::core::poa::OID& TIDorb::core::poa::ActiveObjectMap::get(PortableServer::Servant servant)
 throw (TIDorb::core::poa::ServantNotFound)
{
  AOMTable::iterator index = _table.begin();
  AOMTable::iterator end = _table.end();

  while(index != end)
  {
    if((*index).second->_servant == servant)
      return (*index).first;
    index++;
  }

  throw ServantNotFound();
};

void TIDorb::core::poa::ActiveObjectMap::addUser(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive)
{

  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    throw PortableServer::POA::ObjectNotActive();
  }

  (*index).second->num_users++;
};

CORBA::Boolean TIDorb::core::poa::ActiveObjectMap::removeUser(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive)
{

  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    throw PortableServer::POA::ObjectNotActive();
  }

  AOMElement* element = (*index).second;

  element->num_users--;

  if (element->num_users <= 0) {
    return true;  // nobody uses oid anymore
  } else {
  	return false; // someone is still using oid
  }
};


CORBA::Boolean 
TIDorb::core::poa::ActiveObjectMap::removeUser_if_Active(const TIDorb::core::poa::OID& oid)
{

  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    return false;
  }

  AOMElement* element = (*index).second;

  element->num_users--;

  if (element->num_users <= 0) {
    return true;  // nobody uses oid anymore
  } else {
    return false; // someone is still using oid
  }
};


void TIDorb::core::poa::ActiveObjectMap::destroy(const TIDorb::core::poa::OID& oid)
throw (PortableServer::POA::ObjectNotActive)
{

  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    throw PortableServer::POA::ObjectNotActive();
  }
  AOMElement* element = (*index).second;
  
  element->destroying = true;
  {
    TIDThr::Synchronized synchro(*element);
    element->notify();
  }

};

TIDorb::core::poa::ActiveObjectMap::AOMElement*
  TIDorb::core::poa::ActiveObjectMap::isDestroying(const TIDorb::core::poa::OID& oid)
{

  AOMTable::iterator index = _table.find(oid);

  if(index == _table.end()) {
    return NULL;
  }

  AOMElement* element = (*index).second;

  if (element->destroying) {
    return element;
  } else {
    return NULL;
  }
};

TIDorb::core::poa::ActiveObjectMap::AOMElement::AOMElement()
throw (TIDThr::SystemException)
{
  num_users = 1;
  _servant = NULL;
  destroying = false;
};

TIDorb::core::poa::ActiveObjectMap::AOMElement::AOMElement(PortableServer::Servant servant)
throw (TIDThr::SystemException)
{
  num_users = 1;
  _servant = servant;
  destroying = false;
  servant->_add_ref();
};

TIDorb::core::poa::ActiveObjectMap::AOMElement::AOMElement
  (const TIDorb::core::poa::ActiveObjectMap::AOMElement& other)
throw (TIDThr::SystemException)
{
  num_users = other.num_users;
  _servant = other._servant;
  _servant->_add_ref();
};

TIDorb::core::poa::ActiveObjectMap::AOMElement&
 TIDorb::core::poa::ActiveObjectMap::AOMElement::operator=(PortableServer::Servant servant)
{
  num_users = 1;

  if(_servant) {
    _servant->_remove_ref();
  }
  _servant = servant;

  destroying = false;
  servant->_add_ref();
  return *this;
};

TIDorb::core::poa::ActiveObjectMap::AOMElement&
  TIDorb::core::poa::ActiveObjectMap::AOMElement::operator=
    (const TIDorb::core::poa::ActiveObjectMap::AOMElement& other)
{
  num_users = other.num_users;
  _servant = other._servant;
  _servant->_add_ref();
  return *this;
};


TIDorb::core::poa::ActiveObjectMap::AOMElement::~AOMElement()
throw (TIDThr::SystemException)
{
  if(_servant)
	  _servant->_remove_ref();
};

bool TIDorb::core::poa::ActiveObjectMap::AOMElement::operator==(const PortableServer::Servant servant)
{
  return _servant == servant;
};

bool TIDorb::core::poa::ActiveObjectMap::AOMElement::operator==(const TIDorb::core::poa::ActiveObjectMap::AOMElement& other)
{
  return _servant == other._servant;
};


