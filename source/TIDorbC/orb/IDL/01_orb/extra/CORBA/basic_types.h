#include "CORBA.h"


#ifndef _CORBA_BASIC_TYPES_H_
#define _CORBA_BASIC_TYPES_H_

#include <inttypes.h>
#include <wchar.h>

namespace CORBA {
  
//        C++ type        CORBA TYPE        SIZE
//--------------------------------------------------
  typedef bool            Boolean;      //  1 byte
  typedef char            Char;         //  1 byte
  typedef wchar_t         WChar;        //  4 bytes
  typedef uint8_t         Octet;        //  1 byte
  typedef int16_t         Short;        //  2 bytes
  typedef uint16_t        UShort;       //  2 bytes
  typedef int32_t         Long;         //  4 bytes
  typedef uint32_t        ULong;        //  4 bytes
  typedef int64_t         LongLong;     //  8 bytes
  typedef uint64_t        ULongLong;    //  8 bytes
  typedef float           Float;        //  4 bytes
  typedef double          Double;       //  8 bytes
  typedef long double     LongDouble;   // 16 bytes
  
  typedef Boolean& Boolean_out;
  typedef Char& Char_out;
  typedef WChar& WChar_out;
  typedef Octet& Octet_out;
  typedef Short& Short_out;
  typedef UShort& UShort_out;
  typedef Long& Long_out;
  typedef LongLong& LongLong_out;
  typedef ULong& ULong_out;
  typedef ULongLong& ULongLong_out;
  typedef Float& Float_out;
  typedef Double& Double_out;
  typedef LongDouble& LongDouble_out;
  
}


#endif
