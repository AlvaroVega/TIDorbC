/////////////////////////////////////////////////////////////////////////
//
// File:        ContextListImpl.C
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

TIDorb::core::ContextListImpl::ContextListImpl()   throw (TIDThr::SystemException)
{
  _count(1);
}

TIDorb::core::ContextListImpl::~ContextListImpl()  throw (TIDThr::SystemException)
{
  ItemList::iterator i;
  ItemList::const_iterator end = m_list.end();
  
  for(i = m_list.begin(); i != end; i++)
    CORBA::string_free(*i);
}

CORBA::ULong TIDorb::core::ContextListImpl::count() const
{
  return m_list.size();
}

void TIDorb::core::ContextListImpl::add(const char* ctxt)
{
  if(!ctxt)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
    
  m_list.push_back(CORBA::string_dup(ctxt));
}

void TIDorb::core::ContextListImpl::add_consume(char* ctxt)
{
  if(!ctxt)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
  
  m_list.push_back(ctxt);
}

const char* TIDorb::core::ContextListImpl::item(CORBA::ULong index)
{
  if(index >= m_list.size())
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
    
  return m_list[index];
}

void TIDorb::core::ContextListImpl::remove(CORBA::ULong index)
{
  TIDThr::Synchronized sync(*this);
  
  CORBA::ULong size = m_list.size();

  if(index >= size)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
    
  CORBA::string_free(m_list[index]);
  
  for(CORBA::ULong i = index; i < size; i++) 
    m_list[i] = m_list[i+1];
  
  m_list.resize(size - 1);
}

CORBA::ContextList_ptr CORBA::ContextList::_duplicate(CORBA::ContextList_ptr list)
{
  try {
   
    //jagd 
    //TIDorb::core::ContextListImpl* list_impl = dynamic_cast<TIDorb::core::ContextListImpl*> (list);
    TIDorb::core::ContextListImpl* list_impl = (TIDorb::core::ContextListImpl*) (list);
  
    if(list_impl)
      list_impl->_add_ref();
  
    return list;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}

CORBA::ContextList_ptr CORBA::ContextList::_nil()
{
  return 0;
}

void CORBA::release(CORBA::ContextList_ptr list)
{
  try {
   
    //jagd 
    //TIDorb::core::ContextListImpl* list_impl = dynamic_cast<TIDorb::core::ContextListImpl*> (list);
    TIDorb::core::ContextListImpl* list_impl = (TIDorb::core::ContextListImpl*) (list);
  
    if(list_impl)
      list_impl->_remove_ref();

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}

CORBA::Boolean CORBA::is_nil(CORBA::ContextList_ptr list)
{
  return !list;
}
    
