/////////////////////////////////////////////////////////////////////////
//
// File:        String_SequenceMember.C
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

#include "CORBA.h"

  // Tipo String_SequenceMember
  //
  // Este tipo es un tipo auxiliar no accesible al usuario disenado para
  // permitir que el tipo devuelto por uno de los operadores de indexacion,
  // tenga la misma semantica que los tipos de los miembros de las estructuras
  // o elementos de arrays cuando estos han sido definidos en IDL como de tipo
  // String
  //

#include "TIDorb/types.h"

char* TIDorb::types::String_SequenceMember::operator= (char* chp) {
	if (*release) 
		CORBA::string_free(*ptr_str);
	*ptr_str = chp;	// se consume
	return *ptr_str;
}
    
char* TIDorb::types::String_SequenceMember::operator=(const char* chp) {
	if (*release)
		CORBA::string_free(*ptr_str);
	*ptr_str = CORBA::string_dup(chp);	// se copia
	return *ptr_str;
}
    
    
char* TIDorb::types::String_SequenceMember::operator=(const CORBA::String_var& s) {
	if (*release)
		CORBA::string_free(*ptr_str);
	*ptr_str = CORBA::string_dup(s);	// se copia (cast automatico)
	return *ptr_str;
}
