/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/CDR.h
  
 Revisions:
 
 Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
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

