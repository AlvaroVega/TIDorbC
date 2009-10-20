/////////////////////////////////////////////////////////////////////////
//
// File:        NamedValueImpl.C
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


//MLG
void* TIDorb::core::NamedValueImpl::_impl()
{
	return this;
}

const char* TIDorb::core::NamedValueImpl::_typeid()
{
	//return CORBA::string_dup("NamedValueImpl");
	return "NamedValueImpl";
}
//EMLG

TIDorb::core::NamedValueImpl::NamedValueImpl(char* name,
                                             CORBA::Any* value, CORBA::Flags flags)  
  throw (TIDThr::SystemException)
{
  m_name = name;
  m_value = value;
  m_flags = flags;
  m_release_out = true;
  m_consume_name = false;
}

TIDorb::core::NamedValueImpl::NamedValueImpl(char* name, CORBA::Boolean consume_name,
                                             CORBA::Any* value, CORBA::Flags flags)  
  throw (TIDThr::SystemException)
{
  m_name = name;
  m_value = value;
  m_flags = flags;
  m_release_out = true;
  m_consume_name = consume_name;
}

TIDorb::core::NamedValueImpl::~NamedValueImpl()  throw (TIDThr::SystemException)
{
  if (m_consume_name)
    CORBA::string_free(m_name);
  
// jagd 3
//  CORBA::string_free(m_name);
//MLG
//TODO revisar condicion de borrado  
//FRAN
//  if(!((m_flags == CORBA::ARG_OUT) && m_release_out ))
  if(m_release_out) //jagd 4
  	delete m_value;
//FRAN
//EMLG
}

const char* TIDorb::core::NamedValueImpl::name() const
{
  return m_name;
}
CORBA::Any* TIDorb::core::NamedValueImpl::value() const
{
  return m_value;
}   
CORBA::Flags TIDorb::core::NamedValueImpl::flags() const
{
  return m_flags;
}

void TIDorb::core::NamedValueImpl::release_out(bool value)
{
  m_release_out = value;
}

CORBA::NamedValue_ptr CORBA::NamedValue::_duplicate(CORBA::NamedValue_ptr nv)
{
 /* 
  try {
    
    //TIDorb::core::NamedValueImpl* nv_impl = dynamic_cast<TIDorb::core::NamedValueImpl*> (nv);
    TIDorb::core::NamedValueImpl* nv_impl = NULL;
   
    //jagd 
    	//nv_impl = (TIDorb::core::NamedValueImpl*)nv->_impl();
    	nv_impl = (TIDorb::core::NamedValueImpl*)nv;

    if(nv_impl)
      nv_impl->_add_ref();
    
    return nv;
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
 */ 
  return nv;
}


CORBA::NamedValue_ptr CORBA::NamedValue::_nil()
{
  return 0;
}

void CORBA::release(CORBA::NamedValue_ptr nv)
{
/*  
  try {
    
    //TIDorb::core::NamedValueImpl* nv_impl = dynamic_cast<TIDorb::core::NamedValueImpl*> (nv);
    TIDorb::core::NamedValueImpl* nv_impl = NULL;
    //jagd
	//nv_impl = (TIDorb::core::NamedValueImpl*)nv->_impl();
	nv_impl = (TIDorb::core::NamedValueImpl*)nv;
    
    if(nv_impl)
      nv_impl->_remove_ref();
    
  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL();
  }
 */ 
}

CORBA::Boolean CORBA::is_nil(CORBA::NamedValue_ptr nv)
{
  return !nv;
}

