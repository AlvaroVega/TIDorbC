/////////////////////////////////////////////////////////////////////////
//
// File:        SystemExceptionHolder.C
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

#include "CORBA.h"
#include "TIDorb/core/util.h"



/*
 Copy Constructor
*/  
TIDorb::core::util::SystemExceptionHolder::SystemExceptionHolder(const CORBA::SystemException& ex)
{ 
  	
  	m_value =  TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(ex);
}

/*
 * Constructor
 */  
TIDorb::core::util::SystemExceptionHolder::SystemExceptionHolder(const char* rep_id)
{ 
  m_value = TIDorb::core::util::exceptions::SystemExceptionEncoder::copy(rep_id);
}

/* 
* Operacion de escritura en un OutputStream
*/ 
void TIDorb::core::util::SystemExceptionHolder::_write(::TIDorb::portable::OutputStream& outs) const	
{
  if(!m_value) {
    throw CORBA::MARSHAL();
  }
  
  TIDorb::core::util::exceptions::SystemExceptionEncoder::write(outs, *m_value);
}

/*
Operacion de lectura desde un InputStream
*/
void TIDorb::core::util::SystemExceptionHolder::_read(TIDorb::portable::InputStream& ins)
{
  delete m_value;
  m_value = TIDorb::core::util::exceptions::SystemExceptionEncoder::read(ins);
}

/*
Accesor del miembro type
*/

CORBA::TypeCode_ptr TIDorb::core::util::SystemExceptionHolder::_type() const 
{
    if(!m_value)
      throw CORBA::BAD_OPERATION();
      	
  return CORBA::TypeCode::_duplicate(m_value->type()); 
}
