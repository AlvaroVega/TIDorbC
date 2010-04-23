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
    
