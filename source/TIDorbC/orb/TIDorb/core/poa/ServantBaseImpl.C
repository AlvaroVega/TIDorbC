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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
 
