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


void* TIDorb::core::NamedValueImpl::_impl()
{
	return this;
}

const char* TIDorb::core::NamedValueImpl::_typeid()
{
	//return CORBA::string_dup("NamedValueImpl");
	return "NamedValueImpl";
}

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
  
  if(m_release_out)
  	delete m_value;
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
  return nv;
}


CORBA::NamedValue_ptr CORBA::NamedValue::_nil()
{
  return 0;
}

void CORBA::release(CORBA::NamedValue_ptr nv)
{

}

CORBA::Boolean CORBA::is_nil(CORBA::NamedValue_ptr nv)
{
  return !nv;
}

