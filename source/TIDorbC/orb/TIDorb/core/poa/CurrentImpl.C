/////////////////////////////////////////////////////////////////////////
//
// File:        CurrentImpl.C
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

#include "TIDorb/core/poa/CurrentImpl.h"

TIDorb::core::poa::CurrentInfo* TIDorb::core::poa::CurrentImpl::getCurrentInfo()
  throw (PortableServer::Current::NoContext)
{
  TIDThr::Thread* th = TIDThr::Thread::currentThread();
				
  //if (th instanceof ExecThread) { // the current is in a servant execthread
  TIDorb::core::poa::ExecThread* exec = dynamic_cast<TIDorb::core::poa::ExecThread*>(th);
	
  if(exec) {	
    //TIDorb::core::poa::ExecThread* exec = (TIDorb::core::poa::ExecThread*) th;
			
    TIDorb::core::poa::CurrentInfo* info = exec->getCurrentInfo();

    if (info != NULL)
      return info;
  }

  // error executing a local request or we are in the orb main thread
		
  throw PortableServer::Current::NoContext();
};
	
TIDorb::core::poa::CurrentImpl::CurrentImpl(TIDorb::core::TIDORB* orb)
{
  _orb = orb;
  //PRA
  orb->_add_ref();
  //EPRA
  this->TIDThr::RefCounter::_add_ref();
};

TIDorb::core::poa::CurrentImpl::~CurrentImpl()
  throw(TIDThr::SystemException)
{
  //PRA
  //delete _orb;
  CORBA::release(_orb);
  //EPRA	
};
	
PortableServer::POA* TIDorb::core::poa::CurrentImpl::get_POA() 
  throw (PortableServer::Current::NoContext)
{
  return PortableServer::POA::_duplicate(getCurrentInfo()->_current_poa);
};
		
PortableServer::ObjectId_ptr TIDorb::core::poa::CurrentImpl::get_object_id() 
  throw (PortableServer::Current::NoContext)
{
  return getCurrentInfo()->_current_oid.toByteArray();
};

CORBA::Object_ptr TIDorb::core::poa::CurrentImpl::get_reference()
  throw (PortableServer::Current::NoContext)
{
  PortableServer::POA_var poa = get_POA();
  PortableServer::ObjectId_var oid = get_object_id();
  
  return poa->create_reference_with_id(oid, (const ::CORBA::RepositoryId) "IDL:omg.org/CORBA/Object:1.0");
}
		

PortableServer::Servant TIDorb::core::poa::CurrentImpl::get_servant() 
  throw (PortableServer::Current::NoContext)
{
  return getCurrentInfo()->_servant;
};

CORBA::Boolean TIDorb::core::poa::CurrentImpl::in_context()
{
  TIDThr::Thread* th = TIDThr::Thread::currentThread();
				
  //if (th instanceof ExecThread) { // the current is in a servant execthread
  TIDorb::core::poa::ExecThread* exec = dynamic_cast<TIDorb::core::poa::ExecThread*>(th);
	
  if(exec) {
		
    //ExecThread exec = (ExecThread) th;
			
    TIDorb::core::poa::CurrentInfo* info = exec->getCurrentInfo();

    return (info != NULL);	
  }
    
  return false;
};
