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
  orb->_add_ref();
  this->TIDThr::RefCounter::_add_ref();
};

TIDorb::core::poa::CurrentImpl::~CurrentImpl()
  throw(TIDThr::SystemException)
{
  CORBA::release(_orb);
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
