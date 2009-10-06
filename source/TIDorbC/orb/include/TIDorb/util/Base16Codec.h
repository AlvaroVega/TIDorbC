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
