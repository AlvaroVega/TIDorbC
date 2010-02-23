/////////////////////////////////////////////////////////////////////////
//
// File:        EnviromentImpl.C
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

#include "TIDorb/core.h"

TIDorb::core::EnvironmentImpl::EnvironmentImpl()  throw (TIDThr::SystemException)
  : m_exception(NULL) 
{
  //jagd 3 _count(1);
}

TIDorb::core::EnvironmentImpl::~EnvironmentImpl()  throw (TIDThr::SystemException)
{
  delete m_exception;
}

void TIDorb::core::EnvironmentImpl::exception(CORBA::Exception* e)
{
  try {
    //jagd TIDThr::Synchronized sync(*this);
    
    if(m_exception)
      delete m_exception;
      
    m_exception = e;
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }  
}

void TIDorb::core::EnvironmentImpl::clear()
{
  try {  
   //jagd TIDThr::Synchronized sync(*this);
    
    if(m_exception)
      delete m_exception;
      
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::UNKNOWN(0,CORBA::COMPLETED_NO);
  }
}

CORBA::Exception* TIDorb::core::EnvironmentImpl::exception() const
{
  return m_exception;
}

CORBA::Environment_ptr CORBA::Environment::_duplicate(CORBA::Environment_ptr ev)
{
  /* jagd 3
  try {
   
    //jagd 
    //TIDorb::core::EnvironmentImpl* env = dynamic_cast<TIDorb::core::EnvironmentImpl*> (ev);
    TIDorb::core::EnvironmentImpl* env = (TIDorb::core::EnvironmentImpl*) (ev);
  
    if(env)
      env->_add_ref();
  
    return ev;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  */
  return ev;
  
}

CORBA::Environment_ptr CORBA::Environment::_nil()
{
  return 0;
}

void CORBA::release(CORBA::Environment_ptr ev)
{
  /*jagd 3
  try {
   
    //jagd 
    //TIDorb::core::EnvironmentImpl* env = dynamic_cast<TIDorb::core::EnvironmentImpl*> (ev);
    TIDorb::core::EnvironmentImpl* env = (TIDorb::core::EnvironmentImpl*) (ev);
  
    if(env)
      env->_remove_ref();

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  */
}

CORBA::Boolean CORBA::is_nil(CORBA::Environment_ptr ev)
{
  return !ev;
}

