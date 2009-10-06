/////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeMarshaler.C
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

#include "TIDorb/core/typecode.h"


bool TIDorb::core::typecode::TypeCodeMarshaler::skip_value_array(CORBA::TypeCode_ptr type, 
                                 TIDorb::core::cdr::CDRInputStream& input,
                                 CORBA::ULong n)
{
	switch (type->kind()) {
  	case CORBA::tk_short: 
  		input.skip_short_array(n);
  		break;
  	case CORBA::tk_long: 
   		input.skip_long_array(n);
			break;
  	case CORBA::tk_longlong:
   		input.skip_longlong_array(n);
			break;
  	case CORBA::tk_ushort: 
   		input.skip_ushort_array(n);
			break;
  	case CORBA::tk_ulong: 
   		input.skip_ulong_array(n);	
    	break;
   	case CORBA::tk_ulonglong:                        
    	input.skip_ulonglong_array(n);
			break;
   	case CORBA::tk_float: 
    	input.skip_float_array(n);
			break;
   	case CORBA::tk_double: 
    	input.skip_double_array(n);
			break;
   	case CORBA::tk_boolean: 
    	input.skip_boolean_array(n);
			break;
   	case CORBA::tk_char: 
    	input.skip_char_array(n);
			break;
   	case CORBA::tk_wchar: 
    	input.skip_wchar_array(n);
			break;
   	case CORBA::tk_octet: 
    	input.skip_octet_array(n);
			break;
    default:
    {
      //TypeCodeImpl* tc_impl = dynamic_cast<TypeCodeImpl*>(type);
      //jagd 
      //TypeCodeImpl* tc_impl = (TypeCodeImpl*)type->_impl();
      TypeCodeImpl* tc_impl = (TypeCodeImpl*)type;
    	for (CORBA::ULong i = 0; i < n; i++) 
    		if(!tc_impl->skip_value(input))
    			return false;
    }
  }
  
  return true;
}  

