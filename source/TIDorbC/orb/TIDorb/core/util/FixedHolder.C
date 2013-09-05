/////////////////////////////////////////////////////////////////////////
//
// File:        FixedHolder.C
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
#include "TIDorb/core/util/FixedHolder.h"


/**
 * Particular Holder for FixedType.
 * @autor MACP of CPP Version
 * @version 0.1
 */
		
/* 
Constructor a partir de un typecode.
*/

TIDorb::core::util::FixedHolder::FixedHolder(CORBA::TypeCode_ptr t_code)
{
  	fixed_type = CORBA::TypeCode::_duplicate(t_code);
  	value = "0.0";
}

/*
Constructor con TypeCode y CORBA::Fixed
*/  
TIDorb::core::util::FixedHolder::FixedHolder(CORBA::TypeCode_ptr t_code, const CORBA::Fixed& f)
{
  	fixed_type = CORBA::TypeCode::_duplicate(t_code);
  	value = f;
}

/* 
Operacion de escritura en un OutputStream
*/ 
void TIDorb::core::util::FixedHolder::_write(::TIDorb::portable::OutputStream& outs) const	
{
  outs.write_fixed(value, fixed_type->fixed_digits(), fixed_type->fixed_scale());
}

/*
Operacion de lectura desde un InputStream
*/
void TIDorb::core::util::FixedHolder::_read(TIDorb::portable::InputStream& ins)
{
  ins.read_fixed(value, fixed_type->fixed_digits(), fixed_type->fixed_scale());
}

/*
Accesor del miembro type
*/

CORBA::TypeCode_ptr TIDorb::core::util::FixedHolder::_type() const 
{	
  return CORBA::TypeCode::_duplicate(fixed_type); 
}



 
