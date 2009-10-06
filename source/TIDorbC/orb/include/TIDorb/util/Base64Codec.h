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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
