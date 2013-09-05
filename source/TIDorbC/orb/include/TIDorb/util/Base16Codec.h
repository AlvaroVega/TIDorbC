/*/////////////////////////////////////////////////////////////////////////
//
// File:        Base16Codec.h
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

#include "TIDorb/util.h"

#ifndef _TIDORB_UTIL_BASE_16_CODEC_H_
#define _TIDORB_UTIL_BASE_16_CODEC_H_

#include <sys/types.h>

#include <exception> 

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::exception;
#endif

namespace TIDorb {
namespace util{

class InvalidDataFormat : public virtual exception {};

class Base16Codec {

 public:

  /* return true if can be decoded */

  static void decode(const char* str, size_t str_length,
                     char*& decoded, size_t& decoded_length) 
    throw (InvalidDataFormat, CORBA::SystemException);
    
  static void decode(const char* str, size_t str_length,
                     char* decoded, size_t decoded_offset,
                     size_t decoded_length)
    throw (InvalidDataFormat, CORBA::SystemException);
  
  
  static void encode(const char* data, size_t data_length,
                     char*& str, size_t& str_length);
 

  static void encode(const char* data, size_t data_offset, size_t data_length,
                     char* str, size_t str_length);
 
  static const char toBase16[];

  static char getDigit(int hex_value) throw (InvalidDataFormat, CORBA::SystemException);
  
  static unsigned int getValue(char digit) throw (InvalidDataFormat, CORBA::SystemException);

};

} // util
} // TIDorb


#endif
