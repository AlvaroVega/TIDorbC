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
