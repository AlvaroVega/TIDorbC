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
