/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/CDR.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#ifndef _TIDorb_core_cdr_CDR_H_
#define _TIDorb_core_cdr_CDR_H_

#include "CORBA.h"

#undef BIG_ENDIAN
#undef LITTLE_ENDIAN


namespace TIDorb {
namespace core {
namespace cdr {
  
class CDR {

  public:

  	static const CORBA::ULong MAX_ALIGNMENT;
  	
  	static const CORBA::ULong OCTET_SIZE;
  	static const CORBA::ULong BOOLEAN_SIZE;
  	static const CORBA::ULong CHAR_SIZE;
  	
  	static const CORBA::ULong WCHAR_SIZE;
  	static const CORBA::ULong SHORT_SIZE;
  	static const CORBA::ULong USHORT_SIZE;
  
  	static const CORBA::ULong LONG_SIZE;
  	static const CORBA::ULong ULONG_SIZE;
  	static const CORBA::ULong FLOAT_SIZE;
  
  	static const CORBA::ULong LONGLONG_SIZE;
  	static const CORBA::ULong ULONGLONG_SIZE;
  	static const CORBA::ULong DOUBLE_SIZE;
  	static const CORBA::ULong LONG_DOUBLE_SIZE;
  	
  	static const CORBA::Boolean BIG_ENDIAN;
  	static const CORBA::Boolean LITTLE_ENDIAN;
  	static const CORBA::Boolean LOCAL_BYTE_ORDER;
  	
	typedef unsigned char* Addr;
  	typedef CORBA::ULong AbsolutePosition;
    
    static CORBA::Boolean local_byte_order();
    static Addr align(Addr buffer);
  
};

} // namespace cdr
} // namespace core
} // namespace TIDorb

#endif 

