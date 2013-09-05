/////////////////////////////////////////////////////////////////////////
//
// File:        CurrentInfo.C
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

#include "TIDorb/core/poa/CurrentInfo.h"

TIDorb::core::poa::CurrentInfo::CurrentInfo(TIDorb::core::poa::POAImpl* current_poa, 
                                            const TIDorb::core::poa::OID& current_oid)
{
	_current_poa = current_poa;
	_current_poa->_add_ref();
	_current_oid = current_oid;
  _request_in_poa = false;
	_servant = NULL;
};

TIDorb::core::poa::CurrentInfo::~CurrentInfo() {
  CORBA::release(_current_poa);
  if(_servant)
    _servant->_remove_ref();
};
  
CORBA::Boolean TIDorb::core::poa::CurrentInfo::is_request_in_POA()
{
	return _request_in_poa;
};
  
void TIDorb::core::poa::CurrentInfo::set_request_in_POA(CORBA::Boolean val)
{
	_request_in_poa = val;
};
  
void TIDorb::core::poa::CurrentInfo::setServant(PortableServer::ServantBase* servant) {
  if(_servant)
    _servant->_remove_ref();

  _servant = servant;

  if(_servant)
    _servant->_add_ref();
}
