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
      TypeCodeImpl* tc_impl = (TypeCodeImpl*)type;
    	for (CORBA::ULong i = 0; i < n; i++) 
    		if(!tc_impl->skip_value(input))
    			return false;
    }
  }
  
  return true;
}  

