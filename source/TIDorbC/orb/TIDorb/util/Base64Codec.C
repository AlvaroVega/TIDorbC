/////////////////////////////////////////////////////////////////////////
//
// File:        Base64Codec.C
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
#include <string.h>

void TIDorb::util::Base64Codec::decode (const char* str, char*& decoded, size_t& decoded_length)
throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException)
{
  decoded = 0;
  decoded_length = 0;

  if(str == NULL)
  return;

  size_t str_length = strlen(str);

  if((str_length % CHAR_GROUP) != 0)
  throw TIDorb::util::InvalidDataFormat();

  size_t data_chars = str_length;

  while(str[data_chars-1] == '=')
  data_chars--;

  size_t whites = str_length - data_chars;

  decoded_length = (data_chars / CHAR_GROUP) * BYTE_GROUP;

  if(whites == 1)
  decoded_length += 2;
  else if(whites == 2)
  decoded_length += 1;

  decoded = new char[decoded_length];

  char* decoded_aux = decoded;

  int group_value[CHAR_GROUP];

  size_t i = 0;

  size_t remain = decoded_length;

  for (size_t j = 0; j < data_chars; j += CHAR_GROUP) {
    if(remain >= 3) {
      group_value[0] = position(str[j]);
      group_value[1] = position(str[j+1]);
      group_value[2] = position(str[j+2]);
      group_value[3] = position(str[j+3]);

      decoded_aux[i++] = (char) ((group_value[0]  << 2) | (group_value[1] >> 4));
      decoded_aux[i++] = (char) ((group_value[1] << 4) | (group_value[2] >> 2));
      decoded_aux[i++] = (char) ((group_value[2] << 6) | group_value[3]);
    } else if(remain == 2) {
      group_value[0] = position(str[j]);
      group_value[1] = position(str[j+1]);
      group_value[2] = position(str[j+2]);

      decoded_aux[i++] = (char) ((group_value[0]  << 2) | (group_value[1] >> 4));
      decoded_aux[i++] = (char) ((group_value[1] << 4) | (group_value[2] >> 2));
    } else if(remain == 1) {
      decoded_aux[0] = position(str[j]);
      decoded_aux[1] = position(str[j+1]);

      decoded[i++] = (char) ((group_value[0]  << 2) | (group_value[1] >> 4));
    }

    remain -= BYTE_GROUP;
  }
}



void TIDorb::util::Base64Codec::encode(const char* data, size_t data_length, char*& str)
{
  str = 0;
  
  if(!data)
    return;
    
  size_t str_length = (data_length/BYTE_GROUP) * CHAR_GROUP;

  if((data_length % BYTE_GROUP) != 0)
  str_length += CHAR_GROUP;


  str = new char[str_length + 1];

  str[str_length] = '\0';

  size_t i = 0;
  size_t remain = data_length;
  for (size_t j = 0 ; j < data_length; j += BYTE_GROUP) {

    if (remain >= 3) {
      str[i++] = toBase64[(data[j] & 0xFC) >> 2];
      str[i++] = toBase64[((data[j] & 0x03) << 4) | ((data[j+1] & 0xF0) >> 4)];
      str[i++] = toBase64[((data[j+1] & 0x0F) << 2) | ((data[j+2] & 0xC0) >> 6)];
      str[i++] = toBase64[data[j+2] & 0x3F];
    } else if (remain == 1) {
      str[i++] = toBase64[(data[j] & 0xFC) >> 2];
      str[i++] = toBase64[(data[j] & 0x03) << 4];
      str[i++] = '=';
      str[i++] = '=';
    } else if (remain == 2) {
      str[i++] = toBase64[(data[j] & 0xFC) >> 2];
      str[i++] = toBase64[((data[j] & 0x03) << 4) | ((data[j+1] & 0xF0) >> 4)];
      str[i++] = toBase64[(data[j+1] & 0x0F) << 2];
      str[i++] = '=';
    }
    remain -= BYTE_GROUP;
  }
}



const char TIDorb::util::Base64Codec::toBase64[] =
{  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/'
};

size_t TIDorb::util::Base64Codec::position(char value)   throw (TIDorb::util::InvalidDataFormat, CORBA::SystemException)
{
  if((value >= 'A') && (value <= 'Z')) {
    return value - 'A';
  } else if((value >= 'a') && (value <= 'z')) {
    return value - 'a' + 26;
  } else if((value >= '0') && (value <= '9')) {
    return value - '0' + 52;
  } else if(value == '+') {
    return 62;
  } else if(value == '/') {
    return 63;
//  } else if(value == '=') {
//    return -1;
  } else
  throw TIDorb::util::InvalidDataFormat();
}
