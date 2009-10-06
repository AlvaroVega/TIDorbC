/////////////////////////////////////////////////////////////////////////
//
// File:        ServantDelegate.C
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

#include "TIDorb/core/poa.h"

const CORBA::RepositoryIdSeq_ptr TIDorb::core::poa::ServantDelegate::ids =
TIDorb::core::poa::ServantDelegate::init_ids();

CORBA::RepositoryIdSeq_ptr TIDorb::core::poa::ServantDelegate::init_ids()
{
  CORBA::RepositoryIdSeq_ptr int_ids = new CORBA::RepositoryIdSeq();
  int_ids->length(1);
  (*int_ids)[0] = CORBA::string_dup("IDL:omg.org/CORBA/Object:1.0");

  return int_ids;

}


TIDorb::core::poa::ServantDelegate::ServantDelegate(const TIDorb::core::poa::ServantDelegate& other )
  throw (TIDThr::SystemException)
{
  _poa = PortableServer::POA::_duplicate(other._poa);
  _orb_deleg = CORBA::ORB::_duplicate(other._orb_deleg);
  _oid = other._oid;
  //PRA
  _orb = other._orb;
  //EPRA
}

/**
* Constructor.
* @param poa The POA.
* @param oid The Object Id.
*/
TIDorb::core::poa::ServantDelegate::ServantDelegate
(PortableServer::POA_ptr poa,
const PortableServer::ObjectId& oid)
 throw (TIDThr::SystemException)
{
  //PRA
  //_poa = PortableServer::POA::_duplicate(poa);
  //_orb_deleg = CORBA::ORB::_duplicate((dynamic_cast<TIDorb::core::poa::POAImpl*>(poa))->orb);
  //_oid = oid;

  TIDorb::core::TIDORB* poa_orb = (dynamic_cast<TIDorb::core::poa::POAImpl*>(poa))->orb;
  _oid = oid;
  _poa = PortableServer::POA::_duplicate(poa);
  _orb = (TIDorb::portable::ORB*) poa_orb;
  _orb_deleg = CORBA::ORB::_duplicate(poa_orb);
  //EPRA
}

TIDorb::core::poa::ServantDelegate::~ServantDelegate()
 throw (TIDThr::SystemException)
{
  CORBA::release(_orb_deleg);
  CORBA::release(_poa);
}

/**
* @param self The servant.
* @return The ORB.
*/
//PRA
const CORBA::ORB_ptr TIDorb::core::poa::ServantDelegate::orb(PortableServer::ServantBase* self)
{
  //return CORBA::ORB::_duplicate(_orb_deleg);
  return (const CORBA::ORB_ptr) _orb_deleg;
}

const TIDorb::portable::ORB* TIDorb::core::poa::ServantDelegate::orb()
{
  return (const TIDorb::portable::ORB*) _orb;
}

//EPRA

/**
* @param self The servant.
* @return Return an object reference for this servant.
*/
CORBA::Object_ptr TIDorb::core::poa::ServantDelegate::this_object(PortableServer::ServantBase* self) {
  try {
    return _poa->servant_to_reference(self);
  } catch (PortableServer::POA::WrongPolicy &e) {
    return NULL;
  } catch (PortableServer::POA::ServantNotActive &e) {
    return NULL;
  }
};

/**
* @param self The servant.
* @return This POA.
*/
PortableServer::POA_ptr TIDorb::core::poa::ServantDelegate::poa(PortableServer::ServantBase* self)
{
  return PortableServer::POA::_duplicate(_poa);
};

/**
* @param self The servant.
* @return Return the Object Id.
*/
const PortableServer::ObjectId&
TIDorb::core::poa::ServantDelegate::object_id(PortableServer::ServantBase* self)
{
  return _oid;
};

/**
* @param self The servant.
* @return Return the default POA.
*/

PortableServer::POA_ptr
TIDorb::core::poa::ServantDelegate::default_POA(PortableServer::ServantBase* self)
{
  if(_poa == NULL)
  {        
    return TIDorb::core::poa::POAImpl::default_POA(); 
  }
  
  return PortableServer::POA::_duplicate(_poa);
};

/**
* @param self The servant.
* @param repository_id The Repository Id.
* @return Return true if the servant is an object with this Repository Id.
*/
CORBA::Boolean TIDorb::core::poa::ServantDelegate::is_a
(PortableServer::ServantBase* self,
const char* repository_id) {

  const CORBA::RepositoryIdSeq_ptr reps = self->_all_interfaces(_poa,_oid);

  for (size_t i=0; i<reps->length(); i++) {
//MLG  	
    if (repository_id && (strcmp(repository_id,(*reps)[i])==0))
//EMLG    
    return true;
  }
  
  return false;
};

CORBA::Boolean TIDorb::core::poa::ServantDelegate::non_existent(PortableServer::ServantBase* self)
{
  return false;
};

CORBA::InterfaceDef_ptr TIDorb::core::poa::ServantDelegate::get_interface(PortableServer::ServantBase* self)
{
  throw CORBA::NO_IMPLEMENT();
};

/**
* Sets object Id (useful for default servants).
* @param oid The Object Id.
*/
void TIDorb::core::poa::ServantDelegate::setObjectId(const PortableServer::ObjectId& oid)
{
  _oid = oid;
};

const CORBA::RepositoryIdSeq_ptr
TIDorb::core::poa::ServantDelegate::all_interfaces(PortableServer::ServantBase* self,
PortableServer::POA_ptr poa,
const PortableServer::ObjectId& objectId)
{
  return ids;
}

