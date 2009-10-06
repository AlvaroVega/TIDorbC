/////////////////////////////////////////////////////////////////////////
//
// File:        ExceptionListImpl.C
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

TIDorb::core::ExceptionListImpl::ExceptionListImpl()  throw (TIDThr::SystemException)
{
  _count(1);
}

TIDorb::core::ExceptionListImpl::~ExceptionListImpl()  throw (TIDThr::SystemException)
{
  ItemList::iterator i;
  ItemList::const_iterator end = m_list.end();
  
  for(i = m_list.begin(); i != end; i++)
    CORBA::release(*i);
}

CORBA::ULong TIDorb::core::ExceptionListImpl::count() const
{
  return m_list.size();
}

void TIDorb::core::ExceptionListImpl::add(CORBA::TypeCode_ptr tc)
{
  if(!tc)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
    
  m_list.push_back(CORBA::TypeCode::_duplicate(tc));
}

void TIDorb::core::ExceptionListImpl::add_consume(CORBA::TypeCode_ptr tc)
{
  if(!tc)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
  
  m_list.push_back(tc);
}

CORBA::TypeCode_ptr TIDorb::core::ExceptionListImpl::item(CORBA::ULong index)
{
  if(index >= m_list.size())
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
    
  return CORBA::TypeCode::_duplicate(m_list[index]);
}

void TIDorb::core::ExceptionListImpl::remove(CORBA::ULong index)
{
  TIDThr::Synchronized sync(*this);
  
  CORBA::ULong size = m_list.size();

  if(index >= size)
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);
    
  CORBA::release(m_list[index]);
  
  for(CORBA::ULong i = index; i < size; i++) 
    m_list[i] = m_list[i+1];
  
  m_list.resize(size - 1);
}

CORBA::ExceptionList_ptr CORBA::ExceptionList::_duplicate(CORBA::ExceptionList_ptr list)
{
  try {
    
    //jagd 
    //TIDorb::core::ExceptionListImpl* list_impl = dynamic_cast<TIDorb::core::ExceptionListImpl*> (list);
    TIDorb::core::ExceptionListImpl* list_impl = (TIDorb::core::ExceptionListImpl*) (list);
  
    if(list_impl)
      list_impl->_add_ref();
  
    return list;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}

CORBA::ExceptionList_ptr CORBA::ExceptionList::_nil()
{
  return 0;
}

void CORBA::release(CORBA::ExceptionList_ptr list)
{
  try {
    //jagd 
    //TIDorb::core::ExceptionListImpl* list_impl = dynamic_cast<TIDorb::core::ExceptionListImpl*> (list);
    TIDorb::core::ExceptionListImpl* list_impl = (TIDorb::core::ExceptionListImpl*) (list);
  
    if(list_impl)
      list_impl->_remove_ref();

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
}

CORBA::Boolean CORBA::is_nil(CORBA::ExceptionList_ptr list)
{
  return !list;
}
    
