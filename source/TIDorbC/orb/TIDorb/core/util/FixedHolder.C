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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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



 
