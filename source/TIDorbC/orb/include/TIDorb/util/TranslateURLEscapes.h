/*/////////////////////////////////////////////////////////////////////////
//
// File:        TranslateURLEscapes.h
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
/////////////////////////////////////////////////////////////////////////*/

#ifndef __TIDorb_util_TranslateURLEscapes_H_
#define __TIDorb_util_TranslateURLEscapes_H_

#include "CORBA.h"

namespace TIDorb {
namespace util {


class TranslateURLEscapes {

  public:
   
    static char* put_escapes(const char* name);
    
    static char* translate_escapes(const char* str);
    
    static char* resolve_escapes(const char* str);
    
    static char* stringfy(const char* str);
    	
    static char* translate_unicode(const char* str);
    
    static char* substitute(const char* str, const char* from, const char* to);   
	
};
}
}

#endif
