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
