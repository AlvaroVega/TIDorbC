/////////////////////////////////////////////////////////////////////////
//
// File:        ServantBaseImpl.C
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

#include "CORBA.h"
#include "PortableServer.h"
#include "TIDorb/core/poa.h"

PortableServer::ServantBase::ServantBase()
{
  _delegate = NULL;
}


PortableServer::ServantBase::ServantBase(const PortableServer::ServantBase& other)
{
  if(other._delegate != NULL)
  {
    //jagd
    //_delegate = new TIDorb::core::poa::ServantDelegate
    //  (dynamic_cast< TIDorb::core::poa::ServantDelegate& > (*(other._delegate)));
    _delegate = new TIDorb::core::poa::ServantDelegate
      ( *(TIDorb::core::poa::ServantDelegate *)(other._delegate));
  }
}

PortableServer::ServantBase& 
  PortableServer::ServantBase::operator=(const PortableServer::ServantBase& other)
{

  if(other._delegate != NULL)
  {
    delete _delegate;
    
    //jagd 
    //_delegate = new TIDorb::core::poa::ServantDelegate
    //  (dynamic_cast< TIDorb::core::poa::ServantDelegate& > (*(other._delegate)));
    _delegate = new TIDorb::core::poa::ServantDelegate
      ( *(TIDorb::core::poa::ServantDelegate *)(other._delegate));
  }
  
  return *this;
}

PortableServer::ServantBase::~ServantBase()
{
  delete _delegate;
}

CORBA::InterfaceDef_ptr PortableServer::ServantBase::_get_interface()
{
  if(_delegate == NULL)
	  throw CORBA::NO_IMPLEMENT();
	
	return _delegate->get_interface(this);
}
  	
CORBA::Boolean PortableServer::ServantBase::_is_a(const char* logical_type_id)
{
  if(_delegate == NULL)
	  throw CORBA::NO_IMPLEMENT();
	
	return _delegate->is_a(this, logical_type_id);
}
  	
CORBA::Boolean PortableServer::ServantBase::_non_existent()
{
  if(_delegate == NULL)
	  throw CORBA::NO_IMPLEMENT();
	
	return _delegate->non_existent(this);
}

PortableServer::POA_ptr PortableServer::ServantBase::_default_POA()
{
  if(_delegate == NULL)
   return TIDorb::core::poa::POAImpl::default_POA();
  
  return _delegate->default_POA(this);
};


TIDorb::portable::ServantDelegate* PortableServer::ServantBase::get_delegate()
{
  return _delegate;
}

void PortableServer::ServantBase::set_delegate(TIDorb::portable::ServantDelegate* delegate)
{
  _delegate = delegate;
}

const CORBA::RepositoryIdSeq_ptr 
      PortableServer::ServantBase::_all_interfaces(PortableServer::POA_ptr poa, 
                                                   const PortableServer::ObjectId& objectId)
{

 if(_delegate == NULL)
   throw CORBA::NO_IMPLEMENT();
  
  return _delegate->all_interfaces(this, poa, objectId);  
} 
 
