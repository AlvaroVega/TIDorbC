/////////////////////////////////////////////////////////////////////////
//
// File:        TranslateURLEscapes.C
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


#include <string>

#ifdef TIDORB_HAVE_SSTREAM
#include <sstream>
#define TIDORB_ISTRINGSTREAM istringstream
#else
#ifdef TIDORB_HAME_STRSTREAM
#include <strstream>
#else
#include <strstream.h>
#endif
#define TIDORB_ISTRINGSTREAM istrstream
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
#endif

#include <stdlib.h>




char* TIDorb::util::TranslateURLEscapes::substitute(const char* source, const char* from, const char* to)
{
  string source_str = source;
  string::size_type position = source_str.find(from);
  
  if(position == string::npos)
    return CORBA::string_dup(source);
  else
    position = source_str.find_first_of(from);
    
  size_t from_length = strlen(from);
  string::size_type initial_position = 0;
  
  TIDorb::util::StringBuffer buffer;
  
  do {
    buffer << source_str.substr(initial_position, position - initial_position).c_str() << to;
    initial_position =  position + from_length;

    position = source_str.find_first_of(from, initial_position + 1);	
		
  } while (position != string::npos);
 	
  buffer << source_str.substr(initial_position);
 	
  return CORBA::string_dup(buffer.str().data());
}



   
char* TIDorb::util::TranslateURLEscapes::put_escapes(const char* name)
{
  char* init_str = translate_escapes(name);

  char* first_str  = substitute(init_str,   "%5c%5c", "\\\\");
  char* second_str = substitute(first_str,  "%5c/",   "\\<");
  char* third_str  = substitute(second_str, "%5c." ,  "\\>");
		
  CORBA::string_free(init_str);
  CORBA::string_free(first_str);
  CORBA::string_free(second_str);
		
  return third_str;
}			



	
char* TIDorb::util::TranslateURLEscapes::translate_escapes(const char* str)
{
  TIDorb::util::StringBuffer buffer;
	
  buffer << "";
	
  string no_escapes = ";/:?@&=+$,-_.!~*'()";
	
  char c = 0;
	
  size_t str_length = strlen(str);
  for (size_t i = 0; i < str_length; i++) {
    c = str[i];
    if ((no_escapes.find(c) == string::npos) && !isalnum(c))
      buffer << '%' << hex << c;
    else
      buffer << c;
  }

  return CORBA::string_dup(buffer.str().data());
}




	
char* TIDorb::util::TranslateURLEscapes::resolve_escapes(const char* str)
{
  char* first_str  = substitute(str,        "\\\\", "%5c%5c");
  char* second_str = substitute(first_str,  "\\<",  "%5c/");
  char* third_str  = substitute(second_str, "\\>",  "%5c.");
	
  CORBA::string_free(first_str);
  CORBA::string_free(second_str);
	
  return third_str;
}	



	
char* TIDorb::util::TranslateURLEscapes::stringfy(const char* str)
{
  char* first_str  = substitute(str,        "\\",  "\\\\");
  char* second_str = substitute(first_str,  ".",   "\\.");
  char* third_str  = substitute(second_str, "/",   "\\/" );
  char* forth_str  = substitute(third_str,  "%5c", "\\\\");

  CORBA::string_free(first_str);
  CORBA::string_free(second_str);
  CORBA::string_free(third_str);

  char* result = translate_unicode(forth_str);
  CORBA::string_free(forth_str);
	
  return result;
}



	
char* TIDorb::util::TranslateURLEscapes::translate_unicode(const char* str)
{
  string unicode = str;
  string hex_str;
  int nunicode; 
	
  TIDorb::util::StringBuffer buffer_out;

  string::size_type position = unicode.find('%'); // Reading %hexhex
  size_t initial_position = 0;
	
  // Fix to bug #387: TranslateURLEscapes::translate_unicode erase a character from a string
  if (position = string::npos)
    return strdup(unicode.c_str());

  while (position != string::npos)
  {
    hex_str = unicode.substr(position+1, 2);

    TIDORB_ISTRINGSTREAM buffer_in(hex_str.c_str());
    buffer_in >> hex >> nunicode;
	  
    buffer_out << unicode.substr(initial_position, position - 1 - initial_position) << (char) nunicode;
		
    initial_position = position + 3;
    position = unicode.find('%', position + 1);
  }		
	
  buffer_out << unicode.substr(initial_position + 1);
	
  return CORBA::string_dup(buffer_out.str().data());
}
