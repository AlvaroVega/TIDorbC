/////////////////////////////////////////////////////////////////////////
//
// File:        Base16Codec.C
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/util.h"

void TIDorb::util::Base16Codec::decode (const char* str, size_t str_length,
					char*& decoded, size_t& decoded_length) 
  throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException)
{   
  if((str_length % 2) != 0)
    throw InvalidDataFormat();
  
  decoded_length = str_length / 2; 
  
  decoded = new char[decoded_length];
  
  decode(str, str_length, decoded, 0, decoded_length);
  
}
  
  
void TIDorb::util::Base16Codec::decode(const char* str, size_t str_length,
				       char* decoded, size_t decoded_offset,
				       size_t decoded_length)
  throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException)
{ 
  size_t final_position = decoded_offset + (str_length/2);
  
  if(decoded_length < final_position)
    throw InvalidDataFormat();
  
  size_t j = 0;
  
  for(size_t i = decoded_offset; i < final_position; i++)
    decoded[i]= (char) (16* (getValue(str[j++]))  + 
			getValue(str[j++]));  
  
}

void TIDorb::util::Base16Codec::encode(const char* data, size_t data_length,
	    char*& str, size_t& str_length)
{
  encode(data,0,data_length, str, str_length);    
}

void TIDorb::util::Base16Codec::encode(const char* data, size_t data_offset, 
                                       size_t data_length,
				       char* str, size_t str_length)
{   
  str_length = 2* (data_length - data_offset);
  
  str = new char[str_length];
  unsigned int octet = 0;
  size_t j = 0;
  
  size_t final_position = data_offset + str_length;
  
  for(size_t i = data_offset; i < final_position; i++) { 
    octet = 0xff & data[i];
    str[j++] = toBase16[octet>>4];
    str[j++] = toBase16[octet & 0xf];
  }
}

const char TIDorb::util::Base16Codec::toBase16[] = 
{'0','1','2','3','4','5','6','7',
 '8','9','a','b','c','d','e','f'};

char TIDorb::util::Base16Codec::getDigit(int hex_value)   
throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException)
{
  if (hex_value > 15)
    throw InvalidDataFormat();
  
  return toBase16[hex_value];
}

unsigned int TIDorb::util::Base16Codec::getValue(char digit)
throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException)
{
  if((digit >= '0') && (digit <= '9' ))
    return digit - '0';
  if((digit >= 'A') && (digit <= 'F' ))
    return digit - 'A' + 0xA;
  if((digit >= 'a') && (digit <= 'f' ))
    return digit - 'a' + 0xA;
  //error
  throw InvalidDataFormat(); 
}
