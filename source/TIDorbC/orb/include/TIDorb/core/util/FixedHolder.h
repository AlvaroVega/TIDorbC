/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        FixedHolder.h
// Description: 
//
// Rel:         01.00
// Created:     
// Author:      
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "CORBA.h" 

#ifndef _TIDORB_CORE_UTIL_FIXEDHOLDER_H_                                                  
#define _TIDORB_CORE_UTIL_FIXEDHOLDER_H_     

#include "TIDorb/portable/Streamable.h"

namespace TIDorb {
namespace core {
namespace util {
	
class FixedHolder: public ::TIDorb::portable::Streamable {

	public: // members
		CORBA::TypeCode_ptr fixed_type;
		CORBA::Fixed value;
	
	public: //methods
		FixedHolder(CORBA::TypeCode_ptr t_code);
		FixedHolder(CORBA::TypeCode_ptr t_code,const CORBA::Fixed& f);
		void _write(::TIDorb::portable::OutputStream& outs) const;		
		void _read(::TIDorb::portable::InputStream& ins); 
		::CORBA::TypeCode_ptr _type() const;
		

}; // END OF _FixedHolder		

} // end of namespace util
} // end of namespace core
} // end of namespace TIDorb

#endif
