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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

