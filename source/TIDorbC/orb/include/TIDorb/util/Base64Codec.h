/*/////////////////////////////////////////////////////////////////////////
//
// File:        Base64Codec.h
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

#ifndef _TIDORB_UTIL_BASE_64_CODEC_H_
#define _TIDORB_UTIL_BASE_64_CODEC_H_

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

/**
 * BASE64 encoding encodes 3 bytes into 4 characters.
 *  |11111122|22223333|33444444|
 *  Each set of 6 bits is encoded according to the
 *  toBase64 map. If the number of input bytes is not
 *  a multiple of 3, then the last group of 4 characters
 *  is padded with one or two = signs. Each output line
 *  is at most 76 characters.
 */

class Base64Codec {

 public:
 
   static const size_t BYTE_GROUP = 3;
   static const size_t CHAR_GROUP = 4;

  /* 
   * decode: creates a char array data from a Base64 null terminated string
        str = the null terminated string;
   */

  static void decode(const char* str, char*& decoded, size_t& decoded_length) 
    throw (InvalidDataFormat, CORBA::SystemException);
    
  /* 
   * encode: creates a null terminated string encoding in Base64 format the char array data
   */   
  static void encode(const char* data, size_t data_length, char*& str);
 
 
  static const char toBase64[];

  static size_t position(char value)   throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException);
};

} // util
} // TIDorb


#endif
